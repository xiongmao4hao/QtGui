#include "qtgui.h"

#include "kinect_record.h" 
#include "windows_thread.h"

//������Ҫ����
//1.int startTimer(int); //���ö�ʱ��������һ��ld.
//2.int event->timerld(); //���ص�ǰ��ld.
//3.void killTimer(int); //ֹͣ��ʱ��.

float joints_Angel[ANGLE_NUM];
QTextBrowser* qt_joints_Angel[ANGLE_NUM];

QtGui::QtGui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	//��ʼ������ǶȺʹ��ڵ�����
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
			for (int i = 0; i < ANGLE_NUM; i++)
			{
				float Angel_TCM = joints_Angel[i];
				QString line;
				line = line.sprintf("%f", Angel_TCM);
				//������ʾ.
				/*ui.shoulder_left->setText(line);*/
				qt_joints_Angel[i]->setText(line);

			}
			this->m_lampStatus = true;
		}
		else
		{
			//QString line = "sd";
			////������ʾ.
			//ui.shoulder_left->setText(line);
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