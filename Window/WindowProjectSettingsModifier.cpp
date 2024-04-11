#include "WindowProjectSettingsModifier.h"
#include <qerrormessage.h>
#include "../FileClasses/ConfigFile.h"

WindowProjectSettingsModifier::WindowProjectSettingsModifier(UnrealProject* _project,QWidget *parent)
	: WindowProjectSettings(_project,parent)
{
	ui.setupUi(this);
}

WindowProjectSettingsModifier::~WindowProjectSettingsModifier()
{
}

void WindowProjectSettingsModifier::on_applyButton_clicked()
{
	for (std::pair<QString, ConfigFile*> _pair : files)
	{
		if (!_pair.second->SaveFileInProject())
		{
			QErrorMessage _message = QErrorMessage(this);
			_message.showMessage(QString("Error saving file in path %1").arg(_pair.second->GetPath()));
			_message.exec();
		}
	}
}