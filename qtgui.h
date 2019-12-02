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

private slots:
	void on_checkBox_clicked();
	void on_checkBox_2_clicked();
	void on_pushButton_clicked();
};
