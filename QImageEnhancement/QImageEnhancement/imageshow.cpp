#include "imageshow.h"

ImageShow::ImageShow(QWidget *parent) : QWidget(parent)
{
	this->parent = parent;
	initImageResource();
}

ImageShow::ImageShow(QWidget *parent,
	QString &caption,
	QString &dir,
	QString &filer)
{
	this->parent = parent;
	initImageResource();
	loadImageResource(caption, dir, filer);
}

ImageShow::~ImageShow(void)
{
	this->parent = NULL;
}

int ImageShow::openImageFile(const QString &caption,
	const QString &dir,
	const QString &filer)
{
	initImageResource();
	return loadImageResource(caption, dir, filer);
}

int ImageShow::closeImageFile(void)
{
	initImageResource();
	return 0;
}

int ImageShow::delImageFile(void)
{
	if (original_image_->filename.isEmpty())
	{
		return -1;
	}

	if (QFile::remove(original_image_->filename))
	{
		qDebug() << "remove success: " << original_image_->filename;
	}
	else
	{
		qDebug() << "remove failed: " << original_image_->filename;
		return -1;
	}

	/* delete from list */
	fileInfoList.removeAt(index);

	return 0;
}

int ImageShow::last(void)
{
	if (index < 0)
	{
		return -1;
	}

	while (1)
	{
		index = index - 1;
		int count = fileInfoList.count();
		if (index < 0)
		{
			QMessageBox::information(this, tr("Tip"), tr("This is the first image."));
			index = count - 1;
		}

		original_image_->filename.clear();
		original_image_->filename.append(path);
		original_image_->filename += "/";
		original_image_->filename += fileInfoList.at(index).fileName();

		if (!QFile(original_image_->filename).exists())
		{
			fileInfoList.removeAt(index);
			continue;
		}
		else
		{
			break;
		}
	}

	angle = 0;
	scale = 1;
	moveX = 0;
	moveY = 0;
	size = QSize(0, 0);
	/* load file info */
	return upgradeFileInfo(original_image_->filename);
}

int ImageShow::next(void)
{
	if (index < 0)
	{
		return -1;
	}

	while (1)
	{
		index = index + 1;
		int count = fileInfoList.count();
		if (index == count)
		{
			QMessageBox::information(this, tr("Tip"), tr("This is the last image."));
			index = 0;
		}

		original_image_->filename.clear();
		original_image_->filename.append(path);
		original_image_->filename += "/";
		original_image_->filename += fileInfoList.at(index).fileName();

		if (!QFile(original_image_->filename).exists())
		{
			fileInfoList.removeAt(index);
			continue;
		}
		else
		{
			break;
		}
	}

	angle = 0;
	scale = 1;
	moveX = 0;
	moveY = 0;
	size = QSize(0, 0);
	/* load file info */
	return upgradeFileInfo(original_image_->filename);
}

int ImageShow::zoomIn(void)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	scale += 0.2;
	ChangeSize a(*original_image_);
	a.changeSize_nearest_neighbor_interpolation(scale, original_image_->picture_size_);
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

int ImageShow::zoomOut(void)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	if (scale != 2)
		scale -= 0.2;
	ChangeSize a(*original_image_);
	a.changeSize_nearest_neighbor_interpolation(scale, original_image_->picture_size_);
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

int ImageShow::spinToRight(void)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	angle += 90;
	Rotate a(*original_image_);
	a.spin_nearest_neighbor_interpolation(angle, size);
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

int ImageShow::spinToLeft(void)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	angle += 270;
	Rotate a(*original_image_);
	a.spin_nearest_neighbor_interpolation(angle, size);
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

void ImageShow::initImageResource(void)
{
	index = -1;
	angle = 0;
	scale = 1;
	moveX = 0;
	moveY = 0;
	original_image_ = new Image;
	original_image_->picture_size_ = QSize(0, 0);
	size = QSize(0, 0);
	show_image_ = new Image;
	*show_image_ = *original_image_;
	path.clear();
}

int ImageShow::loadImageResource(void)
{
	//qDebug() << QImageReader::supportedImageFormats();
	original_image_->filename = QFileDialog::getOpenFileName(this, tr("Select image:"),
		"C:\\", tr("Images (*.jpg *.jpeg *.png *.bmp *.gif)"));
	if (original_image_->filename.isEmpty()) {
		return -1;
	}

	/* get file list */
	getFileInfoList();
	/* load file info */
	upgradeFileInfo(original_image_->filename);

	return 0;
}

int ImageShow::loadImageResource(const QString &caption, const QString &directory, const QString &filer)
{
	original_image_->filename = QFileDialog::getOpenFileName(this, caption, directory, filer);

	if (original_image_->filename.isEmpty()) {
		return -1;
	}

	/* get file list */
	getFileInfoList();

	/* load file info */
	upgradeFileInfo(original_image_->filename);

	return 0;
}

int ImageShow::upgradeFileInfo(QString &filename)
{

	if (filename.isEmpty())
	{
		return -1;
	}

	fileInfo = QFileInfo(filename);
	if (!original_image_->image.load(filename))
	{
		return -1;
	}
	if (size == QSize(0, 0))
	{
		size = original_image_->image.size();
	}
	if (original_image_->picture_size_ == QSize(0, 0))
	{
		original_image_->picture_size_ = original_image_->image.size();
	}
	*show_image_ = *original_image_;
	/* upgrade pixmap */
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	/* upgrade index */
	index = getFileCurIndex();

	return 0;
}

int ImageShow::getFileInfoList(void)
{
	QFileInfo info;
	QFileInfoList infoList;

	path = QFileInfo(original_image_->filename).absolutePath();
	dir = QFileInfo(original_image_->filename).absoluteDir();

	/* clear list */
	fileInfoList.clear();

	infoList = dir.entryInfoList(QDir::Files);
	for (int i = 0; i < infoList.count(); i++)
	{
		info = infoList.at(i);
		QString suffix = info.suffix();

		if (suffix == "jpg" || suffix == "bmp" || suffix == "png"
			|| suffix == "gif" || suffix == "jpeg")
		{
			fileInfoList.append(info);
		}
	}

	return 0;
}

int ImageShow::getFileCurIndex(void)
{
	QFileInfo info;
	int j;

	if (fileInfoList.count() <= 0)
	{
		qDebug() << "fileInfoList is NULL!";
		return -1;
	}

	for (j = 0; j < fileInfoList.count(); j++)
	{
		info = fileInfoList.at(j);
		if (info.fileName() == fileInfo.fileName())
		{
			break;
		}
	}

	if (j >= fileInfoList.count())
	{
		qDebug() << "Not find current file!";
		return -1;
	}

	index = j;
	//qDebug() << "Current fileInfo index: " << index;

	return index;
}

int ImageShow::saveImageFile(const QString &caption, const QString &dir)
{
	if (show_image_->filename.isEmpty())
		return -1;
	QString folderpath = QFileDialog::getExistingDirectory(this, caption, dir);
	if (folderpath.isEmpty())
		return -1;
	folderpath += "/";
	folderpath += QFileInfo(show_image_->filename).fileName();
	show_image_->image.save(folderpath, "JPEG", 100);
	return 0;
}

///=================================================================================================
/// <summary>	8位灰度图转化为8幅位平面表示的二值图 </summary>
///
/// <remarks>	John Zjm, 2018/9/26. </remarks>
///
/// <param name="order">	第几位平面 </param>
///
/// <returns>	An int. </returns>
///=================================================================================================

int ImageShow::showBinary(int order)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	GrayDecompose a(*original_image_);
	a.turnBinaryImage(order);
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

int ImageShow::grayStatistics(double &average, double &mid, double &standard, double &pixel, DataList &datalist, DataList &originaldatalist)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	datalist.clear();
	originaldatalist.clear();
	int sum = 0, gray_sum[257] = { 0 }, original_gray_sum[257] = { 0 };
	long long aa = 0, bb = 0;

	for (int x = 0; x < original_image_->image.width(); x++)
	{
		for (int y = 0; y < original_image_->image.height(); y++)
		{
			QColor oldColor(original_image_->image.pixel(x, y));
			unsigned int gray = (oldColor.red() * 299 + oldColor.green() * 587 + oldColor.blue() * 114) / 1000;
			original_gray_sum[gray] ++;
		}
	}
	for (int i = 0; i <= 256; i++)
	{
		QPointF point(i, original_gray_sum[i]);
		originaldatalist << point;
	}

	for (int x = 0; x < show_image_->image.width(); x++)
	{
		for (int y = 0; y < show_image_->image.height(); y++)
		{
			QColor oldColor(show_image_->image.pixel(x, y));
			unsigned int gray = (oldColor.red() * 299 + oldColor.green() * 587 + oldColor.blue() * 114) / 1000;
			gray_sum[gray] ++;
			sum += gray;
		}
	}
	pixel = show_image_->image.width()*show_image_->image.height();
	average = sum / pixel;
	for (int i = 0; i <= 256; i++)
	{
		QPointF point(i, gray_sum[i]);
		datalist << point;
		//originaldatalist << point;
		while (gray_sum[i]--)
		{
			if (gray_sum[i] <= 0)
			{
				break;
			}
			aa += (i - average)*(i - average);
			bb++;
			if (bb == pixel / 2)
			{
				mid = i;
			}
		}
	}
	aa = aa / (pixel - 1);
	standard = sqrt(aa);
	return 0;
}

int ImageShow::ChanggeThreshold(int value)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	Threshold a(*original_image_);
	a.Thresholdchanging(value);

	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

int ImageShow::pointOperationLinearChange(double k, double b)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	PointOperation a(*original_image_);
	a.linearFuncation(k, b);
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}
int ImageShow::pointOperationNonLinear1Change(double c)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	PointOperation a(*original_image_);
	a.nonLinearFuncation1(c);
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}
int ImageShow::pointOperationNonLinear2Change(double c)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	PointOperation a(*original_image_);
	a.nonLinearFuncation2(c);
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}
int ImageShow::histogram_equalization(void)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	HistogramEqualization a(*original_image_);
	a.HistogramEqualizationchanging();
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

int ImageShow::geometry(int transform, int model, double value)
{

	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	if (transform == 0)
	{
		angle = value;
		Rotate a(*original_image_);
		if (model == 0)
		{
			a.spin_nearest_neighbor_interpolation(angle, size);
		}
		if (model == 1)
		{
			a.spin_Bilinear_interpolation(angle, size);
		}
		*show_image_ = a;
	}
	if (transform == 1)
	{
		scale = value;
		ChangeSize a(*original_image_);
		if (model == 0)
		{
			a.changeSize_nearest_neighbor_interpolation(scale, size);
		}
		if (model == 1)
		{
			a.changeSize_Bilinear_interpolation(scale, size);
		}
		*show_image_ = a;
	}
	if (transform == 2)
	{
		if (model == 0)
		{
			moveX = value;
		}
		if (model == 1)
		{
			moveY = value;
		}
		Translation a(*original_image_);
		a.translation(moveX, moveY, size);
		*show_image_ = a;
	}
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

int ImageShow::averagen(void)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	Smooth a(*original_image_);
	a.average();
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

int ImageShow::median(void)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	Smooth a(*original_image_);
	a.median();
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

int ImageShow::Kneighbor(void)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	Smooth a(*original_image_);
	a.Kneighbor();
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

int ImageShow::laplace(void)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	Sharpen a(*original_image_);
	a.laplace();
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

int ImageShow::sobel(void)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	Sharpen a(*original_image_);
	a.sobel();
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

int ImageShow::Isotropic(void)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	Sharpen a(*original_image_);
	a.Isotropic();
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

int ImageShow::custom(double * temp, int row, int column)
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	Custom a(*original_image_);
	a.begin(temp,row,column);
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

int ImageShow::toGray()
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	ToGray a(*original_image_);
	a.to();
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

int ImageShow::laplace2()
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	EdgeDetection a(*original_image_);
	a.laplace2();
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

int ImageShow::Kirsch()
{
	if (show_image_->filename.isEmpty())
	{
		return -1;
	}
	EdgeDetection a(*original_image_);
	a.Kirschperation();
	*show_image_ = a;
	pixmap = QPixmap::fromImage(show_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();
	return 0;
}

int ImageShow::huffmanCompress(const QString & caption, const QString & directory)
{
	QString filename = QFileDialog::getExistingDirectory(this, caption, directory);

	if (filename.isEmpty()) 
	{
		return -1;
	}
	bool flag = 0;
	std::string out_filename = filename.toStdString();
	out_filename += '/';
	out_filename += fileInfoList.at(index).fileName().toStdString();
	int i;
	for (i = out_filename.length()-1; i >= 0; i--)
		if (out_filename[i] == '.')
			break;
	out_filename[i + 1] = 't';
	out_filename[i + 2] = 'x';
	out_filename[i + 3] = 't';
	for (int i = 0; i < out_filename.length(); i++)
	{
		if (out_filename[i] == '/')
			out_filename[i] = '\\';
	}
	Huffman h("", out_filename.c_str());
	h.compress(original_image_->image);
	return 0;
}

int ImageShow::huffmanDecompress(const QString & caption, const QString & directory, const QString & filer)
{
	QString filename = QFileDialog::getOpenFileName(this, caption, directory, filer);

	if (filename.isEmpty()) 
	{
		return -1;
	}
	std::string in_filename = filename.toStdString();
	for (int i = 0; i < in_filename.length(); i++)
	{
		if (in_filename[i] == '/')
			in_filename[i] = '\\';
	}
	Huffman h(in_filename.c_str(), "");
	original_image_->image = h.decompress();

	//加载图片
	size = original_image_->image.size();
	original_image_->picture_size_ = original_image_->image.size();
	original_image_->filename = filename;
	*show_image_ = *original_image_;
	/* upgrade pixmap */
	pixmap = QPixmap::fromImage(original_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();

	return 0;
}

int ImageShow::runLengthCompress(const QString & caption, const QString & directory)
{
	QString filename = QFileDialog::getExistingDirectory(this, caption, directory);

	if (filename.isEmpty())
	{
		return -1;
	}
	bool flag = 0;
	std::string out_filename = filename.toStdString();
	out_filename += '/';
	out_filename += fileInfoList.at(index).fileName().toStdString();
	int i;
	for (i = out_filename.length() - 1; i >= 0; i--)
		if (out_filename[i] == '.')
			break;
	out_filename[i + 1] = 't';
	out_filename[i + 2] = 'x';
	out_filename[i + 3] = 't';
	for (int i = 0; i < out_filename.length(); i++)
	{
		if (out_filename[i] == '/')
			out_filename[i] = '\\';
	}
	RunLength h("", out_filename.c_str());
	h.compress(original_image_->image);
	return 0;
}

int ImageShow::runLengthDecompress(const QString & caption, const QString & directory, const QString & filer)
{
	QString filename = QFileDialog::getOpenFileName(this, caption, directory, filer);

	if (filename.isEmpty())
	{
		return -1;
	}
	std::string in_filename = filename.toStdString();
	for (int i = 0; i < in_filename.length(); i++)
	{
		if (in_filename[i] == '/')
			in_filename[i] = '\\';
	}
	RunLength h(in_filename.c_str(), "");
	original_image_->image = h.decompress();

	//加载图片
	size = original_image_->image.size();
	original_image_->picture_size_ = original_image_->image.size();
	original_image_->filename = filename;
	*show_image_ = *original_image_;
	/* upgrade pixmap */
	pixmap = QPixmap::fromImage(original_image_->image);
	size = pixmap.size();
	index = getFileCurIndex();

	return 0;
}

