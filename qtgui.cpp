#include "qtgui.h"

//几个重要函数
//1.int startTimer(int); //设置定时器，返回一个ld.
//2.int event->timerld(); //返回当前的ld.
//3.void killTimer(int); //停止定时器.

QtGui::QtGui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//连接信号与槽.
	connect(ui.startButton, SIGNAL(clicked()), this, SLOT(startTimerSlot()));
	connect(ui.stopButton, SIGNAL(clicked()), this, SLOT(stopTimerSlot()));
}

void QtGui::timerEvent(QTimerEvent* event)
{
	//判断当前定时器对应的是哪个ld.
	if (event->timerId() == this->m_lamp)//1000毫秒显示一次
	{
		if (this->m_lampStatus == false)
		{
			QString line = "dd";
			//设置按钮的图标.
			ui.shoulder_left->setText(line);
			
			this->m_lampStatus = true;
		}
		else
		{
			this->m_lampStatus = false;
		}
	}
}

void QtGui::startTimerSlot()
{
	//设置定时器，返回一个timerld.注意单位为毫秒，1000毫秒等于1秒.
	this->m_lamp = this->startTimer(1000);
}

void QtGui::stopTimerSlot()
{
	//停止定时器.
	this->killTimer(this->m_lamp);
}