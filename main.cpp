#include "qtgui.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	QtGui w;//Ò»ºÅ»ú
	w.setWindowTitle(QStringLiteral("kinect´°¿Ú"));
	w.show();
	return a.exec();
}
