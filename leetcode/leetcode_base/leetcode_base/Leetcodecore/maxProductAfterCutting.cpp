#include "maxProductAfterCutting.h"
#include "SortAlg.h"
#include <iostream>

using namespace std;

int maxProductAfterCutting::MaxProduct()
{
	int maxval = 0;
	S = new int[TLength];
	M = new int[TLength];
	memset(S, -1, sizeof(int)*TLength);
	memset(M, 0, sizeof(int)*TLength);
	M[0] = 1;
	M[1] = 1;
	M[2] = 2;

	maxval=CalProduct(TLength);

	cout << "Maxval:" << endl;
	for (int i=0 ;i<TLength;i++)
	{
		cout << i+1 << ":" << M[i]<<",";
	}
	cout << endl;
	cout << "Path:" << endl;
	for (int i=0; i < TLength; i++)
	{
		cout << i+1 << ":" << S[i]<<",";
	}
	cout << endl;

	delete []S;
	return maxval;
}

int maxProductAfterCutting::CalProduct(int len) 
{
	if (len < 2)
		return 1;
	else if (len == 2)
		return 1;
	else if (len == 3)
		return 3;
	
	for (int i=1;i<len;i++)
	{
		int templ= CalProduct(i);
		int tempr= CalProduct(len-i);
		int tempm = templ * tempr;
		int tempn = i * (len - i);
		tempm=tempm>tempn?tempm:tempn;
		tempm = tempm>len ? tempm : len;
		if (tempm>M[len-1])
		{
			M[len-1] = tempm;
			S[len-1] = i;
		}
	}
	return M[len-1];
}

int maxProductAfterCutting::UpperToBottom(int begin, int end, int k)
{
	return -1;
}

int maxProductAfterCutting::BottomToUpper(int begin, int end,int k) 
{
	return -1;
}

int maxProductAfterCutting::TestMaxProduct()
{
	return TestmaxProduct(TLength);
}

int maxProductAfterCutting::TestmaxProduct(int length)
{
	if (length < 2)
		return 0;
	if (length == 2)//当绳子原长是2时候，因为必须要切，所以只能切成1和1
		return 1;
	if (length == 3)
		return 2;

	int* product = new int[length + 1];//为什么加一，数组下标从0开始
	memset(product, 0, length + 1);
	product[1] = 1;
	product[2] = 2;//为什么不是上面的length==2时候的返回值1？之前的是总长度是2时候的子情况，
				   //而这里是当2作为切出的子长度时候，
				   //不切割了，保留2为好
	product[3] = 3;

	int Resultmax = 0;

	for (int i = 4; i <= length; ++i)
	{
		int tempmax = 0;
		for (int j = 1; j <= i / 2; ++j)
		{
			if (product[j] * product[i - j] > tempmax)
				tempmax = (product[j] * product[i - j]);
		}
		product[i] = tempmax;
	}
	Resultmax = product[length];
	delete[] product;
	return Resultmax;
}