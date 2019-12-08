#include "qtgui.h"
#include <QtWidgets/QApplication>
#include "kinect_record.h" 
#include <thread>
#include <iostream>
#include <windows.h>


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QtGui w(0);//无法设置Qt::WindowType？
	w.setWindowTitle(QStringLiteral("kinect窗口"));
	w.show();
	std::thread record = std::thread(record_main);
	record.detach();
	return app.exec();
}
