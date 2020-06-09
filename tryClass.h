#pragma once

#include <QWidget>
#include "ui_tryClass.h"

class tryClass : public QWidget
{
	Q_OBJECT

public:
	tryClass(QWidget *parent = Q_NULLPTR);
	~tryClass();

private:
	Ui::tryClass ui;
};
