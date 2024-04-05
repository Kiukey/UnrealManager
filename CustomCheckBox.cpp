#include "CustomCheckBox.h"

CustomCheckBox::CustomCheckBox(QWidget* _parent) : QCheckBox(_parent)
{
	connect(this, &CustomCheckBox::stateChanged, this, &CustomCheckBox::OnStateChanged);
}

CustomCheckBox::CustomCheckBox(const QString& _text, QWidget* _parent) : QCheckBox(_text,_parent)
{
	connect(this, &CustomCheckBox::stateChanged, this, &CustomCheckBox::OnStateChanged);
}

void CustomCheckBox::OnStateChanged(int _state)
{
	emit OnCheckBoxStateChanged(this, _state);
}