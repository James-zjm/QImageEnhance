#ifndef IMAGESHOW_H
#define IMAGESHOW_H
#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>
#include <QSize>
#include <QFileInfo>
#include <QFileInfoList>
#include <cmath>
#include <string>
#include "image.h"
#include "Access.h"

typedef QList<QPointF> DataList;

class ImageShow :public QWidget
{
public:
	explicit ImageShow(QWidget *parent = 0);
	explicit ImageShow(QWidget *parent, QString &caption, QString &dir, QString &filer);

	QWidget *parent;

	int index;
	double angle;
	double scale;//�Ŵ���С����
	double moveX;//��x����ƽ��
	double moveY;//��y����ƽ��
	QString path;
	QDir dir;
	QFileInfo fileInfo;
	QFileInfoList fileInfoList;
	QSize size;

	Image *original_image_;
	Image *show_image_;
	QPixmap pixmap;

	/* open a file */
	int openImageFile(const QString &caption, const QString &dir, const QString &filer);

	/* close file */
	int closeImageFile(void);

	/* delete file */
	int delImageFile(void);

	/* file skipping */
	int last(void);
	int next(void);

	/* change image */
	int zoomIn(void);
	int zoomOut(void);
	int spinToRight(void);
	int spinToLeft(void);
	int showBinary(int order);
	/* save image */
	int saveImageFile(const QString &caption, const QString &dir);
	//�Ҷ�ͳ��
	int grayStatistics(double &average, double &mid, double &standard, double &pixel, DataList &datalist, DataList &originaldatalist_);
	//��ֵ��
	int ChanggeThreshold(int value);
	//���Ե�����
	int pointOperationLinearChange(double k, double b);
	//�����Ե�����
	int pointOperationNonLinear1Change(double c);
	int pointOperationNonLinear2Change(double c);
	//ֱ��ͼ���⻯
	int histogram_equalization(void);
	//��������
	int geometry(int transform,int model,double value);

	//ͼ����ǿ
	int averagen(void);
	int median(void);
	int Kneighbor(void);

	int laplace(void);
	int sobel(void);
	int Isotropic(void);

	int custom(double *temp, int row, int column);

	//��ɫͼ��ҶȻ�
	int toGray();

	//��Ե���
	int laplace2();
	int Kirsch();

	//����
	int huffmanCompress(const QString &caption, const QString &directory);
	int huffmanDecompress(const QString &caption, const QString &directory, const QString &filer);
	int runLengthCompress(const QString &caption, const QString &directory);
	int runLengthDecompress(const QString &caption, const QString &directory, const QString &filer);


private:
	/* init param */
	void initImageResource(void);

	/* open a image */
	int loadImageResource(void);
	int loadImageResource(const QString &caption, const QString &dir, const QString &filer);

	/* get file info list from current path */
	int getFileInfoList(void);
	int getFileCurIndex(void);
	int upgradeFileInfo(QString &filename);

	~ImageShow();
};


#endif // IMAGESHOW_H
