#pragma once
#include <vector>
#include <algorithm>
class Fraction//����
{
public:
	Fraction();
	Fraction(int a, int b);//a�Ǳ�������b�ǳ���
	void set(int a, int b);
	double calculate();
	double result;
private:
	int dividend_;//������
	int divisor_;//����
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
	std::vector<std::vector<double>> template_;//ģ��
	Fraction coefficient_;//ϵ��
	int row_;
	int column_;
};
double K_Neighbor(const int * matrix, const int row, const int column, const int K);
