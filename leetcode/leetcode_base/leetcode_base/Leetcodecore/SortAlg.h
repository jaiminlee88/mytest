#pragma once

class SortAlg {
public:
	SortAlg(){}
	SortAlg(char* input);
	~SortAlg() {
	}

public:
	bool less(int v,int w) { return v < w ? true : false; }
	void exch(int a[], int i, int j) { int temp = a[i]; a[i] = a[j]; a[j] = temp;}
	void Sort();//chose proper alg
	void QuickSort(int a[],int lo,int hi);
	void print(int a[],int num);
	void Show();
private:
	char InputArray[256];
};