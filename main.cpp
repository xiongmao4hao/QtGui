#include "qtgui.h"
#include <QtWidgets/QApplication>
#include "kinect_record.h" 
#include <thread>
#include <iostream>
#include <windows.h>


int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	QtGui w(0);//�޷�����Qt::WindowType��
	w.setWindowTitle(QStringLiteral("kinect����"));
	w.show();
	record_main();
	thread* recordd = thread(record_main());
	return app.exec();
}
