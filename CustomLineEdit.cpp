#include "CustomLineEdit.h"

CustomLineEdit::CustomLineEdit(QWidget* _parent) : QLineEdit(_parent)
{
	connect(this, &CustomLineEdit::editingFinished, this, &CustomLineEdit::OnEditFinished);
}

CustomLineEdit::CustomLineEdit(const QString& _text, QWidget* _parent) : QLineEdit(_text, _parent)
{
	connect(this, &CustomLineEdit::editingFinished, this, &CustomLineEdit::OnEditFinished);
}

void CustomLineEdit::OnEditFinished()
{
	EditFinishedCustomLineEdit(this, text());
}