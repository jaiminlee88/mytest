#include "InvestmentProblems.h"
#include <vector>
#include "../common/stdafx.h"

using namespace std;

void InvestmentProblems::calmax() 
{
	int profit[6][4] = {{0,0,0,0},{11,0,2,20},{12,5,10,21},{13,10,30,22},{14,15,32,23},{15,20,40,24}};//row:money column:proj
	int F[6][4];
	int f[6][4];
	int Money = money;
	int Project = project;
	memset(F, 0, sizeof(int) * 6 * 4);
	memset(f, 0, sizeof(int) * 6 * 4);
	for (int i=0;i<Money;i++)//initial...
	{
		F[i][0] = profit[i][0];
		f[i][0] = i;
	}

	for (int i=1;i<Project;i++)//project 1~n
	{
		for (int j=0;j<Money;j++)//total money 1~m
		{
			int temp = 0;
			int tag = -1;
			for (int k=0;k<=j;k++)//money for proj i
			{
				if ((F[k][i - 1] + profit[j - k][i]) > temp)//Fk(X)=max(Fk-1(X-xk)+fk(xk))
				{
					temp = F[k][i - 1] + profit[j - k][i];
					tag = j - k;
				}
			}
			F[j][i] = temp;
			f[j][i] = tag;
		}
	}

	//out put
	int maxProjuct = 0;
	int investprod = 0;
	for (int j = 0; j < Money; j++)
	{
		if (F[j][Project - 1] > maxProjuct)
		{
			maxProjuct = F[j][Project - 1];
			investprod = f[j][Project - 1];
		}
	}
	cout << "MaxProduct->" << maxProjuct << endl;
	cout << "P" << Project << "->" << investprod << ",";
	int leftmoney = Money - investprod;
	for (int i=Project-2;i>=0;i--)
	{
		investprod = f[leftmoney][i];
		leftmoney -= investprod;
		cout << "P" << i+1 << "->" << investprod << ",";
	}
}