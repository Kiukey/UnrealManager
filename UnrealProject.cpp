#include "UnrealProject.h"
#include "IOToolBox.h"
#include "UnrealProjectWidgets.h"
#include "ConfigFile.h"

UnrealProject::UnrealProject(const QString& _path, const QString& _projectName)
{
	//constructor to create a project 
	QString _projectPath = _path.first(_path.lastIndexOf("/"));
	projectPath = _path;
	projectName = _projectName;

	//if config folder exist Get the .ini files 
	//if not create them and stock them
	QString _projectConfigFolder = _projectPath.append("/Config");
	widgets = new UnrealProjectWidgets(this);
	widgets->GetOpenProjectSettingsButton()->connect(widgets->GetOpenProjectSettingsButton(), &QPushButton::clicked, this, &UnrealProject::LoadConfigFile);
	//connect(widgets, &UnrealProjectWidgets::OnProjectSettingsWindowClosed, this, &UnrealProject::DeleteConfigFiles);
}

UnrealProject::~UnrealProject()
{
	if (configFiles.size() > 0)
		DeleteConfigFiles();
	delete widgets;
}

QString UnrealProject::GetProjectName() const
{
	return projectName;
}

QString UnrealProject::GetProjectPath() const
{
	return projectPath;
}

QHBoxLayout* UnrealProject::GetProjectWidgetLayout() const
{
	if (!widgets) return nullptr;
	return widgets->GetWidget();
}

QString UnrealProject::GetConfigFolderPath() const
{
	QString _toRet = projectPath.first(projectPath.lastIndexOf("/"));
	_toRet += "/Config";
	return _toRet;
}

std::vector<ConfigFile*> UnrealProject::GetConfigFiles()
{
	std::vector<ConfigFile*> _toRet = std::vector<ConfigFile*>();
	for (std::pair _pair : configFiles)
		_toRet.push_back(_pair.second);
	return _toRet;
}

void UnrealProject::LoadConfigFile()
{
	SetConfigFiles();
	emit OnSettingsLoaded(GetConfigFiles());
}

std::map<QString,ConfigFile*> UnrealProject::CreateConfigFiles()
{
	std::map<QString, ConfigFile*>_files = std::map<QString, ConfigFile*>();
	_files.insert(std::pair("DefaultEditor", new ConfigFile(GetConfigFolderPath() + "/DefaultEditor.ini")));
	_files.insert(std::pair("DefaultEngine", new ConfigFile(GetConfigFolderPath() + "/DefaultEngine.ini")));
	_files.insert(std::pair("DefaultGame", new ConfigFile(GetConfigFolderPath() + "/DefaultGame.ini")));
	_files.insert(std::pair("DefaultInput", new ConfigFile(GetConfigFolderPath() + "/DefaultInput.ini")));
	return _files;
}

std::map<QString, ConfigFile*> UnrealProject::GetExistingConfigFile()
{
	std::map<QString, ConfigFile*>_files = std::map<QString, ConfigFile*>();
	std::vector<QString> _filePaths = IOToolBox::GetFilesInFolder(GetConfigFolderPath());
	for (const QString& _path : _filePaths)
	{
		QString _fileName = _path.last(_path.lastIndexOf("/"));
		_files.insert(std::pair(_fileName, new ConfigFile(IOToolBox::ReadFile(_path), _path)));
	}
	return _files;
}

void UnrealProject::DeleteConfigFiles()
{
	std::vector<ConfigFile*> _toDel = GetConfigFiles();
	for (ConfigFile* _file : _toDel)
		delete _file;

	configFiles.clear();
}

void UnrealProject::SetConfigFiles()
{
	configFiles = IOToolBox::IsFolderExisting(GetConfigFolderPath()) ? GetExistingConfigFile() : CreateConfigFiles();
}

std::vector<QString> UnrealProject::GetConfigFilesName() const
{
	std::vector<QString> _toRet = std::vector<QString>();
	for (std::pair _pair : configFiles)
		_toRet.push_back(_pair.first);
	return _toRet;
}