#include "qtgui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication::addLibraryPath(". / plugins");
	QApplication app(argc, argv);
	QtGui w(0);//无法设置Qt::WindowType？
	w.setWindowTitle(QStringLiteral("kinect窗口"));
	w.show();
	return app.exec();	
}
