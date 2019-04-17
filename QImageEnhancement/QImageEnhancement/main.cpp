#include "qimageenhancement.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QApplication::addLibraryPath("./plugins");
	MainWindow w;
	w.resize(640, 480);
	w.setWindowIcon(QIcon(":/QImageEnhancement/images/logo.ico"));
	w.show();

	return a.exec();
}
