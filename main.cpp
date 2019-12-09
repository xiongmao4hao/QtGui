#include "qtgui.h"
#include <QtWidgets/QApplication>
#include "kinect_record.h" 
#include <thread>
#include <iostream>
#include <windows.h>
#include <thread>
#include <iostream>
#include <windows.h>


int main(int argc, char *argv[])
{
	float joints_Angel[12];
	for (int i = 0; i < 12; i++) joints_Angel[i] = NULL;
	QApplication app(argc, argv);
	QtGui w(0);//无法设置Qt::WindowType？
	w.setWindowTitle(QStringLiteral("kinect窗口"));
	w.show();
	std::thread record = std::thread(record_main,std::ref(joints_Angel));
	record.detach();
	return app.exec();	
}
