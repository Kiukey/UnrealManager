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
public:
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
	/// <summary>
	/// called in constructor of this window to create all the buttons, settings, etc... 
	/// </summary>
	void GenerateAllTabs();
	/// <summary>
	/// called in GenerateAllTabs, Generate one ConfigFile class, register it in map, and create all settings corresponding to the file template
	/// </summary>
	/// <param name="_filePath"> the path to the .ini file of the project selected</param>
	/// <param name="_currentSettings">the content of the .ini file of the project</param>
	/// <param name="_settings">structure wich is keeping the template file and the scrollbox area</param>
	void GenerateTab(ConfigFile* _file,DisplayedSettings& _settings);
	/// <summary>
	/// Create one setting, and set it's value to either the default value in the template or the value in the .ini file
	/// </summary>
	/// <param name="_settingLine">a setting line to display</param>
	/// <param name="_groupBox">the group box in wich the setting will be in</param>
	/// <param name="_file">the .ini file of the project selected containing this line</param>
	void CreateSetting(const QString& _settingLine, class QGroupBox* _groupBox, const ConfigFile* _file, int _i);
	/// <summary>
	/// load a saved file and create a Config File with it
	/// </summary>
	/// <param name="_path">the path to the file</param>
	/// <param name="_array">the content of the file</param>
	/// <returns>A Config file all set with the corresponding settings</returns>
	ConfigFile* LoadSavedFile(const QString& _path,const QByteArray& _array);
	/// <summary>
	/// Create a structure keeping and area scrollbox and a configFile
	/// </summary>
	/// <param name="_path">the path to the ConfigFile to put in</param>
	/// <param name="_area"> the scrollboxArea to put in</param>
	/// <returns>the struct keeping both the scrollbox area and the configFile</returns>
	DisplayedSettings CreateDisplayedSettings(const QString& _path, QScrollArea* _area);
	/// <summary>
	/// return either the default value, or the current value in the file if the parameter exist and there is one
	/// </summary>
	/// <param name="_settingLine">the setting line splitted in two with the settingString , and the valueString</param>
	/// <param name="_file">the file to look for the value in</param>
	/// <returns>either the default value, or the current value in the file</returns>
	QString GetDefaultValue(const QStringList& _settingLine,const ConfigFile* _file);
	/// <summary>
	/// emited when the user stopped writting and the edit line was not foccused anymore
	/// </summary>
	/// <param name="_self">the editline modified</param>
	/// <param name="_text">the text the user put in</param>
	void OnLineEditChanged(class CustomLineEdit* _self, const QString& _text);
	/// <summary>
	/// change the value of the setting in _setting
	/// </summary>
	/// <param name="_file">the file the setting is in</param>
	/// <param name="_category">the category the setting to change is in</param>
	/// <param name="_setting">the setting to change the value of</param>
	/// <param name="_value">the new value to assing to the setting</param>
	void ChangeParameter(const QString& _file,const QString& _category,const QString& _setting, const QString& _value);

	template<typename T1,typename T2>
	bool Contains(const std::map<T1, T2>& _map, T1 _key)
	{
		for (const std::pair<T1, T2>& _pair : _map)
		{
			if (_pair.first == _key) return true;
		}
		return false;
	}
private slots:
	void on_applyButton_clicked();
};
