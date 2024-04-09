#pragma once
#include <qstring.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <QHBoxLayout>

class UnrealProjectWidgets;
class ConfigFile;

class UnrealProject : public QObject
{
	Q_OBJECT
	UnrealProjectWidgets* widgets = nullptr;

	QString projectName = "";
	//path to project
	QString projectPath = "";
	//all the config files in Folder Config
	//std::vector<QString> configFilesPath = std::vector<QString>();

	std::map<QString,ConfigFile*> configFiles = std::map<QString, ConfigFile*>();
	//TODO plugins
public:

	UnrealProject(const QString& _path,const QString& _projectName);
	~UnrealProject();
	
	QString GetProjectName() const;
	void SetProjectName(const QString& _name);
	QString GetProjectPath() const;
	void SetProjectPath(const QString& _path);
	QHBoxLayout* GetProjectWidgetLayout() const;
	std::vector<QString> GetConfigFilesName() const;
	QString GetConfigFolderPath() const;
	std::vector<ConfigFile*> GetConfigFiles();
	void LoadConfigFile();
	void UnloadConfigFiles();
	QString ToJson() const;
signals:
	void OnSettingsLoaded(std::vector<ConfigFile*> _files);

private:
	std::map<QString, ConfigFile*> CreateConfigFiles();
	std::map<QString, ConfigFile*> GetExistingConfigFile();
private slots:
	void SetConfigFiles();
	void DeleteConfigFiles();
};

