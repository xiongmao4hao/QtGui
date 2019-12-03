#include "qtgui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtGui w(0);//无法设置Qt::WindowType
	w.setWindowTitle(QStringLiteral("kinect窗口"));
	w.show();
	return a.exec();
}
