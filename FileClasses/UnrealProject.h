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
	std::vector<QString> pluginList = std::vector<QString>();
	//TODO modules
	std::vector<QString> moduleList = std::vector<QString>();
public:

	UnrealProject(const QString& _path,const QString& _projectName);
	~UnrealProject();
#pragma region Get/Set
	QString GetProjectName() const;
	void SetProjectName(const QString& _name);
	QString GetPathToUprojectFile() const;
	void SetProjectPath(const QString& _path);
	QFrame* GetProjectWidgetFrame() const;
	std::vector<QString> GetConfigFilesName() const;
	QString GetConfigFolderPath() const;
	std::vector<ConfigFile*> GetConfigFiles();
	QString GetContentFolderPath() const;
	QString GetSourceFolderPath() const;
	QString GetProjectFolderPath() const;
	void SetPlugins(const QStringList& _list);
	std::vector<QString> GetActivatedPluginList() const;

	void SetModules(const QStringList& _list);
	std::vector<QString> GetActivatedModulesList() const;
#pragma endregion
	void LoadConfigFile();
	void UnloadConfigFiles();


	void CreateProjectFiles(bool _cppModule);
signals:
	void OnSettingsLoaded(std::vector<ConfigFile*> _files);
	void OnNameChanged(const QString& _changedName);
	void OnPathChanged(const QString& _path);
private:
	std::map<QString, ConfigFile*> CreateConfigFiles();
	std::map<QString, ConfigFile*> GetExistingConfigFile();
	QString GetUprojectContent(bool _addCodeModule) const;

	QString GetDefaultBuildCS() const;
	QString GetPluginsToJson() const;
	QString GetModulesToJson() const;
private slots:
	void SetConfigFiles();
	void DeleteConfigFiles();
	
};

