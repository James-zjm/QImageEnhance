#ifndef IMAGE_H
#define IMAGE_H
#include <QImage>
#include <ctime>
#include <cmath>
#include "Convolution.h"

#define PI 3.1415926535
#define RADIAN(angle) ((angle)*PI/180.0)

int inline max(const int &a, const int &b)
{
	return a > b ? a : b;
}

typedef struct
{
	int x = 0, y = 0;
}Point;

class Image
{
public:
	Image();
	Image(const Image& input);
	~Image();
	friend class ImageShow;
	friend class MainWindow;
protected:
	QString filename;
	QImage image;
	QSize picture_size_;
};

class Rotate :public Image
{
public:
	Rotate(Image a) :Image(a) { }
	int spin(double angle, QSize sizeIn);
	int spin_nearest_neighbor_interpolation(double angle, QSize sizeIn);
	int spin_Bilinear_interpolation(double angle, QSize sizeIn);
};

class ChangeSize :public Image
{
public:
	ChangeSize(Image a) :Image(a) { }
	int changeSize(double sizeScale, QSize sizeIn);
	int changeSize_nearest_neighbor_interpolation(double sizeScale, QSize sizeIn);
	int changeSize_Bilinear_interpolation(double sizeScale, QSize sizeIn);
};

class Translation :public Image
{
public:
	Translation(Image a) :Image(a) { }
	int translation(int x_dis, int y_dis, QSize sizeIn);
};
///=================================================================================================
/// <summary>	灰度图像分解为8为二值图像 </summary>
///
/// <remarks>	John Zjm, 2018/9/26. </remarks>
///=================================================================================================

class GrayDecompose :public Image
{
public:
	GrayDecompose(Image a) :Image(a) {}
	int turnBinaryImage(int order);
};

class Threshold :public Image
{
public:
	Threshold(Image a) :Image(a) {}
	int Thresholdchanging(int value);
};

class PointOperation :public Image
{
public:
	PointOperation(Image a) :Image(a) {}
	int linearFuncation(double k, double b);
	int nonLinearFuncation1(double c);
	int nonLinearFuncation2(double c);
};

class HistogramEqualization :public Image
{
public:
	HistogramEqualization(Image a) :Image(a) {}
	int HistogramEqualizationchanging();
};

class Smooth :public Image
{
public:
	Smooth(Image a) :Image(a) {}
	int average();
	int median();
	int Kneighbor();
private:
	Convolution conv;
};

class Sharpen :public Image
{
public:
	Sharpen(Image a) :Image(a) {}
	int laplace();
	int sobel();
	int Isotropic();
private:
	Convolution conv1;
	Convolution conv2;
};

class Custom :public Image
{
public:
	Custom(Image a) :Image(a) {}
	int begin(double *matrix,int row,int column);
private:
	Convolution conv;
};

class ToGray :public Image
{
public:
	ToGray(Image a) :Image(a) {}
	int to();
};

struct point
{
	int x;
	int y;
};
class EdgeDetection :public Image
{
public:
	EdgeDetection(Image a) :Image(a) {}
	int laplace2();
	int Kirschperation();
private:
	int move_x[8] = { 0, 1,1,1,0,-1,-1,-1};
	int move_y[8] = {-1,-1,0,1,1, 1,  0,-1};
	Convolution conv1;
	Convolution conv2;
	Convolution conv3;
	Convolution conv4;
	Convolution conv5;
	Convolution conv6;
	Convolution conv7;
	Convolution conv8;
};
#endif // IMAGE_H
