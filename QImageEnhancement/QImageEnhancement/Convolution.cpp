#include "Convolution.h"



Convolution::Convolution()
{
}

Convolution::Convolution(const double * matrix, const int row, const int column, const Fraction coef)
{
	set(matrix, row, column, coef);
}

void Convolution::set(const double *matrix, const int row, const int column,Fraction coef)
{
	template_.resize(row);
	row_ = row;
	column_ = column;
	for (int i = 0; i < row_; i++)
	{
		template_[i].resize(column_);
	}
	coefficient_ = coef;
	int pos = 0;
	for (int i = 0; i < row_; i++)
	{
		for (int j = 0; j < column_; j++)
		{
			template_[i][j] = matrix[pos++];
		}
	}
}

double Convolution::calcuclate(const int *matrix)
{
	int pos = 0;
	double ans = 0;
	for (int i = 0; i < row_; i++)
	{
		for (int j = 0; j < column_; j++)
		{
			ans += matrix[pos++]*template_[i][j]*coefficient_.result;
		}
	}
	return ans;
}

double Convolution::midCalculate(const int * matrix)
{
	std::vector<double> ans;
	ans.resize(row_*column_);
	int pos=0;
	for(int i=0;i<row_;i++)
		for (int j = 0; j < column_; j++)
		{
			ans[pos++] = matrix[pos] * template_[i][j] * coefficient_.result;
		}
	sort(ans.begin(), ans.end());
	return ans[pos / 2];
}
struct node{
	int value;
	int pos;
};
node getmin(std::vector<node>&array, node flag, int i)
{
	node temp;
	if (i == 0)
	{
		temp.pos = -1;
		temp.value = -1;
		return temp;
	}
	if (array[i-1].value > flag.value)
		temp=getmin(array, flag, i - 1);
	else
	{
		temp.pos = -1;
		temp.value = -1;
		return temp;
	}
	if (temp.value == -1)
	{
		node x = array[i-1];
		array[i-1] = flag;
		return x;
	}
	node x = array[i-1];
	array[i-1] = temp;
	return x;

}
double K_Neighbor(const int * matrix, const int row, const int column, const int K)
{
	std::vector<node> K_neighbor;
	K_neighbor.resize(K);
	for (int i = 0; i < K; i++)
		K_neighbor[i].value = 999;
	int pos = 0;
	double ans = 0;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			node flag;
			flag.value= abs(matrix[pos] - matrix[row * column / 2]);
			flag.pos = pos;
			getmin(K_neighbor, flag, K);
			pos++;
		}
	}
	pos = 0;
	for (int i = 0; i < K; i++)
		ans += (double)matrix[K_neighbor[i].pos] / (double)K;
	return ans;
}


Convolution::~Convolution()
{
}

Fraction::Fraction()
{
}

Fraction::Fraction(int a, int b)
{
	set(a, b);
}

void Fraction::set(int a, int b)
{
	dividend_ = a;
	divisor_ = b;
	calculate();
}

double Fraction::calculate()
{
	if (divisor_ == 0)
		return 0;
	result = (double)dividend_ / (double)divisor_;
	return result;
}
