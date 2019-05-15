#include "SortAlg.h"
#include <iostream>
using namespace std;

SortAlg::SortAlg(char* input) 
{
	int i = 0;
	while (input[i] != '\0')
	{
		if (input[i]=='\0')
			break;
		else 
			InputArray[i] = input[i++];
	}
}

void SortAlg::Sort()//chose proper alg
{
	int input[] = { 5,4,3,2,1 };
	print(input,5);
	QuickSort(input,0,4);
}
void SortAlg::QuickSort(int a[], int lo, int hi) 
{
	if (lo >= hi)
		return;

	int v = a[lo];
	int i = lo;
	int j = hi + 1;
	while (true)
	{
		while (less(a[++i],v))	if (i==hi)	break;
		while (less(v,a[--j]))	if (j==lo)	break;
		if (i>=j)	break;
		exch(a, i, j);
		print(a, 5);
	}
	exch(a, lo, j);
	print(a, 5);
	QuickSort(a, lo, j - 1);
	QuickSort(a, j + 1, hi);
}

void SortAlg::print(int a[],int num)
{
	for (int i=0;i<num;i++)
	{
		cout << a[i]<<", ";
	}
	cout <<endl;
}

void SortAlg::Show() 
{

}