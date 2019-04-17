#include "image.h"

Image::Image()
{
	filename.clear();
}
Image::Image(const Image& input)
{
	this->filename = input.filename;
	this->image = input.image;
	this->picture_size_ = input.picture_size_;
}
Image::~Image()
{
}

int Rotate::spin(double angle, QSize sizeIn)//旋转图像
{
	QImage imgRotate;
	QMatrix matrix;

	/* modify angle */
	matrix.rotate(angle);
	imgRotate = image.transformed(matrix);

	/* modify scale */
	image = imgRotate.scaled(sizeIn.width(), sizeIn.height(), Qt::KeepAspectRatio);
	return 0;
}
//最近邻插值
int Rotate::spin_nearest_neighbor_interpolation(double angle, QSize sizeIn)
{
	int srcW = image.width();
	int srcH = image.height();
	//以图像中心为原点左上角，右上角，左下角和右下角的坐标,用于计算旋转后的图像的宽和高
	Point pLT, pRT, pLB, pRB;
	pLT.x = -srcW / 2; pLT.y = srcH / 2;
	pRT.x = srcW / 2; pRT.y = srcH / 2;
	pLB.x = -srcW / 2; pLB.y = -srcH / 2;
	pRB.x = srcW / 2; pRB.y = -srcH / 2;
	//旋转之后的坐标
	Point pLTN, pRTN, pLBN, pRBN;
	double sina = sin(RADIAN(angle));
	double cosa = cos(RADIAN(angle));
	pLTN.x = pLT.x*cosa + pLT.y*sina;
	pLTN.y = -pLT.x*sina + pLT.y*cosa;
	pRTN.x = pRT.x*cosa + pRT.y*sina;
	pRTN.y = -pRT.x*sina + pRT.y*cosa;
	pLBN.x = pLB.x*cosa + pLB.y*sina;
	pLBN.y = -pLB.x*sina + pLB.y*cosa;
	pRBN.x = pRB.x*cosa + pRB.y*sina;
	pRBN.y = -pRB.x*sina + pRB.y*cosa;
	//旋转后图像宽和高
	int desWidth = max(abs(pRBN.x - pLTN.x), abs(pRTN.x - pLBN.x));
	int desHeight = max(abs(pRBN.y - pLTN.y), abs(pRTN.y - pLBN.y));
	//分配旋转后图像的缓存
	QImage newImage(desWidth, desHeight, QImage::Format_ARGB32);
	//通过新图像的坐标，计算对应的原图像的坐标
	for (int i = 0; i < desWidth; i++)
	{
		for (int j = 0; j < desHeight; j++)
		{
			//转换到以图像为中心的坐标系，并进行逆旋转
			int tX = (i - desWidth / 2)*cos(RADIAN(360 - angle)) + (-j + desHeight / 2)*sin(RADIAN(360 - angle));
			int tY = -(i - desWidth / 2)*sin(RADIAN(360 - angle)) + (-j + desHeight / 2)*cos(RADIAN(360 - angle));
			//如果这个坐标不在原图像内，则不赋值
			if (tX > srcW / 2 || tX < -srcW / 2 || tY > srcH / 2 || tY < -srcH / 2)
			{
				continue;
			}
			//再转换到原坐标系下
			int tXN = tX + srcW / 2; 
			int tYN = abs(tY - srcH / 2);
			//值拷贝
			QColor oldcolor = QColor(image.pixel(tXN, tYN));
			newImage.setPixel(i, j, qRgb(oldcolor.red(),oldcolor.green(),oldcolor.blue()));
		}
	}
	this->image = newImage;
	return 0;
}
//双线性插值
int Rotate::spin_Bilinear_interpolation(double angle, QSize sizeIn)
{
	int srcW = image.width();
	int srcH = image.height();
	//以图像中心为原点左上角，右上角，左下角和右下角的坐标,用于计算旋转后的图像的宽和高
	Point pLT, pRT, pLB, pRB;
	pLT.x = -srcW / 2; pLT.y = srcH / 2;
	pRT.x = srcW / 2; pRT.y = srcH / 2;
	pLB.x = -srcW / 2; pLB.y = -srcH / 2;
	pRB.x = srcW / 2; pRB.y = -srcH / 2;
	//旋转之后的坐标
	Point pLTN, pRTN, pLBN, pRBN;
	double sina = sin(RADIAN(angle));
	double cosa = cos(RADIAN(angle));
	pLTN.x = pLT.x*cosa + pLT.y*sina;
	pLTN.y = -pLT.x*sina + pLT.y*cosa;
	pRTN.x = pRT.x*cosa + pRT.y*sina;
	pRTN.y = -pRT.x*sina + pRT.y*cosa;
	pLBN.x = pLB.x*cosa + pLB.y*sina;
	pLBN.y = -pLB.x*sina + pLB.y*cosa;
	pRBN.x = pRB.x*cosa + pRB.y*sina;
	pRBN.y = -pRB.x*sina + pRB.y*cosa;
	//旋转后图像宽和高
	int desWidth = max(abs(pRBN.x - pLTN.x), abs(pRTN.x - pLBN.x));
	int desHeight = max(abs(pRBN.y - pLTN.y), abs(pRTN.y - pLBN.y));
	//分配旋转后图像的缓存
	QImage newImage(desWidth, desHeight, QImage::Format_ARGB32);
	//通过新图像的坐标，计算对应的原图像的坐标
	for (int i = 0; i < desWidth; i++)
	{
		for (int j = 0; j < desHeight; j++)
		{
			//转换到以图像为中心的坐标系，并进行逆旋转
			double tX = (i - desWidth / 2)*cos(RADIAN(360 - angle)) + (-j + desHeight / 2)*sin(RADIAN(360 - angle));
			double tY = -(i - desWidth / 2)*sin(RADIAN(360 - angle)) + (-j + desHeight / 2)*cos(RADIAN(360 - angle));
			//如果这个坐标不在原图像内，则不赋值
			if (tX > srcW / 2 || tX < -srcW / 2 || tY > srcH / 2 || tY < -srcH / 2)
			{
				continue;
			}
			//再转换到原坐标系下
			double tXN = tX + srcW / 2;
			double tYN = abs(tY - srcH / 2);
			//值拷贝
			Point pp;
			pp.x = tXN;
			pp.y = tYN;
			QColor f0 = QColor(image.pixel(pp.x, pp.y));
			QColor f1 = QColor(image.pixel(pp.x + 1, pp.y));
			QColor f2 = QColor(image.pixel(pp.x, pp.y + 1));
			QColor f3 = QColor(image.pixel(pp.x + 1, pp.y + 1));
			QColor b1 =f0;
			QColor b2 = QColor(f1.red() - f0.red(), f1.green() - f0.green(), f1.blue() - f0.blue());
			QColor b3 = QColor(f2.red() - f0.red(), f2.green() - f0.green(), f2.blue() - f0.blue());
			QColor b4 = QColor(f3.red() - f2.red()-f1.red()+f0.red(), f3.green() - f2.green() - f1.green() + f0.green(), 
				f3.blue() - f2.blue() - f1.blue() + f0.blue());
			int red = b1.red() + b2.red()*(tXN - pp.x) + b3.red()*(tYN - pp.y) + b4.red()*(tXN - pp.x)*(tYN - pp.y);
			int green= b1.green() + b2.green()*(tXN - pp.x) + b3.green()*(tYN - pp.y) + b4.green()*(tXN - pp.x)*(tYN - pp.y);
			int blue= b1.blue() + b2.blue()*(tXN - pp.x) + b3.blue()*(tYN - pp.y) + b4.blue()*(tXN - pp.x)*(tYN - pp.y);
			if(red>255||green>255||blue>255)
				newImage.setPixel(i, j, qRgb(255, 255, 255));
			newImage.setPixel(i, j, qRgb(red, green, blue));
		}
	}
	this->image = newImage;
	return 0;
}

int ChangeSize::changeSize(double sizeScale, QSize sizeIn)
{
	image = image.scaled(sizeIn.width() * sizeScale,
		sizeIn.height() * sizeScale, Qt::KeepAspectRatio);
	picture_size_ = image.size();
	return 0;
}
int ChangeSize::changeSize_nearest_neighbor_interpolation(double sizeScale, QSize sizeIn)
{
	int srcW = image.width();
	int srcH = image.height();
	//旋转后图像宽和高
	int desWidth = srcW * sizeScale;
	int desHeight = srcH * sizeScale;
	//分配旋转后图像的缓存
	QImage newImage(desWidth, desHeight, QImage::Format_ARGB32);
	//通过新图像的坐标，计算对应的原图像的坐标
	for (double i = 0; i < desWidth; i++)
	{
		for (double j = 0; j < desHeight; j++)
		{
			int tXN = i / sizeScale + 1.0 / 2;
			int tYN = j / sizeScale + 1.0 / 2;
			//值拷贝
			QColor oldcolor = QColor(image.pixel(tXN, tYN));
			newImage.setPixel(i, j, qRgb(oldcolor.red(), oldcolor.green(), oldcolor.blue()));
		}
	}
	this->image = newImage;
	return 0;
}
int ChangeSize::changeSize_Bilinear_interpolation(double sizeScale, QSize sizeIn)
{
	int srcW = image.width();
	int srcH = image.height();
	//旋转后图像宽和高
	int desWidth = srcW * sizeScale;
	int desHeight = srcH * sizeScale;
	//分配旋转后图像的缓存
	QImage newImage(desWidth, desHeight, QImage::Format_ARGB32);
	//通过新图像的坐标，计算对应的原图像的坐标
	for (double i = 0; i < desWidth; i++)
	{
		for (double j = 0; j < desHeight; j++)
		{
			int tXN = i / sizeScale + 1.0 / 2;
			int tYN = j / sizeScale + 1.0 / 2;
			//值拷贝
			Point pp;
			pp.x = tXN;
			pp.y = tYN;
			QColor f0 = QColor(image.pixel(pp.x, pp.y));
			QColor f1 = QColor(image.pixel(pp.x + 1, pp.y));
			QColor f2 = QColor(image.pixel(pp.x, pp.y + 1));
			QColor f3 = QColor(image.pixel(pp.x + 1, pp.y + 1));
			QColor b1 = f0;
			QColor b2 = QColor(f1.red() - f0.red(), f1.green() - f0.green(), f1.blue() - f0.blue());
			QColor b3 = QColor(f2.red() - f0.red(), f2.green() - f0.green(), f2.blue() - f0.blue());
			QColor b4 = QColor(f3.red() - f2.red() - f1.red() + f0.red(), f3.green() - f2.green() - f1.green() + f0.green(),
				f3.blue() - f2.blue() - f1.blue() + f0.blue());
			int red = b1.red() + b2.red()*(tXN - pp.x) + b3.red()*(tYN - pp.y) + b4.red()*(tXN - pp.x)*(tYN - pp.y);
			int green = b1.green() + b2.green()*(tXN - pp.x) + b3.green()*(tYN - pp.y) + b4.green()*(tXN - pp.x)*(tYN - pp.y);
			int blue = b1.blue() + b2.blue()*(tXN - pp.x) + b3.blue()*(tYN - pp.y) + b4.blue()*(tXN - pp.x)*(tYN - pp.y);
			if (red > 255 || green > 255 || blue > 255)
				newImage.setPixel(i, j, qRgb(255, 255, 255));
			newImage.setPixel(i, j, qRgb(red, green, blue));
		}
	}
	this->image = newImage;
	return 0;
}

int Translation::translation(int w_dis, int h_dis, QSize sizeIn)
{
	int srcW = image.width();
	int srcH = image.height();
	//旋转后图像宽和高
	int desWidth = srcW + abs(w_dis);
	int desHeight = srcH + abs(h_dis);
	//分配旋转后图像的缓存
	QImage newImage(desWidth, desHeight, QImage::Format_ARGB32);
	//通过新图像的坐标，计算对应的原图像的坐标
	for (double i = 0; i < desWidth; i++)
	{
		for (double j = 0; j < desHeight; j++)
		{
			int tXN = i -w_dis;
			int tYN = j -h_dis;
			if (tXN <0 || tXN>image.width() || tYN<0 || tXN>image.height())
				continue;
			//值拷贝
			QColor oldcolor = QColor(image.pixel(tXN, tYN));
			newImage.setPixel(i, j, qRgb(oldcolor.red(), oldcolor.green(), oldcolor.blue()));
		}
	}
	this->image = newImage;
	return 0;
}
///=================================================================================================
/// <summary>	turn the binary image. </summary>
///
/// <remarks>	John Zjm, 2018/9/26. </remarks>
///
/// <param name="order">	第几位二值图像. </param>
///
/// <returns>	操作是否成功. </returns>
///=================================================================================================

int GrayDecompose::turnBinaryImage(int order)
{
	QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);
	QColor oldColor;
	for (int x = 0; x < newImage.width(); x++)
	{
		for (int y = 0; y < newImage.height(); y++)
		{
			oldColor = QColor(image.pixel(x, y));
			unsigned int red = oldColor.red()& (1 << order);
			unsigned int green = oldColor.green()& (1 << order);
			unsigned int blue = oldColor.blue()& (1 << order);
			if ((red * 299 + green * 587 + blue * 114) / 1000 >= 1.0)
				newImage.setPixel(x, y, qRgb(0, 0, 0));//黑
			else
				newImage.setPixel(x, y, qRgb(255, 255, 255));//白
		}
	}
	this->image = newImage;
	return 0;
}
int Threshold::Thresholdchanging(int value)
{

	QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);
	QColor oldColor;
	for (int x = 0; x < newImage.width(); x++)
	{
		for (int y = 0; y < newImage.height(); y++)
		{
			oldColor = QColor(image.pixel(x, y));
			int gray = (oldColor.red() * 299 + oldColor.green() * 587 + oldColor.blue() * 114) / 1000;
			if (gray < value)
				newImage.setPixel(x, y, qRgb(0, 0, 0));//黑
			else
				newImage.setPixel(x, y, qRgb(255, 255, 255));//白
		}
	}
	this->image = newImage;
	return 0;
}

int PointOperation::linearFuncation(double k, double b)
{
	QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);
	QColor oldColor;
	for (int x = 0; x < newImage.width(); x++)
	{
		for (int y = 0; y < newImage.height(); y++)
		{
			oldColor = QColor(image.pixel(x, y));
			unsigned int gray = (oldColor.red() * 299 + oldColor.green() * 587 + oldColor.blue() * 114) / 1000;
			unsigned int output = k * gray + b;
			if (output > 255)
				newImage.setPixel(x, y, 255);
			newImage.setPixel(x, y, qRgb(output, output, output));
		}
	}
	this->image = newImage;
	return 0;
}
int PointOperation::nonLinearFuncation1(double c)
{
	QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);
	QColor oldColor;
	for (int x = 0; x < newImage.width(); x++)
	{
		for (int y = 0; y < newImage.height(); y++)
		{
			oldColor = QColor(image.pixel(x, y));
			unsigned int gray = (oldColor.red() * 299 + oldColor.green() * 587 + oldColor.blue() * 114) / 1000;
			unsigned int output = gray + c * gray*(255 - gray);
			if (output > 255)
				newImage.setPixel(x, y, 255);
			newImage.setPixel(x, y, qRgb(output, output, output));
		}
	}
	this->image = newImage;
	return 0;
}
int PointOperation::nonLinearFuncation2(double c)
{
	QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);
	QColor oldColor;
	for (int x = 0; x < newImage.width(); x++)
	{
		for (int y = 0; y < newImage.height(); y++)
		{
			oldColor = QColor(image.pixel(x, y));
			double gray = (oldColor.red() * 299 + oldColor.green() * 587 + oldColor.blue() * 114) / 1000;
			unsigned int output = 255.0 / 2.0*(1 + tan(c*3.14159*(gray / 255.0 - 1.0 / 2.0)) / tan(3.14159 / 2.0 * c));
			if (output > 255)
				newImage.setPixel(x, y, 255);
			newImage.setPixel(x, y, qRgb(output, output, output));
		}
	}
	this->image = newImage;
	return 0;
}

int HistogramEqualization::HistogramEqualizationchanging()
{
	int gray_sum[257] = { 0 };
	double sum = 0;
	QColor Color;
	for (int x = 0; x < image.width(); x++)
	{
		for (int y = 0; y < image.height(); y++)
		{
			Color = QColor(image.pixel(x, y));
			unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
			gray_sum[gray]++;
			sum++;
		}
	}
	double s[257];
	for (int i = 0; i <= 255; i++)
	{
		if (i == 0)
			s[i] = gray_sum[i] / sum;
		else
			s[i] = s[i - 1] + gray_sum[i] / sum;
	}
	unsigned int l[257];
	for (int i = 0; i < 256; i++)
		l[i] = s[i] * (256 - 1) + 1 / 2;
	QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);
	for (int x = 0; x < newImage.width(); x++)
	{
		for (int y = 0; y < newImage.height(); y++)
		{
			Color = QColor(image.pixel(x, y));
			unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
			newImage.setPixel(x, y, qRgb(l[gray], l[gray], l[gray]));
		}
	}
	this->image = newImage;
	return 0;
}

int Smooth::average()
{
	double temp[9] = { 1,1,1,1,1,1,1,1,1 };
	Fraction coef(1, 9);
	conv.set(temp, 3, 3, coef);
	QColor Color;
	QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);
	for (int x = 0; x < newImage.width(); x++)
	{
		for (int y = 0; y < newImage.height(); y++)
		{
			int pos = 0;
			int window[9] = { 0 };
			for (int p = y - 1; p <= y + 1; p++)//行优先
				for (int q = x - 1; q <= x + 1; q++)
				{
					if (p < 0 || q < 0)
						window[pos++] = 0;
					else if (p >= newImage.height() || q >= newImage.width())
						window[pos++] = 0;
					else
					{
						Color = QColor(image.pixel(q, p));
						unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
						window[pos++] = gray;
					}
				}
			double ans = conv.calcuclate(window);
			if (ans > 255)
				ans = 255;
			if (ans < 0)
				ans = 0;
			newImage.setPixel(x, y, qRgb(ans, ans, ans));
		}
	}
	this->image = newImage;
	return 0;
}

int Smooth::median()
{
	double temp[9] = { 1,1,1,1,1,1,1,1,1 };
	Fraction coef(1, 1);
	conv.set(temp, 3, 3, coef);
	QColor Color;
	QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);
	for (int x = 0; x < newImage.width(); x++)
	{
		for (int y = 0; y < newImage.height(); y++)
		{
			int pos = 0;
			int window[9] = { 0 };
			for (int p = y - 1; p <= y + 1; p++)//行优先
				for (int q = x - 1; q <= x + 1; q++)
				{
					if (p < 0 || q < 0)
						window[pos++] = 0;
					else if (p >= newImage.height() || q >= newImage.width())
						window[pos++] = 0;
					else
					{
						Color = QColor(image.pixel(q, p));
						unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
						window[pos++] = gray;
					}
				}
			double ans = conv.midCalculate(window);
			if (ans > 255)
				ans = 255;
			if (ans < 0)
				ans = 0;
			newImage.setPixel(x, y, qRgb(ans, ans, ans));
		}
	}
	this->image = newImage;
	return 0;
}

int Smooth::Kneighbor()
{
	QColor Color;
	QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);
	for (int x = 0; x < newImage.width(); x++)
	{
		for (int y = 0; y < newImage.height(); y++)
		{
			int pos = 0;
			int window[9] = { 0 };
			for (int p = y - 1; p <= y + 1; p++)//行优先
				for (int q = x - 1; q <= x + 1; q++)
				{
					if (p < 0 || q < 0)
						window[pos++] = 0;
					else if (p >= newImage.height() || q >= newImage.width())
						window[pos++] = 0;
					else
					{
						Color = QColor(image.pixel(q, p));
						unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
						window[pos++] = gray;
					}
				}
			double ans = K_Neighbor(window,3,3,3);
			if (ans > 255)
				ans = 255;
			if (ans < 0)
				ans = 0;
			newImage.setPixel(x, y, qRgb(ans, ans, ans));
		}
	}
	this->image = newImage;
	return 0;
}

int Sharpen::laplace()
{
	double temp[9] = { 0,-1,0,-1,4,-1,0,-1,0 };
	Fraction coef(1, 1);
	conv1.set(temp, 3, 3, coef);
	QColor Color;
	QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);
	for (int x = 0; x < newImage.width(); x++)
	{
		for (int y = 0; y < newImage.height(); y++)
		{
			int pos = 0;
			int window[9] = { 0 };
			for (int p = y - 1; p <= y + 1; p++)//行优先
				for (int q = x - 1; q <= x + 1; q++)
				{
					if (p < 0 || q < 0)
						window[pos++] = 0;
					else if (p >= newImage.height() || q >= newImage.width())
						window[pos++] = 0;
					else
					{
						Color = QColor(image.pixel(q, p));
						unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
						window[pos++] = gray;
					}
				}
			double ans = conv1.calcuclate(window);
			if (ans > 255)
				ans = 255;
			if (ans < 0)
				ans = 0;
			newImage.setPixel(x, y, qRgb(ans, ans, ans));
		}
	}
	this->image = newImage;
	return 0;
}

int Sharpen::sobel()
{
	double temp[9] = { 1,0,-1,2,0,-2,1,0,-1 };
	Fraction coef(1, 1);
	conv1.set(temp, 3, 3, coef);

	double temp2[9] = { -1,-2,-1,0,0,0,1,2,1 };
	Fraction coef2(1, 1);
	conv2.set(temp2, 3, 3, coef2);

	QColor Color;
	QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);
	for (int x = 0; x < newImage.width(); x++)
	{
		for (int y = 0; y < newImage.height(); y++)
		{
			int pos = 0;
			int window[9] = { 0 };
			for (int p = y - 1; p <= y + 1; p++)//行优先
				for (int q = x - 1; q <= x + 1; q++)
				{
					if (p < 0 || q < 0)
						window[pos++] = 0;
					else if (p >= newImage.height() || q >= newImage.width())
						window[pos++] = 0;
					else
					{
						Color = QColor(image.pixel(q, p));
						unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
						window[pos++] = gray;
					}
				}
			double ans_x = conv1.calcuclate(window);
			double ans_y = conv2.calcuclate(window);
			double ans = abs(ans_x) + abs(ans_y);
			if (ans > 255)
				ans = 255;
			if (ans < 0)
				ans = 0;
			newImage.setPixel(x, y, qRgb(ans, ans, ans));
		}
	}
	this->image = newImage;
	return 0;
}

int Sharpen::Isotropic()
{
	double temp[9] = { 1,0,-1,sqrtf(2),0,-sqrtf(2),1,0,-1 };
	Fraction coef(1, 1);
	conv1.set(temp, 3, 3, coef);

	double temp2[9] = { -1,-sqrtf(2),-1,0,0,0,1,sqrtf(2),1 };
	Fraction coef2(1, 1);
	conv2.set(temp2, 3, 3, coef2);

	QColor Color;
	QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);
	for (int x = 0; x < newImage.width(); x++)
	{
		for (int y = 0; y < newImage.height(); y++)
		{
			int pos = 0;
			int window[9] = { 0 };
			for (int p = y - 1; p <= y + 1; p++)//行优先
				for (int q = x - 1; q <= x + 1; q++)
				{
					if (p < 0 || q < 0)
						window[pos++] = 0;
					else if (p >= newImage.height() || q >= newImage.width())
						window[pos++] = 0;
					else
					{
						Color = QColor(image.pixel(q, p));
						unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
						window[pos++] = gray;
					}
				}
			double ans_x = conv1.calcuclate(window);
			double ans_y = conv2.calcuclate(window);
			double ans = abs(ans_x)+abs(ans_y);
			if (ans > 255)
				ans = 255;
			if (ans < 0)
				ans = 0;
			newImage.setPixel(x, y, qRgb(ans, ans, ans));
		}
	}
	this->image = newImage;
	return 0;
}

int ToGray::to()
{
	QColor Color;
	QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);
	for (int x = 0; x < newImage.width(); x++)
	{
		for (int y = 0; y < newImage.height(); y++)
		{
			Color = QColor(image.pixel(x, y));
			unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
			newImage.setPixel(x, y, qRgb(gray, gray, gray));
		}
	}
	this->image = newImage;
	return 0;
}

int EdgeDetection::laplace2()
{
	srand(time(0));
	//double temp[25] = { 0,0,-1,0,0,  0,-1,-2,-1,0,  -1,-2,16,-2,-1, 0,-1,-2,-1,0, 0,0,-1,0,0 };
	//Fraction coef(1, 1);
	//conv1.set(temp, 5, 5, coef);
	//QColor Color;
	//QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);
	//for (int x = 0; x < newImage.width(); x++)
	//{
	//	for (int y = 0; y < newImage.height(); y++)
	//	{
	//		int pos = 0;
	//		int window[25] = { 0 };
	//		for (int p = y  -2; p <= y + 2; p++)//行优先
	//			for (int q = x  -2; q <= x + 2; q++)
	//			{
	//				if (p < 0 || q < 0)
	//					window[pos++] = 0;
	//				else if (p >= newImage.height() || q >= newImage.width())
	//					window[pos++] = 0;
	//				else
	//				{
	//					Color = QColor(image.pixel(q, p));
	//					unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
	//					window[pos++] = gray;
	//				}
	//			}
	//		double ans = conv1.calcuclate(window);
	//		if (ans > 255)
	//			ans = 255;
	//		if (ans < 0)
	//			ans = 0;
	//		newImage.setPixel(x, y, qRgb(ans, ans, ans));
	//	}
	//}
	double temp[9] = { 0,-1,0,-1,4,-1,0,-1,0 };
	Fraction coef(1, 1);
	conv1.set(temp, 3, 3, coef);
	QColor Color;
	QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);
	for (int x = 0; x < newImage.width(); x++)
	{
		for (int y = 0; y < newImage.height(); y++)
		{
			int pos = 0;
			int window[9] = { 0 };
			for (int p = y - 1; p <= y + 1; p++)//行优先
				for (int q = x - 1; q <= x + 1; q++)
				{
					if (p < 0 || q < 0)
						window[pos++] = 0;
					else if (p >= newImage.height() || q >= newImage.width())
						window[pos++] = 0;
					else
					{
						Color = QColor(image.pixel(q, p));
						unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
						window[pos++] = gray;
					}
				}
			double ans = conv1.calcuclate(window);
			if (ans > 255)
				ans = 255;
			if (ans < 0)
				ans = 0;
			newImage.setPixel(x, y, qRgb(ans, ans, ans));
		}
	}

	//std::vector<std::vector<bool>> vis(newImage.height(), std::vector<bool>(newImage.width(), 0));
	////bool vis[1000][1000];
	//int curr_i = 0, size = 0, counts, threshold = 200,st=0,en=0, curr_i_copy=0;
	//std::vector<point> edge;
	//QColor randColor = QColor(0, 255, 0);
	//for (int x = 0; x < newImage.width(); x++)
	//{
	//	for (int y = 0; y < newImage.height(); y++)
	//	{
	//		Color = QColor(image.pixel(x, y));
	//		unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
	//		if (gray > threshold && vis[y][x] != 1)//阈值
	//		{
	//			vis[y][x] = 1;//注意
	//			curr_i = -1;
	//			point now, next, stor;
	//			now.x = x;
	//			now.y = y;
	//			edge.clear();
	//			size = 0;
	//			while (1)
	//			{
	//				if (curr_i == -1)
	//				{
	//					st = 0;
	//					en = 8;
	//					curr_i = 0;
	//				}
	//				int gray_max = -1;
	//				for (counts = st; counts < en; counts++)
	//				{
	//					if (curr_i >= 8)
	//					{
	//						curr_i -= 8;
	//					}
	//					if (curr_i < 0)
	//					{
	//						curr_i += 8;
	//					}
	//					next.x = now.x + move_x[curr_i];
	//					next.y = now.y + move_y[curr_i];
	//					if ((next.x > 0) && (next.x < newImage.width() - 1) &&
	//						(next.y > 0) && (next.y < newImage.height() - 1) && 
	//						vis[next.y][next.x] != 1)
	//					{
	//						Color = QColor(newImage.pixel(next.x, next.y));
	//						int gray_next = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
	//						if (gray_next > gray_max)
	//						{
	//							gray_max = gray_next;
	//							stor = next;
	//							curr_i_copy = curr_i - 1;
	//						}
	//					}
	//					curr_i++;
	//				}
	//				//if (gray_max < threshold)
	//			//	break;
	//				if (size > newImage.width()*newImage.height())
	//					break;
	//				//if (stor.x == start.x&&stor.y == start.y)
	//				//	break;
	//				now = stor;
	//				vis[now.y][now.x] = 1;//注意
	//				curr_i = curr_i_copy;
	//				st = 0, en = 3;
	//				edge.push_back(now);
	//				//vis[now.y][now.x] = 1;
	//				size++;
	//			}
	//			if (size > 5)
	//			{
	//				for (int i = 0; i < size; i++)
	//				{
	//					newImage.setPixel(edge[i].x, edge[i].y, qRgb(randColor.red(), randColor.green(), randColor.blue()));

	//				}
	//			}
	//		}
	//	}
	//}


	int max = 0;
	point now, next,start,stor;
	std::vector<std::vector<bool>> vis(newImage.height(), std::vector<bool>(newImage.width(), 0));
	int curr_i = -1, size = 0, counts, threshold = 100, difference_threshold = 60, st = 0, en = 0;
	int curr_i_copy = -1;
	std::vector<point> edge;
	for (int x = 1; x < newImage.width()-1; x++)//忽略边界
	{
		for (int y = 1; y < newImage.height()-1; y++)
		{
			Color = QColor(newImage.pixel(x, y));
			unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
			if (gray > max)
			{
				max = gray;
				now.x = x;
				now.y = y;
			}
		}
	}
	start = now;
	//vis[now.y][now.x] = 1;//注意
	edge.clear();
	size = 0;
	edge.push_back(now);
	size++;
	while (1)
	{
		if (curr_i == -1)
		{
			st = 0;
			en = 8;
			curr_i = 0;
		}
		int gray_max = -1;
		for (counts = st; counts < en; counts++)
		{
			if (curr_i >= 8)
			{
				curr_i -= 8;
			}
			if (curr_i < 0)
			{
				curr_i += 8;
			}
			next.x = now.x + move_x[curr_i];
			next.y = now.y + move_y[curr_i];
			if ((next.x > 0) && (next.x < newImage.width()-1) &&
				(next.y > 0) && (next.y < newImage.height()-1)
				&&vis[next.y][next.x]!=1)
			{
				Color = QColor(newImage.pixel(next.x, next.y));
				int gray_next = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
				if (gray_next > gray_max)
				{
					gray_max = gray_next;
					stor = next;
					curr_i_copy = curr_i - 1;
				}
			}
			curr_i++;
		}
		//if (gray_max < threshold)
		//	break;
		if (size > newImage.width()*newImage.height())
			break;
		if (stor.x == start.x&&stor.y == start.y)
			break;
		now = stor;
		vis[now.y][now.x] = 1;//注意
		curr_i = curr_i_copy;
		st = 0, en = 3;
		edge.push_back(now);
		//vis[now.y][now.x] = 1;
		size++;
	}
	//if (size > 5)
	//{
		QColor randColor = QColor(rand() % 255, rand() % 255, rand() % 255);
		for (int i = 0; i < size; i++)
		{
			newImage.setPixel(edge[i].x, edge[i].y, qRgb(0, 255, 0));

		}
	this->image = newImage;
	return 0;
}

int EdgeDetection::Kirschperation()
{
	Fraction coef(1, 1);

	double temp1[9] = { +5,+5,+5 ,  -3,0,-3 ,  -3,-3,-3 };
	conv1.set(temp1, 3, 3, coef);

	double temp2[9] = { -3,+5,+5 , -3,0,+5 , -3,-3,-3 };
	conv2.set(temp2, 3, 3, coef);

	double temp3[9] = { -3,-3,+5 , -3,0,+5 , -3,-3,+5 };
	conv3.set(temp3, 3, 3, coef);

	double temp4[9] = { -3,-3,-3, -3,0,+5 , -3,+5,+5 };
	conv4.set(temp4, 3, 3, coef);

	double temp5[9] = { -3,-3,-3 , -3,0,-3 , +5,+5,+5 };
	conv5.set(temp5, 3, 3, coef);

	double temp6[9] = { -3,-3,-3, +5,0,-3 , +5,+5,-3 };
	conv6.set(temp6, 3, 3, coef);

	double temp7[9] = { +5, -3, -3 , +5,0,-3 , +5,-3,-3 };
	conv7.set(temp7, 3, 3, coef);

	double temp8[9] = { +5,+5,-3 , +5,0,-3 , -3,-3,-3 };
	conv8.set(temp8, 3, 3, coef);

	QColor Color;
	QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);
	for (int x = 0; x < newImage.width(); x++)
	{
		for (int y = 0; y < newImage.height(); y++)
		{
			int pos = 0;
			int window[9] = { 0 };
			for (int p = y - 1; p <= y + 1; p++)//行优先
				for (int q = x - 1; q <= x + 1; q++)
				{
					if (p < 0 || q < 0)
						window[pos++] = 0;
					else if (p >= newImage.height() || q >= newImage.width())
						window[pos++] = 0;
					else
					{
						Color = QColor(image.pixel(q, p));
						unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
						window[pos++] = gray;
					}
				}
			double max = 0,ans[9];
			ans[1] = conv1.calcuclate(window);
			ans[2] = conv2.calcuclate(window);
			ans[3] = conv3.calcuclate(window);
			ans[4] = conv4.calcuclate(window);
			ans[5] = conv5.calcuclate(window);
			ans[6] = conv6.calcuclate(window);
			ans[7] = conv7.calcuclate(window);
			ans[8] = conv8.calcuclate(window);
			for (int i = 1; i <= 8; i++)
			{
				if (max < ans[i])
					max = ans[i];
			}
			if (max > 255)
				max = 255;
			if (max < 0)
				max = 0;
			newImage.setPixel(x, y, qRgb(max, max, max));
		}
	}
	//std::vector<std::vector<bool>> vis(newImage.height(), std::vector<bool>(newImage.width(), 0));
	//int curr_i=0,size=0,counts, threshold = 100;
	//std::vector<point> edge;
	//for (int x = 0; x < newImage.width(); x++)
	//{
	//	for (int y = 0; y < newImage.height(); y++)
	//	{
	//		srand(time(0));
	//		unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
	//		if (gray > threshold &&vis[y][x]==0)
	//		{
	//			edge.clear();
	//			size = 0;
	//			vis[y][x] = 1;//注意
	//			QColor randColor = QColor(rand() % 255, rand() % 255, rand() % 255);	
	//			while (1)
	//			{
	//				point now, next;
	//				now.x = x;
	//				now.y = y;
	//				edge.push_back(now);
	//				size++;
	//				for (counts = 0; counts < 8; counts++)
	//				{
	//					if(curr_i >= 8)
	//					{
	//						curr_i -= 8;
	//					}
	//					if (curr_i < 0)
	//					{
	//						curr_i += 8;
	//					}
	//					next.x = now.x + move_x[curr_i];
	//					next.y = now.y + move_y[curr_i];
	//
	//					if ((next.x > 0) && (next.x < newImage.width()) &&
	//						(next.y > 0) && (next.y < newImage.height())&&
	//						vis[next.x][next.y]==0)
	//					{
	//						Color = QColor(image.pixel(next.x, next.y));
	//						unsigned int gray_next = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
	//						// 如果存在边缘
	//						if (gray_next > threshold)
	//						{
	//							curr_i -= 2;   // 更新当前方向
	//							// 更新跟踪的点
	//							now = next;
	//							edge.push_back(now);
	//							vis[now.y][now.x] = 1;
	//							size++;
	//							break;   // 跳出for循环
	//						}
	//					}
	//					curr_i++;
	//				}   // end for
	//				if (8 == counts)
	//				{
	//					// 清零
	//					curr_i = 0;
	//					break;
	//				}
	//				}
	//			if (size > 5)
	//			{
	//				for (int i = 0; i < size; i++)
	//					newImage.setPixel(edge[i].x, edge[i].y, qRgb(randColor.red(), randColor.green(), randColor.blue()));
	//			}
	//		}
	//	}
	//}

	int max = 0;
	point now, next, start, stor;
	std::vector<std::vector<bool>> vis(newImage.height(), std::vector<bool>(newImage.width(), 0));
	int curr_i = -1, size = 0, counts, threshold = 100, difference_threshold = 60, st = 0, en = 0;
	int curr_i_copy = -1;
	std::vector<point> edge;
	for (int x = 1; x < newImage.width() - 1; x++)//忽略边界
	{
		for (int y = 1; y < newImage.height() - 1; y++)
		{
			Color = QColor(newImage.pixel(x, y));
			unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
			if (gray > max)
			{
				max = gray;
				now.x = x;
				now.y = y;
			}
		}
	}
	start = now;
	//vis[now.y][now.x] = 1;//注意
	edge.clear();
	size = 0;
	edge.push_back(now);
	size++;
	while (1)
	{
		if (curr_i == -1)
		{
			st = 0;
			en = 8;
			curr_i = 0;
		}
		int gray_max = -1;
		for (counts = st; counts < en; counts++)
		{
			if (curr_i >= 8)
			{
				curr_i -= 8;
			}
			if (curr_i < 0)
			{
				curr_i += 8;
			}
			next.x = now.x + move_x[curr_i];
			next.y = now.y + move_y[curr_i];
			if ((next.x > 0) && (next.x < newImage.width() - 1) &&
				(next.y > 0) && (next.y < newImage.height() - 1)
				&& vis[next.y][next.x] != 1)
			{
				Color = QColor(newImage.pixel(next.x, next.y));
				int gray_next = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
				if (gray_next > gray_max)
				{
					gray_max = gray_next;
					stor = next;
					curr_i_copy = curr_i - 1;
				}
			}
			curr_i++;
		}
		//if (gray_max < threshold)
		//	break;
		if (size > newImage.width()*newImage.height())
			break;
		if (stor.x == start.x&&stor.y == start.y)
			break;
		now = stor;
		vis[now.y][now.x] = 1;//注意
		curr_i = curr_i_copy;
		st = 0, en = 3;
		edge.push_back(now);
		//vis[now.y][now.x] = 1;
		size++;
	}
	//if (size > 5)
	//{
	QColor randColor = QColor(rand() % 255, rand() % 255, rand() % 255);
	for (int i = 0; i < size; i++)
	{
		newImage.setPixel(edge[i].x, edge[i].y, qRgb(0, 255, 0));

	}
	this->image = newImage;
	return 0;
}

int Custom::begin(double *matrix, int row, int column)
{
	Fraction coef(1, 1);
	conv.set(matrix, row, column, coef);
	QColor Color;
	QImage newImage(image.width(), image.height(), QImage::Format_ARGB32);
	int *window=new int[row*column];
	for (int x = 0; x < newImage.width(); x++)
	{
		for (int y = 0; y < newImage.height(); y++)
		{
			int pos = 0;
			memset(window, 0, sizeof(window));
			for (int p = y - 1; p <= y + 1; p++)//行优先
				for (int q = x - 1; q <= x + 1; q++)
				{
					if (p < 0 || q < 0)
						window[pos++] = 0;
					else if (p >= newImage.height() || q >= newImage.width())
						window[pos++] = 0;
					else
					{
						Color = QColor(image.pixel(q, p));
						unsigned int gray = (Color.red() * 299 + Color.green() * 587 + Color.blue() * 114) / 1000;
						window[pos++] = gray;
					}
				}
			double ans = conv.calcuclate(window);
			if (ans > 255)
				ans = 255;
			if (ans < 0)
				ans = 0;
			newImage.setPixel(x, y, qRgb(ans, ans, ans));
		}
	}
	delete window;
	this->image = newImage;
	return 0;
}
