#include "qtgui.h"

#include "kinect_record.h" 
#include "windows_thread.h"

//几个重要函数
//1.int startTimer(int); //设置定时器，返回一个ld.
//2.int event->timerld(); //返回当前的ld.
//3.void killTimer(int); //停止定时器.

float joints_Angel[ANGLE_NUM];
QTextBrowser* qt_joints_Angel[ANGLE_NUM];

QtGui::QtGui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//初始化保存角度和窗口的数组
	for (int i = 0; i < ANGLE_NUM; i++) joints_Angel[i] = NULL;
	QTextBrowser*  qt_joints_Angel_TCM[ANGLE_NUM] = \
	{
		ui.shoulder_left,
	    ui.shoulder_right,
		ui.elbow_left,
		ui.elbow_right,
		ui.wrist_left,
		ui.wrist_right,
		ui.knee_left,
		ui.knee_right,
		ui.ankle_left,
		ui.ankle_right,
		ui.neck,
		ui.spin_naval,
		ui.transverse_left,
		ui.frantal_left,
		ui.sagittal_left,
		ui.transverse_right,
		ui.frantal_right,
		ui.sagittal_right,	
	};
	for (int i = 0; i < ANGLE_NUM; i++) qt_joints_Angel[i] = qt_joints_Angel_TCM[i];

	std::thread record = std::thread(record_main, std::ref(joints_Angel));
	record.detach();
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
			for (int i = 0; i < ANGLE_NUM; i++)
			{
				float Angel_TCM = joints_Angel[i];
				QString line;
				line = line.sprintf("%f", Angel_TCM);
				//设置显示.
				/*ui.shoulder_left->setText(line);*/
				qt_joints_Angel[i]->setText(line);

			}
			this->m_lampStatus = true;
		}
		else
		{
			//QString line = "sd";
			////设置显示.
			//ui.shoulder_left->setText(line);
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