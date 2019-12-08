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
	record_main();
	thread* recordd = thread(record_main());
	return app.exec();
}
