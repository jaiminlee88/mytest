#include "RobotMovingCount.h"
#include "SortAlg.h"
#include <iostream>

using namespace std;

int RobotMovingCount::MovingCount() 
{
	if (columns < 0 || rows < 0)
		return -1;
	
	int count = 0;
	bool *visited=new bool[columns*rows];
	memset(visited, false, sizeof(bool)*columns*rows);
	
	for (int i=0;i<rows;i++)
	{
		for (int j=0;j<columns;j++)
		{
			cout << visited[i * columns +j] << ", ";
		}
		cout << endl;
	}
	cout << endl;

	count = MovingCountTree(visited, entrancepos_row, entrancepos_column);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < columns; j++)
		{
			cout << visited[i * columns + j] << ", ";
		}
		cout << endl;
	}
	cout << endl;

	delete[] visited;
	cout << "moving count->" << count << endl;
	return count;
}

int RobotMovingCount::MovingCountTree(bool visited[],int entran_row,int entran_col)
{
	int count = 0;
	int c1 = 0;
	int c2 = 0;
	int c3 = 0;
	int c4 = 0;
	//current
	if (check(visited, entran_row, entran_col))
	{
		visited[entran_row*columns + entran_col] = true;
		
		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				cout << visited[i * columns + j] << ", ";
			}
			cout << endl;
		}
		cout << endl;

		count++;
		//check 4 direction
		c1 = MovingCountTree(visited, entran_row - 1, entran_col);
		c2 = MovingCountTree(visited, entran_row + 1, entran_col);
		c3 = MovingCountTree(visited, entran_row, entran_col + 1);
		c4 = MovingCountTree(visited, entran_row, entran_col - 1);
		count = count + c1 + c2 + c3 + c4;
	}
	return count;
}
bool RobotMovingCount::check(bool visited[], int entran_row, int entran_col)
{
	if (entran_col<0 || entran_col>=columns || entran_row<0 || entran_row>=rows)//boundary
		return false;

	if (visited[entran_row*columns+entran_col] == true)
		return false;

	if ((GetDigitCount(entran_row) + GetDigitCount(entran_col) )> threshold)
		return false;

	return true;
}

int RobotMovingCount::GetDigitCount(const int inputnum)
{
	if (inputnum <= 0)
		return 0;

	int num = inputnum;
	int sum = 0;
	while (num%10 || num/10)
	{
		sum += num % 10;
		num /= 10;
	}
	return sum;
}