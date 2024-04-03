#pragma once

#include <QDialog>
#include "ui_WindowProjectSettings.h"
#include <qscrollarea.h>

class UnrealProject;

struct SettingsTemplate
{
	QString path;
	QScrollArea* container = nullptr;

	SettingsTemplate(const QString& _path, QScrollArea* _area)
	{
		path = _path;
		container = _area;
	}
};


class WindowProjectSettings : public QDialog
{
	Q_OBJECT
	Ui::WindowProjectSettingsClass ui;
	//to not delete because the main window handle that
	UnrealProject* project = nullptr;
	std::vector<SettingsTemplate> templatePaths = std::vector<SettingsTemplate>();
public:
	WindowProjectSettings(UnrealProject* _project, QWidget* parent = nullptr);
	~WindowProjectSettings();

private:
	void GenerateAllTabs();
	void GenerateTab(const QString& _currentSettings, const SettingsTemplate& _template);
};
