#include "qtgui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtGui w;//һ�Ż�
	w.setWindowTitle(QStringLiteral("kinect����"));
	w.show();
	return a.exec();
}
