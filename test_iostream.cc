#include<iostream>
#include<fstream>
#include<string>

using namespace std;

void test_cin(){
    cout << "enter numbers:" << endl;
    int sum = 0;
    int input;
    while (cin >> input) // 1 2 3 z 4 5, z is not a number, sum=6s
    {
        if (input == 0)
            break;
        sum += input;
    }
    cout << "last entered=" << input << endl;
    cout << "sum=" << sum << endl;
    cout << "cin.eof()=" << cin.eof() << " cin.bad()=" << cin.bad() << " cin.fail()=" << cin.fail() 
         << " cin.exceptions()=" << cin.exceptions() << endl;

    if (cin.fail() && !cin.eof()) {
        cin.clear();
        while(cin.get() != '\n') {
            continue;
        }
        while (cin >> input) // 1 2 3 z 4 5, z is not a number, sum=6s
        {
            if (input == 0)
                break;
            sum += input;
        }
    }

    cout << "last entered=" << input << endl;
    cout << "sum=" << sum << endl;
}

void test_outfile(){
    cout << "enter file name:" << endl;
    string filename;
    cin >> filename;

    ofstream fout(filename.c_str(), ios_base::app);
    fout << "for your eyes only!" << endl;

    cout << "enter your secret number:" << endl;
    float secret;
    cin >> secret;
    fout << "your secret number is " << secret << endl;
    fout.close();

    ifstream fin(filename.c_str());
    cout << "content in file[" << filename << "]:" << endl;
    char ch;
    while(fin.get(ch))
        cout << ch;
    cout << "done." << endl;
}

void test_binary_file(){
    ofstream fout_abc("abc.bin", ios_base::binary);
    string str_abc = "for your eyes only!";
    int str_size_abc = str_abc.size();
    fout_abc.write((char*)&str_size_abc, sizeof(str_size_abc));
    fout_abc.write((char*)str_abc.c_str(), str_size_abc);
    cout << "save-> size=" << str_size_abc << " str=" << str_abc.c_str() << endl;
    fout_abc.close(); // 不关闭，下面的读取会失败,或者用flush fout_abc.flush(); 原因是数据没有刷入文件，还在缓存中

    ifstream fin_abc("abc.bin", ios_base::binary);
    int size2_abc;
    fin_abc.read((char*)&size2_abc, sizeof(size2_abc));
    string str2_abc(size2_abc, '\0');
    fin_abc.read((char*)str2_abc.c_str(), size2_abc);
    cout << "size2=" << size2_abc << " str2=" << str2_abc << endl;
    fin_abc.close();

    // 写入字符串到二进制文件
    std::ofstream fout("data.bin", std::ios::binary);
    std::string str = "Hello, world!";
    int str_size = str.size();
    fout.write((char*)&str_size, sizeof(str_size));
    fout.write((char*)str.c_str(), str_size);
    fout.close();

    // 从二进制文件中读取字符串
    std::ifstream fin("data.bin", std::ios::binary);
    int size2;
    fin.read((char*)&size2, sizeof(size2));
    std::string str2(size2, '\0');
    fin.read(&str2[0], size2);
    fin.close();

    // 打印读取到的字符串
    std::cout << str2 << std::endl;


}
int main(){
    // test_cin();
    // test_outfile();
    test_binary_file();
    return 0;
}