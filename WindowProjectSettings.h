#pragma once

#include <QDialog>
#include "ui_WindowProjectSettings.h"
#include <qscrollarea.h>
#include <vector>

class UnrealProject;
class ConfigFile;
class CustomCheckBox;


struct DisplayedSettings
{
	ConfigFile* file = nullptr;
	QScrollArea* area = nullptr;

	DisplayedSettings(ConfigFile* _file, QScrollArea* _scrollArea)
	{
		file = _file;	
		area = _scrollArea;
	}
};

class WindowProjectSettings : public QDialog
{
	Q_OBJECT
	Ui::WindowProjectSettingsClass ui;
	//to not delete because the main window handle that
	UnrealProject* project = nullptr;
	std::vector<DisplayedSettings> templateSettings = std::vector<DisplayedSettings>();
	std::map<QString, ConfigFile*> files = std::map<QString, ConfigFile*>();
	//std::vector<SettingsTemplate> templatePaths = std::vector<SettingsTemplate>();
public:
	WindowProjectSettings(UnrealProject* _project, QWidget* parent = nullptr);
	~WindowProjectSettings();

private:
	void GenerateAllTabs();
	void GenerateTab(const QString& _currentSettings,DisplayedSettings& _settings);
	
	void CreateSetting(const QString& _settingLine, class QGroupBox* _groupBox);
	
	ConfigFile* LoadSavedFile(const QByteArray& _array);
	DisplayedSettings CreateDisplayedSettings(const QString& _path, QScrollArea* _area);

	//Working but reworking
	//void OnCheckBoxValueChanged(CustomCheckBox* _checkBox, int _state);
	//void ChangeParameter(const QString& _file,const QString& _category,const QString& _setting, bool _value);

	template<typename T1,typename T2>
	bool Contains(const std::map<T1, T2>& _map, T1 _key)
	{
		for (const std::pair<T1, T2>& _pair : _map)
		{
			if (_pair.first == _key) return true;
		}
		return false;
	}
};
