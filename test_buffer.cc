#include <iostream>
#include <vector>
#include <iterator>
#include <mutex>
#include <thread>

using namespace std;

class FixSizeCache{
public:
    FixSizeCache(int size):
        cache_size(size),
        buffer(nullptr),
        pwrite(nullptr),
        pread(nullptr),
        fill_size(0) {
        if (!buffer) {
            buffer = new (nothrow) char[cache_size];
            if (!buffer) {
                throw bad_alloc();
            }
            pwrite = buffer;
        }
    }
    FixSizeCache() {
        FixSizeCache(1024);
    }
    ~FixSizeCache(){
        if(buffer) {
            delete[] buffer;
        }
        buffer = nullptr;
        pwrite = nullptr;
        pread = nullptr;
        cache_size = 0;
    }

    bool is_empty() {
        return fill_size == 0;
    }

    bool is_full() {
        return fill_size == cache_size;
    }

    int write(const char* pdata, int write_size) {
        if (pdata == nullptr || write_size <= 0) {
            return 0;
        }
        int remain_size = cache_size - fill_size;
        if (remain_size == 0) {
            return remain_size;
        }
        write_size = write_size >= remain_size ? remain_size : write_size;
        // cout << "write_size=" << write_size << endl;
        for (int i=0; i < write_size; ++i) {
            if (pwrite == nullptr) {
                pwrite = buffer;
            }
            if (pwrite == (buffer + cache_size)) {
                pwrite = buffer;
            }
            (*pwrite) = pdata[i];
            if (pread == nullptr) {
                pread = pwrite; // set begin, important
            }
            ++fill_size;
            ++pwrite;
        }
        if ((pwrite == (buffer + cache_size)) && (fill_size == cache_size)) {
            pwrite = nullptr;
        }

        // cout << "N: " << buffer << endl;
        return write_size;
    }

    int read(char* pdata, int read_size) {
        if ((pdata == nullptr) || (read_size <= 0)) {
            return 0;
        }
        if ((pread == nullptr) && (fill_size != 0)) {
            pread = buffer;
        }
        read_size = read_size > fill_size ? fill_size : read_size;
        for (int i=0; i < read_size; ++i) {
            if (pread == (buffer + cache_size)) {
                pread = buffer;
            }
            pdata[i] = (*pread);
            --fill_size;
            ++pread;
        }
        if (fill_size == 0) {
            pread = nullptr;
        }
        return read_size;
    }
private:
    int cache_size;
    char* buffer;
    char* pwrite;
    char* pread;
    int   fill_size;
};

std::mutex mlock;

void test() {
    FixSizeCache cache(5);
    vector<string> strs = {"abcdedf", "hijklmn", "opq", "rstuv", "uvwxyz", "1234567890"};
    for (auto& str : strs) {
        cout << "W: " << str << endl;
        int writeSize = cache.write(str.c_str(), str.size());
        cout << "w: ";
        copy(str.begin(),str.begin()+writeSize, ostream_iterator<char>(cout, ""));
        cout << endl;

        char readData[6];
        int readSize = cache.read(readData, sizeof(readData) - 1);
        readData[readSize] = '\0';
        cout << "R: " << readData << endl;
        cout << endl;
    }
}

void test1() {
    bool job_done = false;
    FixSizeCache mcache(5);
    std::string str = "Life is too short to spend time with people who suck the happiness out of you. If someone wants you in their life, they'll make room for you. You shouldn't have to fight for a spot. Never, ever insist yourself to someone who continuously overlooks your worth. And remember, it's not the people that stand by your side when you're at your best, but the ones who stand beside you when you're at your worst that are your true friends.";
    char* pdata = const_cast<char*>(str.c_str());
    cout << "Input:" << endl;
    while(*pdata != '\0') {
        cout << *pdata;
        ++pdata;
    }
    cout << endl;

    std::thread t1([&job_done](std::string& str, FixSizeCache& cache){
        char* pdata = const_cast<char*>(str.c_str());
        while(*pdata != '\0') {
            std::lock_guard<std::mutex> lock(mlock);
            while(!cache.is_full()) {
                int writeSize = cache.write(pdata, 1);
                if (writeSize == 0) {
                    break;
                }
                ++pdata;
            }
        }
        job_done = true;
        cout << "thread t1 exit." << endl;
    }, std::ref(str), std::ref(mcache));

    std::thread t2([&job_done](FixSizeCache& cache){
        while(!job_done) {
            while(!cache.is_empty()) {
                char readData[6];
                int readSize = 0;
                {
                std::lock_guard<std::mutex> lock(mlock);
                readSize = cache.read(readData, sizeof(readData) - 1);
                }
                readData[readSize] = '\0';
                cout << readData;
            }
        }
        if (!cache.is_empty()) {
            char readData[6];
            int readSize = 0;
            {
            std::lock_guard<std::mutex> lock(mlock);
            readSize = cache.read(readData, sizeof(readData) - 1);
            }
            readData[readSize] = '\0';
            cout << readData;
        }
        cout << endl;
        cout << "thread t2 exit." << endl;
    }, std::ref(mcache));
    t1.join();
    t2.join();
}

int main() {
    test();
    test1();

    return 0;
}