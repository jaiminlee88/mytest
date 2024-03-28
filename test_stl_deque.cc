#include<iostream>
#include<set>
#include<algorithm>
#include<iterator>
#include <vector>
#include <queue>
#include <deque>

using namespace std;

void test1() {
    deque<int> q;
    q.push_back(1);
    q.push_back(2);
    q.push_back(3);
    q.push_back(4);

    cout << "q.size()=" << q.size() << " front=" << q.front() << " back:" << q.back() << endl;
    cout << q[0] << endl;
    cout << q[1] << endl;
    cout << q[2] << endl;
    cout << q[3] << endl;
    
    q.pop_front();
    cout << "q.size()=" << q.size() << " front=" << q.front() << " back:" << q.back() << endl;
}


int main() {
    /**
     * 
        std::deque（双端队列）通常使用分块的动态数组作为其底层数据结构。具体来说，
        它通常由多个连续的存储块（chunks）组成，每个存储块都是一个固定大小的数组（通常是几百个元素），用来存储元素。

        std::deque 的内部结构类似于一个数组的数组，每个存储块都是一个数组，
        用于存储一定数量的元素，而这些存储块则被组织成一个链表，用于实现双端插入和删除操作。
        这种结构使得 std::deque 在两端进行插入和删除操作时都能够达到较高的效率。
     * **/
    test1();
}