#pragma once

class RobotMovingCount {
public:
	RobotMovingCount() {};
	RobotMovingCount(int t, int e_r, int e_c, int c, int r) { threshold = t; entrancepos_row = e_r; entrancepos_column = e_c;  rows = r; columns = c; };//entrance=row*column
	~RobotMovingCount() {};

public:
	int		GetDigitCount(const int num);
	int		MovingCount();
	int		MovingCountTree(bool visited[], int entran_col, int entran_row);
	bool	check(bool visited[], int entran_col, int entran_row);

private:
	int threshold;
	int entrancepos_row;
	int entrancepos_column;
	int columns;
	int rows;
};