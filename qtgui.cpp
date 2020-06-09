#include "qtgui.h"

#include "kinect_record.h" 
#include "read_record.h"
#include "windows_thread.h"

//几个重要函数
//1.int startTimer(int); //设置定时器，返回一个ld.
//2.int event->timerld(); //返回当前的ld.
//3.void killTimer(int); //停止定时器.

float joints_AngelALL[ANGLE_NUM];
float dis[1];
QTextBrowser* qt_joints_Angel[ANGLE_NUM];
QTextBrowser* qt_dis;

float joints_AngelALL_r[ANGLE_NUM];
QTextBrowser* qt_joints_Angel_r[ANGLE_NUM];

char* input_path;
uint32_t num;


QtGui::QtGui(QWidget *parent)
	: QMainWindow(parent)
{
	//ui = new Ui::QtGuiClass;

	ui.setupUi(this);
	//uir.setupUi(this);
	//初始化保存角度和窗口的数组
	for (int i = 0; i < ANGLE_NUM; i++) joints_AngelALL[i] = NULL;
	QTextBrowser*  qt_joints_Angel_TCM[ANGLE_NUM] = \
	{
		ui.wrist_left,
		ui.elbow_left,
		ui.shoulder_left,
		ui.wrist_right,
		ui.elbow_right,
		ui.shoulder_right,
		ui.knee_left,
		ui.ankle_left,
		ui.knee_right,
		ui.ankle_right,
		ui.neck,
		ui.spin_naval,
		ui.transverse_left,
		ui.sagittal_left,
		ui.frantal_left,
		ui.transverse_right,
		ui.sagittal_right,
		ui.frantal_right,
	};
	qt_dis = ui.disEE;
	for (int i = 0; i < ANGLE_NUM; i++) qt_joints_Angel[i] = qt_joints_Angel_TCM[i];
	/********************初始化录像的保存相关****/
	/*for (int i = 0; i < ANGLE_NUM; i++) joints_AngelALL_r[i] = NULL;
	QTextBrowser* qt_joints_Angel_TCM_R[ANGLE_NUM] = \
	{
		uir.shoulder_left,
			uir.shoulder_right,
			uir.elbow_left,
			uir.elbow_right,
			uir.wrist_left,
			uir.wrist_right,
			uir.knee_left,
			uir.knee_right,
			uir.ankle_left,
			uir.ankle_right,
			uir.neck,
			uir.spin_naval,
			uir.transverse_left,
			uir.frantal_left,
			uir.sagittal_left,
			uir.transverse_right,
			uir.frantal_right,
			uir.sagittal_right,
	};
	for (int i = 0; i < ANGLE_NUM; i++) qt_joints_Angel_r[i] = qt_joints_Angel_TCM_R[i];*/
	/********************初始化录像的保存相关结束****/
	std::thread record = std::thread(record_main, std::ref(joints_AngelALL), std::ref(dis));
	/*std::thread record_r = std::thread(read_record_main, input_path, std::ref(joints_AngelALL), num);*/
	record.detach();
	//record_r.detach();
	//连接录像界面信号与槽.
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
			float di = dis[0];
			for (int i = 0; i < ANGLE_NUM; i++)
			{
				float Angel_TCM = joints_AngelALL[i];
				QString line;
				line = line.sprintf("%f", Angel_TCM);
				//设置显示.
				/*ui.shoulder_left->setText(line);*/
				qt_joints_Angel[i]->setText(line);

			}
			QString line;
			line = line.sprintf("%f", di);
			qt_dis->setText(line);
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

