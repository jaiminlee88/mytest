#include <iostream>
using namespace std;
class Test1
{
public :
	Test1(int num):n(num){}
private:
	int n;
};
class Test2
{
public :
	explicit Test2(int num):n(num){} // 可以防止单构造函数的隐式转换，把错误扼杀于编译阶段
private:
	int n;
};
 
int main()
{
	Test1 t1 = 12; // valid
	Test1 t11(12); // valid
	Test2 t2(13);
	// Test2 t3 = 14; // invalid 不能出现隐式转换
	return 0;
}