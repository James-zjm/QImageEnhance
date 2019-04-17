#include "getTemplate.h"



getTemplate::getTemplate(QWidget *parent): QWidget(parent)
{
	set();
}

int getTemplate::get_x_spin()
{
	return x_spin->value();
}

int getTemplate::get_y_spin()
{
	return y_spin->value();
}

int getTemplate::get_x_center_spin()
{
	return x_center_spin->value();
}

int getTemplate::get_y_center_spin()
{
	return y_center_spin->value();
}

double * getTemplate::getTable()
{
	double *temp = new double[row*column];
	for(int y=0;y<row;y++)
		for (int x = 0; x < column; x++)
		{
			if (table->item(x, y) == 0)
			{
				temp[y*column + x] = 0;
			}
			else
			{
				QString z = table->item(x, y)->text();
				temp[y*column + x] = z.toDouble();
			}
		}
	return temp;
}

void getTemplate::set()
{
	layout = new QGridLayout(this);

	x_label = new QLabel();
	x_label->setText(QString::fromLocal8Bit("矩阵宽度:"));
	layout->addWidget(x_label,0,0,1,1);
	x_spin = new QSpinBox();
	x_spin->setValue(0);
	x_spin->setMinimum(0);
	x_spin->setSingleStep(1);
	layout->addWidget(x_spin, 0, 1, 1, 1);
	connect(x_spin, SIGNAL(valueChanged(int)), SLOT(x_spinChangged(int)));

	y_label = new QLabel();
	y_label->setText(QString::fromLocal8Bit("矩阵高度:"));
	layout->addWidget(y_label, 0, 2, 1, 1);
	y_spin = new QSpinBox();
	y_spin->setValue(0);
	y_spin->setMinimum(0);
	y_spin->setSingleStep(1);
	layout->addWidget(y_spin, 0, 3, 1, 1);
	connect(y_spin, SIGNAL(valueChanged(int)), SLOT(y_spinChangged(int)));

	x_center_label = new QLabel();
	x_center_label->setText(QString::fromLocal8Bit("矩阵中心x坐标:"));
	layout->addWidget(x_center_label, 1, 0, 1, 1);
	x_center_spin = new QSpinBox();
	x_center_spin->setValue(0);
	x_center_spin->setMinimum(0);
	x_center_spin->setMaximum(0);
	x_center_spin->setSingleStep(1);
	layout->addWidget(x_center_spin, 1, 1, 1, 1);

	y_center_label = new QLabel();
	y_center_label->setText(QString::fromLocal8Bit("矩阵中心y坐标:"));
	layout->addWidget(y_center_label, 1, 2, 1, 1);
	y_center_spin = new QSpinBox();
	y_center_spin->setValue(0);
	y_center_spin->setMinimum(0);
	y_center_spin->setMaximum(0);
	y_center_spin->setSingleStep(1);
	layout->addWidget(y_center_spin, 1, 3, 1, 1);

	table = new QTableWidget();
	table->setColumnCount(0);
	table->setRowCount(0);
	layout->addWidget(table, 2, 0, 4, 4);

	ok_button = new QPushButton();
	ok_button->setText(QString::fromLocal8Bit("确认"));
	layout->addWidget(ok_button, 6, 3, 1, 1);
	connect(ok_button, SIGNAL(clicked(void)), this, SLOT(ok_clicked(void)));
	this->setLayout(layout);

	row = 0; 
	column = 0;
}


getTemplate::~getTemplate()
{
}

void getTemplate::y_spinChangged(int a)
{
	row = a;
	table->setRowCount(a);
	table->clearContents();
	data.clear();
	data.resize(row);
	for (int i = 0; i < row; i++)
	{
		data[i].resize(column);
	}
	//y_header.clear();
	//for (int i = 0; i < row; i++)
	//	y_header << QString::fromStdString(std::to_string(i));
	//table->setHorizontalHeaderLabels(y_header);
	y_center_spin->setMinimum(0);
	y_center_spin->setMaximum(row );
}

void getTemplate::ok_clicked(void)
{
	hide();
	emit matrixOk();
}

void getTemplate::x_spinChangged(int a)
{
	column = a;
	table->setColumnCount(a);
	table->clearContents();
	data.clear();
	data.resize(row);
	for (int i = 0; i < row; i++)
	{
		data[i].resize(column);
	}
	//x_header.clear();
	//for (int i = 0; i < column; i++)
	//	x_header << QString::fromStdString(std::to_string(i));
	//table->setVerticalHeaderLabels(x_header);
	x_center_spin->setMinimum(0 );
	x_center_spin->setMaximum(column );
}