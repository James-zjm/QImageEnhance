#pragma once
#include <QWidget>
#include <QTableWidget>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <vector>
class getTemplate:public QWidget
{
	Q_OBJECT
public:
	getTemplate(QWidget * parent=0);
	int get_x_spin();
	int get_y_spin();
	int get_x_center_spin();
	int get_y_center_spin();
	double *getTable();

	void set();
	~getTemplate();
private:
	QLabel *x_label;
	QSpinBox *x_spin;
	QLabel *y_label;
	QSpinBox *y_spin;
	QGridLayout *layout;
	QLabel *x_center_label;
	QSpinBox *x_center_spin;
	QLabel *y_center_label;
	QSpinBox *y_center_spin;
	QPushButton *ok_button;
	QTableWidget *table;

	std::vector<std::vector<double>> data;
	//QStringList x_header;
	//QStringList y_header;
	int row, column;
public slots:
	void x_spinChangged(int);
	void y_spinChangged(int);
	void ok_clicked(void);

signals:
	void matrixOk(void);
};

