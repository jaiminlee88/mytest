#include<iostream>
#include<set>
#include<algorithm>
#include<iterator>
#include <vector>
#include <queue>
using namespace std;


void test1() {
    /**
     * 
     *  struct Node
        {
            int x,y;
            bool operator <(Node a) const  {  return y < a.y; }
            bool operator >(Node a) const  {  return y > a.y; }
        };
        priority_queue<Node> A;                    //大根堆
        priority_queue<Node, vector<Node>, greater<Node> > B;    //小根堆 


        方式一：

        struct Node
        {int adj;
        int val;
        friend  bool operator<(const Node &a,const Node &b) { return  a.val > b.val; }
        };
        priority_queue<Node>Q; 
        方式二：（cmp将结构体以val由大到小排列，组成大根堆）一般也用这个！

        struct Node
        {int adj;
        int val;
        };
        struct cmp
        {bool operator()(Node a,Node b) { return  a.val > b.val; }
        };
        priority_queue<Node,vector<Node>,cmp>Q; 
        方式三：

        struct TMax
        {
            TMax(int tx):x(tx){}
            int x;
        }; 

        struct TMin
        {
            TMin(int tx):x(tx){}
            int x;
        }; 

        bool operator<(const TMax &a, const TMax &b)
        {
            return a.x<b.x;
        } 

        bool operator<(const TMin &a, const TMin &b)
        {
            return a.x>b.x;
        } 

        priority_queue<TMax> hmax;    //大顶堆
        priority_queue<TMin> hmin;    //小顶堆 
        3.下面是将指针存在优先队列中的方式

        struct Node
        {
            short f;
            short d;
            short fishs;
            short times;
            short i;
        }; 

        struct PCmp
        {
            bool operator () (Node const *x, Node const *y)
            {
                if(x->f == y->f)
                    return x->i > y->i;
                return x->f < y->f;
            }
        }; 

        priority_queue<Node*, vector<Node*>, PCmp > Q;
        注：在这种情况下往往可以预分配空间以避免new带来的麻烦。例如：堆中定义Node Qt[26]， 栈中的使用则为Node *tmp1 = Qt。

        经过测试，在优选队列当中存指针进行一系列操作要比存节点进行一系列操作快一些。

        

        注：

        less<class T>这是大顶堆，按值大的优先，值大的在最上面。greater<class T>这是小顶堆，按值小的优先，值小的在最上面。
        自定义cmp如果还有不明白的看这里：
        struct cmp
        {
        bool operator()（const int &a,const int &b）//这里的&是引用
        {
        return a>b;//最大堆
        return a<b;//最小堆
        }
        };
        priority_queue< int, vector<int>, cmp >
        还是自定义cmp函数，注意，一般ACM中用结构体内含“bool operator()（const int &a,const int &b）”。这其实等价于Class cmp，不过更省事，当然也不规范（不需要规范）。
        return就是希望如何排列为true。如果希望由大到小，就将大到小的情况return；反则亦然。和sort的自定义cmp是一样的。
            * 
     **/
    cout << "test1====================" << endl;
    std::priority_queue<int, vector<int>, greater<int>> q;
    vector<int> v = {1, 8, 5, 6, 3, 4, 0, 9, 7, 2};
    for (auto i : v) {
        q.push(i);
    }
    while (!q.empty()) {
        cout << q.top() << " ";
        q.pop();
    }
    cout << endl;
}

void test3() {
    vector<int> reward1 = {1,2,3};
    vector<int> reward2 = {3,2,1};
    struct Node {
        Node(int _r1, int _r2) : r1(_r1), r2(_r2) {}
        int r1;
        int r2;
    };
    struct cmp {
        bool operator()(const Node& a, const Node& b) const {
            return a.r1 < b.r1 || (a.r1 == b.r1 && a.r2 > b.r2);// 满足树底部走
        }
    };
    priority_queue<Node, vector<Node>, cmp> q;                    //大根堆
    for (int i = 0; i < reward1.size(); i++) {
        Node node(reward1[i], reward2[i]);
        q.push(node);
    }
}
void test2(){
    // algorithm heap
    //     STL heap包括：

    // 1 push_heap算法：新元素插入在底层的vector的end()处。向上回溯

    // 2 pop_heap算法：把堆顶元素和数值末尾元素调换，向下回溯。

    // 3 sort_heap算法：持续对整个heap做pop_heap操作，每次将操作范围从后向前缩减一个元素。执行过后，原来的heap不再是个合法的heap了。

    // 4 meak_heap算法：找出第一个需要重排的子树头部(n-2)/2，从当前位置向根节点回溯。
    cout << "test2====================" << endl;
    vector<int> v = {1, 8, 5, 6, 3, 4, 0, 9, 7, 2};
    make_heap(v.begin(), v.end());
    for (auto i : v) {
        cout << i << " ";
    }
    cout << endl;
}
int main(){
    /*
    begin()     　　 ,返回set容器的第一个元素
    end() 　　　　 ,返回set容器的最后一个元素
    clear()   　　     ,删除set容器中的所有的元素
    empty() 　　　,判断set容器是否为空
    max_size() 　 ,返回set容器可能包含的元素最大个数
    size() 　　　　 ,返回当前set容器中的元素个数
    rbegin　　　　 ,返回的值和end()相同
    rend()　　　　 ,返回的值和rbegin()相同
    */
    test1();
    test2();
    test3();
}