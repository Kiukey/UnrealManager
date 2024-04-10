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
	//TODO modules

public:

	UnrealProject(const QString& _path,const QString& _projectName);
	~UnrealProject();
#pragma region Get/Set
	QString GetProjectName() const;
	void SetProjectName(const QString& _name);
	QString GetPathToUprojectFile() const;
	void SetProjectPath(const QString& _path);
	QHBoxLayout* GetProjectWidgetLayout() const;
	std::vector<QString> GetConfigFilesName() const;
	QString GetConfigFolderPath() const;
	std::vector<ConfigFile*> GetConfigFiles();
	QString GetContentFolderPath() const;
	QString GetSourceFolderPath() const;
	QString GetProjectFolderPath() const;
#pragma endregion
	void LoadConfigFile();
	void UnloadConfigFiles();
	QString ToJson(bool _addCodeModule) const;


	void CreateProjectFiles(bool _cppModule);
signals:
	void OnSettingsLoaded(std::vector<ConfigFile*> _files);
	void OnNameChanged(const QString& _changedName);
	void OnPathChanged(const QString& _path);
private:
	std::map<QString, ConfigFile*> CreateConfigFiles();
	std::map<QString, ConfigFile*> GetExistingConfigFile();
	QString GetDefaultBuildCS() const;
private slots:
	void SetConfigFiles();
	void DeleteConfigFiles();
	
};

