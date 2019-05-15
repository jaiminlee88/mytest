#pragma once

class maxProductAfterCutting {
public:
	maxProductAfterCutting() {};
	maxProductAfterCutting(int l) { TLength = l; };
	~maxProductAfterCutting() {};

	void reset_len(int l) { TLength = l; }
public:
	int MaxProduct();
	int CalProduct(int len);
	int UpperToBottom(int begin, int end,int k);
	int	BottomToUpper(int begin, int end, int k);
	
	int TestMaxProduct();
	int TestmaxProduct(int len);
private:
	int TLength=0;
	int *S = nullptr;
	int *M = nullptr;
};