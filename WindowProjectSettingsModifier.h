#pragma once

#include "WindowProjectSettings.h"
#include "ui_WindowProjectSettingsModifier.h"

class WindowProjectSettingsModifier : public WindowProjectSettings
{
	Q_OBJECT

public:
	WindowProjectSettingsModifier(UnrealProject* _project,QWidget *parent = nullptr);
	~WindowProjectSettingsModifier();

private:
	Ui::WindowProjectSettingsModifierClass ui;

private slots:
	void on_applyButton_clicked();
};
