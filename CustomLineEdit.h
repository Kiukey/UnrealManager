#pragma once
#include <qlineedit.h>

class CustomLineEdit : public QLineEdit
{
	Q_OBJECT

public:
	CustomLineEdit(QWidget* _parent);
	CustomLineEdit(const QString& _text, QWidget* _parent);


signals:
	void EditFinishedCustomLineEdit(CustomLineEdit* _self, const QString& _text);

private slots:
	void OnEditFinished();
};

