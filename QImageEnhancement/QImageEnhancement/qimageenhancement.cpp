#include "qimageenhancement.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	/* init main window */
	initMainWindow();

	/* init ui */
	initUiComponent();

	/* init resource */
	initImageResource();

	/* create imageViewer */
	imageViewer = new ImageShow();
}

void MainWindow::initMainWindow(void)
{
	menuBar = new QMenuBar(this);
	setMenuBar(menuBar);

	toolBar = new QToolBar(this);
	addToolBar(toolBar);

	centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	statusBar = new QStatusBar(this);
	setStatusBar(statusBar);

	chart = new QChart();
	chart->setTitle(QString::fromLocal8Bit("灰度直方图"));
	series = new QLineSeries();
	series->setName(QString::fromLocal8Bit("Now Gray"));
	original_series_ = new QLineSeries();
	original_series_->setName(QString::fromLocal8Bit("Before Gray"));
	area = nullptr;

}

void MainWindow::initUiComponent(void)
{
	/* set menu and toolbar */
	setWindowComponet();

	/* image show widget */
	setQImageViewerWidget();
}

void MainWindow::initImageResource(void)
{
	imageLabel->clear();
	imageLabel->resize(QSize(200, 100));
	setWindowTitle(tr("QImageViewer"));
}

void MainWindow::setWindowComponet(void)
{
	histogram_ = new QChartView(chart);
	histogram_->setRenderHint(QPainter::Antialiasing);
	histogram_->setMinimumSize(QSize(500, 500));
	histogram_->setMaximumHeight(500);
	histogram_->chart()->setTheme(QChart::ChartThemeBlueCerulean);
	histogram_->chart()->setAnimationOptions(QChart::AllAnimations);
	histogram_->chart()->legend()->setAlignment(Qt::AlignBottom);

	open_action_ = new QAction(tr("Open"), this);
	open_action_->setShortcut(QKeySequence::Open);
	open_action_->setToolTip(tr("Open a image."));
	open_action_->setIcon(QIcon(":/QImageEnhancement/images/open.png"));

	close_action_ = new QAction(tr("Close"), this);
	close_action_->setShortcut(QKeySequence::Close);
	close_action_->setToolTip(tr("Close a image."));
	close_action_->setIcon(QIcon(":/QImageEnhancement/images/close.png"));

	last_action_ = new QAction(tr("Last"), this);
	last_action_->setToolTip(tr("Last image."));
	last_action_->setIcon(QIcon(":/QImageEnhancement/images/last.png"));

	next_action_ = new QAction(tr("Next"), this);
	next_action_->setToolTip(tr("Next image"));
	next_action_->setIcon(QIcon(":/QImageEnhancement/images/next.png"));

	rotate_left_action_ = new QAction(tr("LeftSpin"), this);
	rotate_left_action_->setToolTip(tr("To Left."));
	rotate_left_action_->setIcon(QIcon(":/QImageEnhancement/images/rotateleft.png"));

	rotate_right_action_ = new QAction(tr("RightSpin"), this);
	rotate_right_action_->setToolTip(tr("To Right."));
	rotate_right_action_->setIcon(QIcon(":/QImageEnhancement/images/rotateright.png"));

	enlarge_action_ = new QAction(tr("Enlarge"), this);
	enlarge_action_->setToolTip(tr("To Enlarge."));
	enlarge_action_->setIcon(QIcon(":/QImageEnhancement/images/enlarge.png"));

	lessen_action_ = new QAction(tr("Lessen"), this);
	lessen_action_->setToolTip(tr("To Lessen."));
	lessen_action_->setIcon(QIcon(":/QImageEnhancement/images/lessen.png"));

	delete_action_ = new QAction(tr("Delete"), this);
	delete_action_->setToolTip(tr("Delete a image"));
	delete_action_->setIcon(QIcon(":/QImageEnhancement/images/delete.png"));
	delete_action_->setShortcut(QKeySequence::Delete);

	save_action_ = new QAction(tr("Save"), this);
	close_action_->setShortcut(QKeySequence::Save);
	save_action_->setToolTip(tr("save a image"));
	save_action_->setIcon(QIcon(":/QImageEnhancement/images/save.png"));

	magic_action_ = new QAction(tr("Magic"), this);
	magic_action_->setToolTip(tr("have fun"));
	magic_action_->setIcon(QIcon(":/QImageEnhancement/images/magic.png"));

	QMenu *magic_menu = new QMenu;

	///=================================================================================================
	eight2two = new QAction(QString::fromLocal8Bit("分解灰度图"), this);
	//设置doc widget组
	eight_to_binary_map_ = new QDockWidget(QString::fromLocal8Bit("分解灰度图"), this);//构建停靠窗口，指定父类

	eight_to_binary_map_->setFeatures(QDockWidget::AllDockWidgetFeatures);//设置停靠窗口特性，具有全部停靠窗口的特性
	QWidget * eight_to_binary_widget = new QWidget(eight_to_binary_map_);
	// 设置按钮组
	QVBoxLayout *button_layout = new QVBoxLayout();
	eight_to_binary_map_eight_button_group_ = new QButtonGroup(eight_to_binary_map_);

	// 设置互斥
	eight_to_binary_map_eight_button_group_->setExclusive(true);
	for (int i = 0; i < 8; ++i)
	{
		QRadioButton *pButton = new QRadioButton(eight_to_binary_map_);

		// 设置文本
		pButton->setText(QString::fromLocal8Bit("%1 Binary image").arg(i + 1));

		button_layout->addWidget(pButton);
		eight_to_binary_map_eight_button_group_->addButton(pButton, i);
	}
	button_layout->setSpacing(10);
	button_layout->setContentsMargins(10, 10, 10, 10);

	eight_to_binary_widget->setLayout(button_layout);
	eight_to_binary_map_->setWidget(eight_to_binary_widget);
	removeDockWidget(eight_to_binary_map_);
	magic_docks.append(eight_to_binary_map_);
	magic_menu->addAction(eight2two);
	magic_menu->addSeparator();
	///=================================================================================================
	grayShow = new QAction(QString::fromLocal8Bit("直方图显示"), this);
	gray_show_dock_ = new QDockWidget(QString::fromLocal8Bit("直方图显示"), this);
	gray_show_dock_->setFeatures(QDockWidget::AllDockWidgetFeatures);

	QWidget * gray_show_widget = new QWidget(gray_show_dock_);
	QGridLayout *gray_show_layout = new QGridLayout();

	gray_show_layout->addWidget(histogram_, 0, 0, 1, 5);

	average_gray_ = new QLabel(gray_show_dock_);
	average_gray_->setText(QString::fromLocal8Bit("平均灰度："));
	gray_show_layout->addWidget(average_gray_, 1, 0);

	average_gray_num_ = new QLabel(gray_show_dock_);
	average_gray_num_->setText(QString::fromLocal8Bit(""));
	gray_show_layout->addWidget(average_gray_num_, 1, 1);

	mid_gray_ = new QLabel(gray_show_dock_);
	mid_gray_->setText(QString::fromLocal8Bit("中值灰度："));
	gray_show_layout->addWidget(mid_gray_, 1, 2);

	mid_gray_num_ = new QLabel(gray_show_dock_);
	mid_gray_num_->setText(QString::fromLocal8Bit(""));
	gray_show_layout->addWidget(mid_gray_num_, 1, 3);

	standard_deviation_gray_ = new QLabel(gray_show_dock_);
	standard_deviation_gray_->setText(QString::fromLocal8Bit("标准差："));
	gray_show_layout->addWidget(standard_deviation_gray_, 2, 0);

	standard_deviation_gray_num_ = new QLabel(gray_show_dock_);
	standard_deviation_gray_num_->setText(QString::fromLocal8Bit(""));
	gray_show_layout->addWidget(standard_deviation_gray_num_, 2, 1);

	pixel_sum_ = new QLabel(gray_show_dock_);
	pixel_sum_->setText(QString::fromLocal8Bit("像素总数："));
	gray_show_layout->addWidget(pixel_sum_, 2, 2);

	pixel_sum_num_ = new QLabel(gray_show_dock_);
	pixel_sum_num_->setText(QString::fromLocal8Bit(""));
	gray_show_layout->addWidget(pixel_sum_num_, 2, 3);

	gray_show_widget->setLayout(gray_show_layout);
	gray_show_dock_->setWidget(gray_show_widget);
	removeDockWidget(gray_show_dock_);

	magic_docks.append(gray_show_dock_);
	magic_menu->addAction(grayShow);

	///=================================================================================================
	Thresholding = new QAction(QString::fromLocal8Bit("二值化"), this);
	Thresholding_dock_ = new QDockWidget(QString::fromLocal8Bit("二值化"), this);
	QWidget * Thresholding_widget = new QWidget(gray_show_dock_);
	QHBoxLayout *Thresholding_layout = new QHBoxLayout();

	Thresholding_spinBox = new QSpinBox(gray_show_dock_);
	Thresholding_spinBox->setMinimum(0);  // 最小值
	Thresholding_spinBox->setMaximum(256);  // 最大值
	Thresholding_spinBox->setSingleStep(1);  // 步长
	Thresholding_layout->addWidget(Thresholding_spinBox);

	QSlider *Thresholding_slider = new QSlider(gray_show_dock_);
	Thresholding_slider->setOrientation(Qt::Horizontal);  // 水平方向
	Thresholding_slider->setMinimum(0);  // 最小值
	Thresholding_slider->setMaximum(256);  // 最大值
	Thresholding_slider->setSingleStep(1);  // 步长
	Thresholding_layout->addWidget(Thresholding_slider);

	Thresholding_widget->setLayout(Thresholding_layout);
	Thresholding_dock_->setWidget(Thresholding_widget);
	removeDockWidget(Thresholding_dock_);

	magic_docks.append(Thresholding_dock_);
	magic_menu->addAction(Thresholding);
	magic_menu->addSeparator();
	///=================================================================================================
	PointOperation = new QAction(QString::fromLocal8Bit("点运算"), this);
	PointOperation_dock_ = new QDockWidget(QString::fromLocal8Bit("点运算"), this);
	point_operation_group_ = new QButtonGroup(PointOperation_dock_);
	QWidget * point_operation_widget = new QWidget(PointOperation_dock_);
	point_operation_layout = new QGridLayout();
	//point_operation_layout->addWidget(histogram_,0, 0, 1, 5);


	QRadioButton *point_operation_linner_button = new QRadioButton(eight_to_binary_map_);
	point_operation_linner_button->setText(QString::fromLocal8Bit("线性点运算"));
	point_operation_layout->addWidget(point_operation_linner_button, 1, 0, 1, 2);
	point_operation_group_->addButton(point_operation_linner_button, 0);
	linear_funcation_ = new QLabel(PointOperation_dock_);
	QPixmap px;
	px.load(QString(":/QImageEnhancement/images/linear.png"));
	linear_funcation_->setPixmap(px);
	linear_funcation_->setAlignment(Qt::AlignCenter);
	linear_funcation_->hide();

	linear_funcation_k_ = new QLabel(PointOperation_dock_);
	linear_funcation_k_->setText(QString::fromLocal8Bit("a:"));
	linear_funcation_k_->hide();
	linear_funcation_k_line = new QDoubleSpinBox(gray_show_dock_);
	linear_funcation_k_line->setMinimum(-256);  // 最小值
	linear_funcation_k_line->setMaximum(256);  // 最大值
	linear_funcation_k_line->setSingleStep(0.5);
	linear_funcation_k_line->hide();

	linear_funcation_b_ = new QLabel(PointOperation_dock_);
	linear_funcation_b_->setText(QString::fromLocal8Bit("b:"));
	linear_funcation_b_->hide();
	linear_funcation_b_line_ = new QDoubleSpinBox(gray_show_dock_);
	linear_funcation_b_line_->setMinimum(-256);  // 最小值
	linear_funcation_b_line_->setMaximum(256);  // 最大值
	linear_funcation_b_line_->setSingleStep(0.5);
	linear_funcation_b_line_->hide();

	QRadioButton *point_operation_nonlinear_1_button = new QRadioButton(eight_to_binary_map_);
	point_operation_nonlinear_1_button->setText(QString::fromLocal8Bit("第一种非线性点运算"));
	point_operation_layout->addWidget(point_operation_nonlinear_1_button, 1, 2, 1, 2);
	point_operation_group_->addButton(point_operation_nonlinear_1_button, 1);

	nonlinear_function_1_ = new QLabel(PointOperation_dock_);
	QPixmap py;
	py.load(QString(":/QImageEnhancement/images/nonlinear1.png"));
	nonlinear_function_1_->setPixmap(py);
	nonlinear_function_1_->setAlignment(Qt::AlignCenter);
	nonlinear_function_1_->hide();

	nonlinear_function_1_c_ = new QLabel(PointOperation_dock_);
	nonlinear_function_1_c_->setText(QString::fromLocal8Bit("C:"));
	nonlinear_function_1_c_->hide();
	nonlinear_funcation_1_c_line_ = new QDoubleSpinBox(PointOperation_dock_);
	nonlinear_funcation_1_c_line_->setMinimum(-256);  // 最小值
	nonlinear_funcation_1_c_line_->setMaximum(256);  // 最大值
	nonlinear_funcation_1_c_line_->setDecimals(10);
	nonlinear_funcation_1_c_line_->setSingleStep(0.001);
	nonlinear_funcation_1_c_line_->hide();

	QRadioButton *point_operation_nonlinear_2_button = new QRadioButton(eight_to_binary_map_);
	point_operation_nonlinear_2_button->setText(QString::fromLocal8Bit("第二种非线性点运算"));
	point_operation_layout->addWidget(point_operation_nonlinear_2_button, 1, 4);
	point_operation_group_->addButton(point_operation_nonlinear_2_button, 2);

	nonlinear_function_2_ = new QLabel(PointOperation_dock_);
	QPixmap pz;
	pz.load(QString(":/QImageEnhancement/images/nonlinear2.png"));
	nonlinear_function_2_->setPixmap(pz);
	nonlinear_function_2_->setAlignment(Qt::AlignCenter);
	nonlinear_function_2_->hide();

	nonlinear_function_2_c_ = new QLabel(PointOperation_dock_);
	nonlinear_function_2_c_->setText(QString::fromLocal8Bit("ɑ:"));
	nonlinear_function_2_c_->hide();
	nonlinear_funcation_2_c_line_ = new QDoubleSpinBox(PointOperation_dock_);
	nonlinear_funcation_2_c_line_->setMinimum(-256);  // 最小值
	nonlinear_funcation_2_c_line_->setMaximum(256);  // 最大值
	nonlinear_funcation_2_c_line_->setDecimals(10);
	nonlinear_funcation_2_c_line_->setSingleStep(0.001);
	nonlinear_funcation_2_c_line_->hide();

	point_operation_widget->setLayout(point_operation_layout);
	PointOperation_dock_->setWidget(point_operation_widget);
	removeDockWidget(PointOperation_dock_);
	magic_docks.append(PointOperation_dock_);
	magic_menu->addAction(PointOperation);
	///=================================================================================================
	HistogramEqualization = new QAction(QString::fromLocal8Bit("直方图均衡化"), this);
	magic_menu->addAction(HistogramEqualization);
	magic_menu->addSeparator();
	///=================================================================================================
	GeometricOperation = new QAction(QString::fromLocal8Bit("几何运算"), this);
	GeometricOperation_dock_ = new QDockWidget(QString::fromLocal8Bit("几何运算"), this);
	QWidget * Geometric_widget = new QWidget(GeometricOperation_dock_);
	QGridLayout *Geometric_layout = new QGridLayout();

	transform_combo_ = new QComboBox();
	transform_combo_->addItem(QString::fromLocal8Bit("旋转"));
	transform_combo_->addItem(QString::fromLocal8Bit("缩放"));
	transform_combo_->addItem(QString::fromLocal8Bit("平移"));
	Geometric_layout->addWidget(transform_combo_,0,0);
	interpolation_algorithm_combo_ = new QComboBox(GeometricOperation_dock_);
	interpolation_algorithm_combo_->addItem(QString::fromLocal8Bit("最近邻插值算法"));
	interpolation_algorithm_combo_->addItem(QString::fromLocal8Bit("双线性插值算法"));
	Geometric_layout->addWidget(interpolation_algorithm_combo_,0,1);
	angle_multiple_line_ = new QDoubleSpinBox(GeometricOperation_dock_);
	angle_multiple_line_->setValue(0);
	angle_multiple_line_->setMaximum(360);
	angle_multiple_line_->setMinimum(-360);
	Geometric_layout->addWidget(angle_multiple_line_,1,0);

	Geometric_widget->setLayout(Geometric_layout);
	GeometricOperation_dock_->setWidget(Geometric_widget);
	removeDockWidget(GeometricOperation_dock_);
	magic_docks.append(GeometricOperation_dock_);
	magic_menu->addAction(GeometricOperation);
	magic_menu->addSeparator();
	///=================================================================================================
	averageOperation = new QAction(QString::fromLocal8Bit("邻域平均法"), this);
	connect(averageOperation, SIGNAL(triggered(bool)), this, SLOT(averageOperation_triggered()));
	medianOperation = new QAction(QString::fromLocal8Bit("中值滤波"), this);
	connect(medianOperation, SIGNAL(triggered(bool)), this, SLOT(medianOperation_triggered()));
	KneighborOperation = new QAction(QString::fromLocal8Bit("K近邻中值滤波器"), this);
	connect(KneighborOperation, SIGNAL(triggered(bool)), this, SLOT(KneighborOperation_triggered()));
	smooth_menu_ = new QMenu();
	smooth_menu_->addAction(averageOperation);
	smooth_menu_->addAction(medianOperation);
	smooth_menu_->addAction(KneighborOperation);

	laplaceOperation = new QAction(QString::fromLocal8Bit("拉普拉斯增强算子"), this);
	connect(laplaceOperation, SIGNAL(triggered(bool)), this, SLOT(laplaceOperation_triggered()));
	sobelOperation = new QAction(QString::fromLocal8Bit("Sobel算子"), this);
	connect(sobelOperation, SIGNAL(triggered(bool)), this, SLOT(sobelOperationn_triggered()));
	IsotropicOperation = new QAction(QString::fromLocal8Bit("Isotropic算子"), this);
	connect(IsotropicOperation, SIGNAL(triggered(bool)), this, SLOT(IsotropicOperatio_triggered()));
	sharpen_menu_ = new QMenu();
	sharpen_menu_->addAction(laplaceOperation);
	sharpen_menu_->addAction(sobelOperation);
	sharpen_menu_->addAction(IsotropicOperation);

	smoothOperation= new QAction(QString::fromLocal8Bit("图像平滑"), this);
	smoothOperation->setMenu(smooth_menu_);
	sharpenOperation = new QAction(QString::fromLocal8Bit("图像锐化"), this);
	sharpenOperation->setMenu(sharpen_menu_);
	customOperation= new QAction(QString::fromLocal8Bit("自定义模板"), this);
	connect(customOperation, SIGNAL(triggered(bool)), this, SLOT(customOperation_triggered()));
	enhance_menu_ = new QMenu();
	enhance_menu_->addAction(smoothOperation);
	enhance_menu_->addAction(sharpenOperation);
	enhance_menu_->addAction(customOperation);

	EnhanceOperation = new QAction(QString::fromLocal8Bit("图像增强"), this);
	EnhanceOperation->setMenu(enhance_menu_);

	magic_menu->addAction(EnhanceOperation);
	magic_menu->addSeparator();
	///=================================================================================================
	toGrayOperation = new QAction(QString::fromLocal8Bit("彩色图像灰度化处理"), this);
	connect(toGrayOperation, SIGNAL(triggered(bool)), this, SLOT(toGrayOperation_triggered()));

	magic_menu->addAction(toGrayOperation);
	magic_menu->addSeparator();
	///=================================================================================================
	EdgeDetectionOperation = new QAction(QString::fromLocal8Bit("边缘检测"), this);
	edge_detection_menu = new QMenu();

	laplace2Operation = new QAction(QString::fromLocal8Bit("拉普拉斯算子"), this);
	connect(laplace2Operation, SIGNAL(triggered(bool)), this, SLOT(laplace2Operation_triggered()));
	KirschOperation = new QAction(QString::fromLocal8Bit("Kirsch方向算子"), this);
	connect(KirschOperation, SIGNAL(triggered(bool)), this, SLOT(KirschOperation_triggered()));

	edge_detection_menu->addAction(laplace2Operation);
	edge_detection_menu->addAction(KirschOperation);

	EdgeDetectionOperation->setMenu(edge_detection_menu);
	magic_menu->addAction(EdgeDetectionOperation);
	magic_menu->addSeparator();
	///=================================================================================================
	ImageCodeOperation = new QAction(QString::fromLocal8Bit("图像编码"), this);
	
	image_code_menu = new QMenu();
	Huffmanperation = new QAction(QString::fromLocal8Bit("Huffman编码"), this);
	RunLengthOperation = new QAction(QString::fromLocal8Bit("游程编码"), this);
	image_code_menu->addAction(Huffmanperation);
	image_code_menu->addAction(RunLengthOperation);
	ImageCodeOperation -> setMenu(image_code_menu);

	huffman_menu = new QMenu();
	huffmanCompressOperation = new QAction(QString::fromLocal8Bit("压缩"), this);
	connect(huffmanCompressOperation, SIGNAL(triggered(bool)), this, SLOT(huffmanCompressOperation_triggered()));
	huffmanDecompressOperation = new QAction(QString::fromLocal8Bit("解压缩"), this);
	connect(huffmanDecompressOperation, SIGNAL(triggered(bool)), this, SLOT(huffmanDecompressOperation_triggered()));
	huffman_menu->addAction(huffmanCompressOperation);
	huffman_menu->addAction(huffmanDecompressOperation);
	Huffmanperation->setMenu(huffman_menu);

	run_length_menu = new QMenu();
	runLengthCompressOperation = new QAction(QString::fromLocal8Bit("压缩"), this);
	connect(runLengthCompressOperation, SIGNAL(triggered(bool)), this, SLOT(runLengthCompressOperation_triggered()));
	runLengthDecompressOperation = new QAction(QString::fromLocal8Bit("解压缩"), this);
	connect(runLengthDecompressOperation, SIGNAL(triggered(bool)), this, SLOT(runLengthDecompressOperation_triggered()));
	run_length_menu->addAction(runLengthCompressOperation);
	run_length_menu->addAction(runLengthDecompressOperation);
	RunLengthOperation->setMenu(run_length_menu);

	magic_menu->addAction(ImageCodeOperation);
	magic_menu->addSeparator();
	///=================================================================================================
	magic_action_->setMenu(magic_menu);

	QMenu *fileMenu = menuBar->addMenu(tr("File"));
	fileMenu->addAction(open_action_);
	fileMenu->addAction(close_action_);
	fileMenu->addSeparator();
	fileMenu->addAction(delete_action_);
	fileMenu->addAction(save_action_);
	fileMenu->addSeparator();
	fileMenu->addAction(close_action_);

	QMenu *operationMenu = menuBar->addMenu(tr("Operate"));
	operationMenu->addAction(last_action_);
	operationMenu->addAction(next_action_);
	operationMenu->addSeparator();
	operationMenu->addAction(rotate_left_action_);
	operationMenu->addAction(rotate_right_action_);
	operationMenu->addSeparator();
	operationMenu->addAction(enlarge_action_);
	operationMenu->addAction(lessen_action_);
	operationMenu->addSeparator();
	operationMenu->addAction(magic_action_);

	toolBar->addAction(open_action_);
	toolBar->addAction(close_action_);
	toolBar->addAction(last_action_);
	toolBar->addAction(next_action_);
	toolBar->addAction(rotate_left_action_);
	toolBar->addAction(rotate_right_action_);
	toolBar->addAction(enlarge_action_);
	toolBar->addAction(lessen_action_);
	toolBar->addAction(delete_action_);
	toolBar->addAction(save_action_);
	toolBar->addAction(magic_action_);

	connect(open_action_, SIGNAL(triggered(bool)), this, SLOT(openActionTriggered()));
	connect(close_action_, SIGNAL(triggered(bool)), this, SLOT(closeActionTriggered()));
	connect(last_action_, SIGNAL(triggered(bool)), this, SLOT(lastActionTriggered()));
	connect(next_action_, SIGNAL(triggered(bool)), this, SLOT(nextActionTriggered()));
	connect(rotate_left_action_, SIGNAL(triggered(bool)), this, SLOT(toLeftActionTriggered()));
	connect(rotate_right_action_, SIGNAL(triggered(bool)), this, SLOT(toRightActionTriggered()));
	connect(enlarge_action_, SIGNAL(triggered(bool)), this, SLOT(toEnlargeActionTriggered()));
	connect(lessen_action_, SIGNAL(triggered(bool)), this, SLOT(toLessenActionTriggered()));
	connect(delete_action_, SIGNAL(triggered(bool)), this, SLOT(deleteActionTriggered()));
	connect(save_action_, SIGNAL(triggered(bool)), this, SLOT(saveActionTriggered()));

	// 连接8转2信号槽
	connect(eight2two, SIGNAL(triggered(bool)), this, SLOT(eight2twoTriggered()));
	connect(eight_to_binary_map_eight_button_group_, SIGNAL(buttonClicked(int)), this, SLOT(on8to2ButtonClicked(int)));

	//直方图显示
	connect(grayShow, SIGNAL(triggered(bool)), this, SLOT(gray_show_Triggered()));
	// 阈值化
	connect(Thresholding, SIGNAL(triggered(bool)), this, SLOT(ThresholdingTriggered()));
	connect(Thresholding_spinBox, SIGNAL(valueChanged(int)), Thresholding_slider, SLOT(setValue(int)));
	connect(Thresholding_slider, SIGNAL(valueChanged(int)), Thresholding_spinBox, SLOT(setValue(int)));
	connect(Thresholding_spinBox, SIGNAL(valueChanged(int)), SLOT(ThresholdingChangged(int)));
	Thresholding_spinBox->setValue(0);

	//点运算
	connect(PointOperation, SIGNAL(triggered(bool)), this, SLOT(point_opeartion_Triggered()));
	connect(point_operation_group_, SIGNAL(buttonClicked(int)), this, SLOT(point_opration_Clicked(int)));
	connect(linear_funcation_k_line, SIGNAL(valueChanged(double)), SLOT(linear_funcationChangged(double)));
	connect(linear_funcation_b_line_, SIGNAL(valueChanged(double)), SLOT(linear_funcationChangged(double)));
	connect(nonlinear_funcation_1_c_line_, SIGNAL(valueChanged(double)), SLOT(nonlinear_funcation1Changged(double)));
	connect(nonlinear_funcation_2_c_line_, SIGNAL(valueChanged(double)), SLOT(nonlinear_funcation2Changged(double)));

	//直方图均衡化
	connect(HistogramEqualization, SIGNAL(triggered(bool)), this, SLOT(histogram_equalization_Triggered()));

	//几何运算
	connect(GeometricOperation, SIGNAL(triggered(bool)), this, SLOT(geometric_operation_triggered()));
	connect(transform_combo_, SIGNAL(currentIndexChanged(int)), this, SLOT(interpolation_algorithm_combo_changed(int)));
	connect(angle_multiple_line_, SIGNAL(valueChanged(double)), SLOT(angle_multipleChangged(double)));
}

void MainWindow::setQImageViewerWidget(void)
{
	/* label show image */
	imageLabel = new QLabel();

	QScrollArea *imageScrollArea = new QScrollArea();
	imageScrollArea->setAlignment(Qt::AlignCenter);
	imageScrollArea->setFrameShape(QFrame::NoFrame);
	imageScrollArea->setWidget(imageLabel);

	QGridLayout *mainLayout = new QGridLayout();
	mainLayout->addWidget(imageScrollArea, 0, 0);
	centralWidget->setLayout(mainLayout);
}

void MainWindow::loadImageResource(void)
{
	imageLabel->setPixmap(imageViewer->pixmap);
	imageLabel->resize(imageViewer->size);
	setWindowTitle(QFileInfo(imageViewer->original_image_->filename).fileName() + tr(" - QImageEnhancement"));
	getHistogram();
	updateHistogram();
}

void MainWindow::openActionTriggered(void)
{
	int ret = imageViewer->openImageFile(tr("Select image:"), "C:\\", tr("Images (*.jpg *.jpeg *.png *.bmp *.gif)"));
	if (ret)
	{
		QMessageBox::information(this, tr("Error"), tr("Open a file failed!"));
		return;
	}

	loadImageResource();
}

void MainWindow::closeActionTriggered(void)
{
	initImageResource();
	imageViewer->closeImageFile();
}

void MainWindow::lastActionTriggered(void)
{
	int ret = imageViewer->last();
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::nextActionTriggered(void)
{
	int ret = imageViewer->next();
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::toLeftActionTriggered(void)
{
	int ret = imageViewer->spinToLeft();
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::toRightActionTriggered(void)
{
	int ret = imageViewer->spinToRight();
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::toEnlargeActionTriggered(void)
{
	int ret = imageViewer->zoomIn();
	if (ret) {

		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::toLessenActionTriggered(void)
{
	int ret = imageViewer->zoomOut();
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::deleteActionTriggered(void)
{
	if (!QFile(imageViewer->original_image_->filename).exists())
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	QMessageBox message(QMessageBox::Warning,
		tr("Warning"),
		tr("Do you want to delete this image?"),
		QMessageBox::Yes | QMessageBox::No,
		NULL);
	if (message.exec() == QMessageBox::No)
	{
		return;
	}

	int ret = imageViewer->delImageFile();
	if (ret) {
		QMessageBox::warning(this,
			tr("Error"),
			tr("Delete a image failed!"));
		return;
	}

	initImageResource();
}

void MainWindow::saveActionTriggered(void)
{
	int ret = imageViewer->saveImageFile(tr("Select folder:"), "C:\\");
	if (ret)
	{
		QMessageBox::information(this, tr("Error"), tr("Open a image, please!"));
		return;
	}
	else
	{
		QMessageBox::information(this, tr("Success"), tr("Save image success"));
		return;
	}
}

void MainWindow::eight2twoTriggered(void)
{
	//for (int i = 0; i < magic_docks.length(); i++)
	//    removeDockWidget(magic_docks[i]);
	eight_to_binary_map_->show();
	addDockWidget(Qt::RightDockWidgetArea, eight_to_binary_map_);
}

void MainWindow::on8to2ButtonClicked(int button_id)
{
	int ret = imageViewer->showBinary(button_id);
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::gray_show_Triggered(void)
{
	//    for (int i = 0; i < magic_docks.length(); i++)
	//        removeDockWidget(magic_docks[i]);
		//getHistogram(); 
		//updateHistogram();
	gray_show_dock_->show();
	addDockWidget(Qt::RightDockWidgetArea, gray_show_dock_);
}
void MainWindow::getHistogram(void)
{
	double average, mid, standard, pixel;
	int ret = imageViewer->grayStatistics(average, mid, standard, pixel, data_list_, original_data_list_);
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}
	char num[10] = { 0 };
	sprintf(num, "%.2lf", average);
	average_gray_num_->setText(num);
	memset(num, 0, sizeof(num));
	sprintf(num, "%.2lf", mid);
	mid_gray_num_->setText(num);
	sprintf(num, "%.2lf", standard);
	standard_deviation_gray_num_->setText(num);
	sprintf(num, "%.2lf", pixel);
	pixel_sum_num_->setText(num);
}
void MainWindow::updateHistogram(void)
{
	if (area != nullptr)
	{
		chart->removeSeries(area);
		delete area;
	}
	chart->removeSeries(series);
	original_series_->clear();
	series->clear();
	for (int i(0); i < data_list_.count(); i++)
	{
		series->append(data_list_[i]);
		original_series_->append(original_data_list_[i]);
	}
	area = new QAreaSeries(original_series_);
	area->setName(QString::fromLocal8Bit("Before Gray"));
	chart->addSeries(area);
	chart->addSeries(series);
	chart->createDefaultAxes();
}

void MainWindow::ThresholdingTriggered(void)
{
	//for (int i = 0; i < magic_docks.length(); i++)
	//    removeDockWidget(magic_docks[i]);
	Thresholding_dock_->show();
	addDockWidget(Qt::BottomDockWidgetArea, Thresholding_dock_);
}

void  MainWindow::ThresholdingChangged(int value)
{
	int ret = imageViewer->ChanggeThreshold(value);
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::point_opeartion_Triggered(void)
{
	//for (int i = 0; i < magic_docks.length(); i++)
	//	removeDockWidget(magic_docks[i]);
	PointOperation_dock_->show();
	addDockWidget(Qt::RightDockWidgetArea, PointOperation_dock_);
}

void MainWindow::point_opration_Clicked(int button_id)
{
	point_operation_layout->removeWidget(linear_funcation_);
	linear_funcation_->hide();
	point_operation_layout->removeWidget(linear_funcation_k_);
	linear_funcation_k_->hide();
	point_operation_layout->removeWidget(linear_funcation_k_line);
	linear_funcation_k_line->hide();
	point_operation_layout->removeWidget(linear_funcation_b_);
	linear_funcation_b_->hide();
	point_operation_layout->removeWidget(linear_funcation_b_line_);
	linear_funcation_b_line_->hide();
	point_operation_layout->removeWidget(nonlinear_function_1_);
	nonlinear_function_1_->hide();
	point_operation_layout->removeWidget(nonlinear_function_1_c_);
	nonlinear_function_1_c_->hide();
	point_operation_layout->removeWidget(nonlinear_funcation_1_c_line_);
	nonlinear_funcation_1_c_line_->hide();
	point_operation_layout->removeWidget(nonlinear_function_2_);
	nonlinear_function_2_->hide();
	point_operation_layout->removeWidget(nonlinear_function_2_c_);
	nonlinear_function_2_c_->hide();
	point_operation_layout->removeWidget(nonlinear_funcation_2_c_line_);
	nonlinear_funcation_2_c_line_->hide();
	if (button_id == 0)
	{
		point_operation_layout->addWidget(linear_funcation_, 2, 0, 1, 4);
		linear_funcation_->show();
		point_operation_layout->addWidget(linear_funcation_k_, 3, 0);
		linear_funcation_k_->show();
		point_operation_layout->addWidget(linear_funcation_k_line, 3, 1);
		linear_funcation_k_line->show();
		point_operation_layout->addWidget(linear_funcation_b_, 3, 2);
		linear_funcation_b_->show();
		point_operation_layout->addWidget(linear_funcation_b_line_, 3, 3);
		linear_funcation_b_line_->show();
	}
	if (button_id == 1)
	{
		point_operation_layout->addWidget(nonlinear_function_1_, 2, 0, 1, 4);
		nonlinear_function_1_->show();
		point_operation_layout->addWidget(nonlinear_function_1_c_, 3, 1);
		nonlinear_function_1_c_->show();
		point_operation_layout->addWidget(nonlinear_funcation_1_c_line_, 3, 2);
		nonlinear_funcation_1_c_line_->show();
	}
	if (button_id == 2)
	{
		point_operation_layout->addWidget(nonlinear_function_2_, 2, 0, 1, 4);
		nonlinear_function_2_->show();
		point_operation_layout->addWidget(nonlinear_function_2_c_, 3, 1);
		nonlinear_function_2_c_->show();
		point_operation_layout->addWidget(nonlinear_funcation_2_c_line_, 3, 2);
		nonlinear_funcation_2_c_line_->show();
	}
}

void MainWindow::linear_funcationChangged(double value)
{
	double k = linear_funcation_k_line->value();
	double b = linear_funcation_b_line_->value();
	int ret = imageViewer->pointOperationLinearChange(k, b);
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}
void MainWindow::nonlinear_funcation1Changged(double value)
{
	int ret = imageViewer->pointOperationNonLinear1Change(value);
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}
void MainWindow::nonlinear_funcation2Changged(double value)
{
	int ret = imageViewer->pointOperationNonLinear2Change(value);
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::histogram_equalization_Triggered()
{
	int ret = imageViewer->histogram_equalization();
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::geometric_operation_triggered(void)
{
	GeometricOperation_dock_->show();
	addDockWidget(Qt::RightDockWidgetArea, GeometricOperation_dock_);
}
void MainWindow::angle_multipleChangged(double value)
{
	int ret = imageViewer->geometry(transform_combo_->currentIndex(), interpolation_algorithm_combo_->currentIndex(),value);
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}
void MainWindow::interpolation_algorithm_combo_changed(int index)
{
	interpolation_algorithm_combo_->clear();
	if (index == 0)
	{
		interpolation_algorithm_combo_->addItem(QString::fromLocal8Bit("最近邻插值算法"));
		interpolation_algorithm_combo_->addItem(QString::fromLocal8Bit("双线性插值算法"));
		angle_multiple_line_->setSingleStep(1);
		angle_multiple_line_->setDecimals(0);
		angle_multiple_line_->setMaximum(360);
		angle_multiple_line_->setMinimum(-360);
	}
	if (index == 1)
	{
		interpolation_algorithm_combo_->addItem(QString::fromLocal8Bit("最近邻插值算法"));
		interpolation_algorithm_combo_->addItem(QString::fromLocal8Bit("双线性插值算法"));
		angle_multiple_line_->setSingleStep(0.01);
		angle_multiple_line_->setValue(1);
		angle_multiple_line_->setDecimals(5);
		angle_multiple_line_->setMaximum(10);
		angle_multiple_line_->setMinimum(0);
	}
	if (index == 2)
	{
		interpolation_algorithm_combo_->addItem(QString::fromLocal8Bit("X"));
		interpolation_algorithm_combo_->addItem(QString::fromLocal8Bit("Y"));
		angle_multiple_line_->setSingleStep(0.01);
		angle_multiple_line_->setDecimals(0);
		angle_multiple_line_->setMaximum(1000);
		angle_multiple_line_->setMinimum(0);
	}
}

void MainWindow::averageOperation_triggered(void)
{
	int ret = imageViewer->averagen();
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::medianOperation_triggered(void)
{
	int ret = imageViewer->median();
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::KneighborOperation_triggered(void)
{
	int ret = imageViewer->Kneighbor();
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::laplaceOperation_triggered(void)
{
	int ret = imageViewer->laplace();
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::sobelOperationn_triggered(void)
{
	int ret = imageViewer->sobel();
		if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::IsotropicOperatio_triggered(void)
{
	int ret = imageViewer->Isotropic();
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::customOperation_triggered(void)
{
	getmatrix = new getTemplate();
	getmatrix->show();
	connect(getmatrix, SIGNAL(matrixOk(void)), this, SLOT(customBegin(void)));

}

void MainWindow::customBegin(void)
{
	double *table = getmatrix->getTable();
	int ret = imageViewer->custom(table,getmatrix->get_y_spin(), getmatrix->get_x_spin());
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}
	delete getmatrix;
	delete table;
	loadImageResource();
}

void MainWindow::toGrayOperation_triggered(void)
{
	int ret = imageViewer->toGray();
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::laplace2Operation_triggered(void)
{
	int ret = imageViewer->laplace2();
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::KirschOperation_triggered(void)
{
	int ret = imageViewer->Kirsch();
	if (ret)
	{
		QMessageBox::information(this,
			tr("Error"),
			tr("Open a image, please!"));
		return;
	}

	loadImageResource();
}

void MainWindow::huffmanCompressOperation_triggered(void)
{
	int ret = imageViewer->huffmanCompress(tr("Select path:"), "C:\\");
	if (ret)
	{
		QMessageBox::information(this, tr("Error"), tr("Open a file failed!"));
		return;
	}

	loadImageResource();
}

void MainWindow::huffmanDecompressOperation_triggered(void)
{
	int ret = imageViewer->huffmanDecompress(tr("Select path:"), "C:\\", tr("compress (*.txt)"));
	if (ret)
	{
		QMessageBox::information(this, tr("Error"), tr("Open a file failed!"));
		return;
	}

	loadImageResource();
}

void MainWindow::runLengthCompressOperation_triggered(void)
{
	int ret = imageViewer->runLengthCompress(tr("Select path:"), "C:\\");
	if (ret)
	{
		QMessageBox::information(this, tr("Error"), tr("Open a file failed!"));
		return;
	}

	loadImageResource();
}

void MainWindow::runLengthDecompressOperation_triggered(void)
{
	int ret = imageViewer->runLengthDecompress(tr("Select path:"), "C:\\", tr("compress (*.txt)"));
	if (ret)
	{
		QMessageBox::information(this, tr("Error"), tr("Open a file failed!"));
		return;
	}

	loadImageResource();
}
