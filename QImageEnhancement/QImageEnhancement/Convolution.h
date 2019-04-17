#pragma once
#include <vector>
#include <algorithm>
class Fraction//分数
{
public:
	Fraction();
	Fraction(int a, int b);//a是被除数，b是除数
	void set(int a, int b);
	double calculate();
	double result;
private:
	int dividend_;//被除数
	int divisor_;//除数
};


class Convolution
{
public:
	Convolution();
	Convolution(const double *matrix, const int row, const int column, Fraction coef);
	void set(const double *matrix, const int row, const int column, Fraction coef);
	double calcuclate(const int *matrix);
	double midCalculate(const int *matrix);
	~Convolution();
private:
	std::vector<std::vector<double>> template_;//模板
	Fraction coefficient_;//系数
	int row_;
	int column_;
};
double K_Neighbor(const int * matrix, const int row, const int column, const int K);
