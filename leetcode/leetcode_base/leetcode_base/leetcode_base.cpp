// leetcode_base.cpp: 定义控制台应用程序的入口点。
//

#include "./common/stdafx.h"
#include "Leetcodecore/SortAlg.h"
#include "Leetcodecore/RobotMovingCount.h"
#include "Leetcodecore/maxProductAfterCutting.h"
#include "Leetcodecore/InvestmentProblems.h"

using namespace std;

int main()
{
	/*char a[] = "abcd";
	SortAlg mSort;
	mSort.Sort();*/

	/*RobotMovingCount mRobot(18,0,0,3,3);
	int count=mRobot.MovingCount();*/

	/*maxProductAfterCutting mCut(5);
	mCut.reset_len(20);
	int maxval = mCut.MaxProduct();
	int tmaxval = mCut.TestMaxProduct();*/

	InvestmentProblems mInvestPro;
	mInvestPro.calmax();

    return 0;
}

