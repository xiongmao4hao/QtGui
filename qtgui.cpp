#include "qtgui.h"

QtGui::QtGui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}
void QtGui::on_checkBox_clicked()
{
	if (ui.checkBox->isChecked())
	{
		ui.lineEdit->setReadOnly(true);
	}
	else
	{
		ui.lineEdit->setReadOnly(false);
	}
}
void QtGui::on_checkBox_2_clicked()
{
	if (ui.checkBox_2->isChecked())
	{
		ui.lineEdit->setEchoMode(QLineEdit::Password);
	}
	else
	{
		ui.lineEdit->setEchoMode(QLineEdit::Normal);
	}
}

void QtGui::on_pushButton_clicked()
{
	ui.label->setText(ui.lineEdit->text());
}