#include "qtgui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtGui w(0);//�޷�����Qt::WindowType
	w.setWindowTitle(QStringLiteral("kinect����"));
	w.show();
	return a.exec();
}
