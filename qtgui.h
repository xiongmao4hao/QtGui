#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_qtgui.h"

class QtGui : public QMainWindow
{
	Q_OBJECT

public:
	QtGui(QWidget *parent = Q_NULLPTR);

private:
	Ui::QtGuiClass ui;

protected:
	void timerEvent(QTimerEvent* event);

private :
	int m_lamp;
	bool m_lampStatus = false;

private slots:
	void startTimerSlot();
	void stopTimerSlot();
	//void on_checkBox_clicked();
	//void on_checkBox_2_clicked();
	//void on_pushButton_clicked();
};
