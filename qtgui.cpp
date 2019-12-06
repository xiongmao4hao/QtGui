#include "qtgui.h"

QtGui::QtGui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void qtgui::timerEvent(QTimerEvent* event)
{
	//�жϵ�ǰ��ʱ����Ӧ�����ĸ�ld.
	if (event->timerId() == this->m_lamp)
	{
		if (this->m_lampStatus == false)
		{
			//���ð�ť��ͼ��.
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