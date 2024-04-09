#include "UnrealProject.h"
#include "IOToolBox.h"
#include "UnrealProjectWidgets.h"
#include "ConfigFile.h"

UnrealProject::UnrealProject(const QString& _path, const QString& _projectName)
{
	//constructor to create a project 
	const int _index = _path.lastIndexOf("/");
	QString _projectPath = _index >= 0 ? _path.first(_index) : _path;
	projectPath = _path;
	projectName = _projectName;
	//if config folder exist Get the .ini files 
	//if not create them and stock them
	QString _projectConfigFolder = _projectPath.append("/Config");
	widgets = new UnrealProjectWidgets(this);
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

void UnrealProject::SetProjectName(const QString& _name)
{
	projectName = "";
}

QString UnrealProject::GetProjectPath() const
{
	return projectPath;
}

void UnrealProject::SetProjectPath(const QString& _path)
{
	projectPath = _path;
	for (std::pair _pair : configFiles)
	{
		_pair.second->SetPath(_path + "/Config");
	}
}

QHBoxLayout* UnrealProject::GetProjectWidgetLayout() const
{
	if (!widgets) return nullptr;
	return widgets->GetWidget();
}

QString UnrealProject::GetConfigFolderPath() const
{
	//D:Folder/UnrealProject.uproject
	const int _index = projectPath.lastIndexOf("/");
	QString _toRet = "";
	if(_index >= 0)
		_toRet += projectPath.first(_index);
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

void UnrealProject::UnloadConfigFiles()
{
	DeleteConfigFiles();
}

QString UnrealProject::ToJson() const
{
	QString _toRet = "{\n";
	_toRet += "\"EngineAssociation\" : \"5.2\",\n";
	_toRet += "\"Plugins\": [\n{\n\"Name\": \"ModelingToolsEditorMode\",\n\"Enabled\" : true,\n\"TargetAllowList\" : [\n\"Editor\"\n]\n}\n]\n}";
	return _toRet;
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
	//Todo fix keys not good 
	configFiles = IOToolBox::IsFolderExisting(GetConfigFolderPath()) ? GetExistingConfigFile() : CreateConfigFiles();
}

std::vector<QString> UnrealProject::GetConfigFilesName() const
{
	std::vector<QString> _toRet = std::vector<QString>();
	for (std::pair _pair : configFiles)
		_toRet.push_back(_pair.first);
	return _toRet;
}