#include "qtgui.h"

QtGui::QtGui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void qtgui::timerEvent(QTimerEvent* event)
{
	//判断当前定时器对应的是哪个ld.
	if (event->timerId() == this->m_lamp)
	{
		if (this->m_lampStatus == false)
		{
			//设置按钮的图标.
			ui.toolButton->setIcon(QIcon("Icons/lamp.png"));
			this->m_lampStatus = true;
		}
		else
		{
			ui.toolButton->setIcon(QIcon("Icons/space.png"));
			this->m_lampStatus = false;
		}
	}
}