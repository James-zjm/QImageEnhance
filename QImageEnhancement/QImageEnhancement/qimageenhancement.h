#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QLabel>
#include <QFileInfoList>
#include <QMessageBox>
#include <QScrollArea>
#include <QDockWidget>
#include <QGridLayout>
#include <QRadioButton>
#include <QButtonGroup>
#include <QPainter>
#include <QString>
#include <QSpinBox>
#include <QSlider>
#include <QComboBox>

#include <QtCharts/QChartGlobal>
#include <QtCharts/QChartView>
#include <QtCharts/QLegend>
#include <QtCharts/QLineSeries>
#include <QtCharts/QAreaSeries>


#include "image.h"
#include "imageshow.h"
#include "getTemplate.h"

QT_CHARTS_BEGIN_NAMESPACE
class QChartView;
class QChart;
QT_CHARTS_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

typedef QList<QPointF> DataList;


///=================================================================================================
/// <summary>	The application's main form. </summary>
///
/// <remarks>	John Zjm, 2018/10/7. </remarks>
///=================================================================================================

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
private:
	/*默认界面*/
	QToolBar *toolBar;
	QWidget *centralWidget;
	QStatusBar *statusBar;
	QMenuBar *menuBar;
	QLabel *imageLabel;
	/* new class */
	ImageShow *imageViewer;
	/*记录所有docks*/
	QList<QDockWidget*> magic_docks;
	//直方图表
	DataList data_list_;
	DataList original_data_list_;
	QChartView *histogram_;
	QChart *chart;
	QLineSeries *series;
	QLineSeries *original_series_;
	QAreaSeries *area;

	void initMainWindow(void);
	void initUiComponent(void);
	void setQImageViewerWidget(void);
	void setWindowComponet(void);
	void initImageResource(void);
	void loadImageResource(void);
	void getHistogram(void);
	void updateHistogram(void);

	QAction *open_action_;
	QAction *close_action_;
	QAction *last_action_;
	QAction *next_action_;
	QAction *rotate_left_action_;
	QAction *rotate_right_action_;
	QAction *enlarge_action_;
	QAction *lessen_action_;
	QAction *delete_action_;
	QAction *save_action_;
	QAction *magic_action_;

	/*8位灰度图转化为8幅位平面表示的二值图*/
	QAction *eight2two;
	QDockWidget *eight_to_binary_map_;
	QButtonGroup *eight_to_binary_map_eight_button_group_;

	//灰度直方图显示
	QAction *grayShow;
	QDockWidget *gray_show_dock_;
	QLabel *average_gray_;
	QLabel *average_gray_num_;
	QLabel *mid_gray_;
	QLabel *mid_gray_num_;
	QLabel *standard_deviation_gray_;
	QLabel *standard_deviation_gray_num_;
	QLabel *pixel_sum_;
	QLabel *pixel_sum_num_;
	//阈值化
	QAction *Thresholding;
	QDockWidget *Thresholding_dock_;
	QSpinBox *Thresholding_spinBox;
	QSlider *Thresholding_slider;

	//点运算
	QAction *PointOperation;
	QDockWidget *PointOperation_dock_;
	QButtonGroup *point_operation_group_;
	QGridLayout *point_operation_layout;
	QLabel *linear_funcation_;
	QLabel *linear_funcation_k_;
	QDoubleSpinBox *linear_funcation_k_line;
	QLabel *linear_funcation_b_;
	QDoubleSpinBox *linear_funcation_b_line_;
	QLabel *nonlinear_function_1_;
	QLabel *nonlinear_function_1_c_;
	QDoubleSpinBox *nonlinear_funcation_1_c_line_;
	QLabel *nonlinear_function_2_;
	QLabel *nonlinear_function_2_c_;
	QDoubleSpinBox *nonlinear_funcation_2_c_line_;

	//直方图均衡化
	QAction *HistogramEqualization;

	//几何运算
	QAction *GeometricOperation;
	QDockWidget *GeometricOperation_dock_;
	QComboBox *transform_combo_;
	QComboBox *interpolation_algorithm_combo_;
	QDoubleSpinBox *angle_multiple_line_;

	//图像增强
	QAction *EnhanceOperation;
	QMenu *enhance_menu_;
	QAction *smoothOperation;
	QAction *sharpenOperation;
	QAction *customOperation;
	QMenu *smooth_menu_;
	QAction *averageOperation;
	QAction *medianOperation;
	QAction *KneighborOperation;
	QMenu *sharpen_menu_;
	QAction *sobelOperation;
	QAction *laplaceOperation;
	QAction *IsotropicOperation;
	getTemplate *getmatrix;

	//彩色图像灰度化处理
	QAction *toGrayOperation;

	//边缘检测
	QAction *EdgeDetectionOperation;
	QMenu *edge_detection_menu;
	QAction *laplace2Operation;
	QAction *KirschOperation;

	//图像编码
	QAction *ImageCodeOperation;
	QMenu *image_code_menu;
	QAction *Huffmanperation;
	QAction *RunLengthOperation;
	QMenu *huffman_menu;
	QAction *huffmanCompressOperation;
	QAction *huffmanDecompressOperation;
	QMenu *run_length_menu;
	QAction *runLengthCompressOperation;
	QAction *runLengthDecompressOperation;
	

public slots:
	void openActionTriggered(void);
	void closeActionTriggered(void);
	void lastActionTriggered(void);
	void nextActionTriggered(void);
	void toLeftActionTriggered(void);
	void toRightActionTriggered(void);
	void toEnlargeActionTriggered(void);
	void toLessenActionTriggered(void);
	void deleteActionTriggered(void);
	void saveActionTriggered(void);

	void eight2twoTriggered(void);
	void on8to2ButtonClicked(int button_id);

	void gray_show_Triggered(void);
	void ThresholdingTriggered(void);
	void ThresholdingChangged(int value);

	void point_opeartion_Triggered(void);
	void point_opration_Clicked(int button_id);
	void linear_funcationChangged(double value);
	void nonlinear_funcation1Changged(double value);
	void nonlinear_funcation2Changged(double value);

	void histogram_equalization_Triggered(void);

	void geometric_operation_triggered(void);
	void angle_multipleChangged(double value);
	void interpolation_algorithm_combo_changed(int index);

	void averageOperation_triggered(void);
	void medianOperation_triggered(void);
	void KneighborOperation_triggered(void);

	void laplaceOperation_triggered(void);
	void sobelOperationn_triggered(void);
	void IsotropicOperatio_triggered(void);

	void customOperation_triggered(void);
	void customBegin(void);

	void toGrayOperation_triggered(void);

	void laplace2Operation_triggered(void);
	void KirschOperation_triggered(void);

	void huffmanCompressOperation_triggered(void);
	void huffmanDecompressOperation_triggered(void);

	void runLengthCompressOperation_triggered(void);
	void runLengthDecompressOperation_triggered(void);
};


#endif // MAINWINDOW_H
