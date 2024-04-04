#pragma once
#include <qcheckbox.h>

class CustomCheckBox : public QCheckBox
{
	Q_OBJECT
public :
	CustomCheckBox(QWidget* _parent = nullptr);
	CustomCheckBox(const QString& _text,QWidget* _parent = nullptr);


signals:
	void OnCheckBoxStateChanged(CustomCheckBox* _self, int _state);
private slots:
	void OnStateChanged(int _state);
};

