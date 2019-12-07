#include "qtgui.h"

//������Ҫ����
//1.int startTimer(int); //���ö�ʱ��������һ��ld.
//2.int event->timerld(); //���ص�ǰ��ld.
//3.void killTimer(int); //ֹͣ��ʱ��.

QtGui::QtGui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//�����ź����.
	connect(ui.startButton, SIGNAL(clicked()), this, SLOT(startTimerSlot()));
	connect(ui.stopButton, SIGNAL(clicked()), this, SLOT(stopTimerSlot()));
}

void QtGui::timerEvent(QTimerEvent* event)
{
	//�жϵ�ǰ��ʱ����Ӧ�����ĸ�ld.
	if (event->timerId() == this->m_lamp)//1000������ʾһ��
	{
		if (this->m_lampStatus == false)
		{
			QString line = "dd";
			//���ð�ť��ͼ��.
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
	//���ö�ʱ��������һ��timerld.ע�ⵥλΪ���룬1000�������1��.
	this->m_lamp = this->startTimer(1000);
}

void QtGui::stopTimerSlot()
{
	//ֹͣ��ʱ��.
	this->killTimer(this->m_lamp);
}