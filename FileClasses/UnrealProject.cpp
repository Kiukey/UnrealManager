#include "UnrealProject.h"
#include "../IOToolBox.h"
#include "../UnrealProjectWidgets.h"
#include "ConfigFile.h"
#include "../Window/UnrealManager.h"

#define GAMEMODEBASE_CONTENT(projectName) "#include \"CoreMinimal.h\"\n#include \"GameFramework/GameModeBase.h\"\n\n#include \""+projectName+"GameModeBase.generated.h\"\n/*\n*\n*\n*/\nUCLASS()\nclass "+projectName.toUpper()+"_API A"+projectName+"GameModeBase : public AGameModeBase\n{\n    GENERATED_BODY()\n\n};"
#define TARGET_CONTENT(projectName) "// Copyright Epic Games, Inc. All Rights Reserved.\nusing UnrealBuildTool;\nusing System.Collections.Generic;\n\npublic class " + projectName+"Target : TargetRules\n{\npublic "+projectName+"Target(TargetInfo Target) : base(Target)\n{\n\nType = TargetType.Game;\nDefaultBuildSettings = BuildSettingsVersion.V2;\n\nIncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;\nExtraModuleNames.Add(\""+projectName+"\");\n}\n}\n"
#define TARGET_EDITOR_CONTENT(projectName) "// Copyright Epic Games, Inc. All Rights Reserved.\nusing UnrealBuildTool;\nusing System.Collections.Generic;\n\npublic class " + projectName+"EditorTarget : TargetRules\n{\npublic "+projectName+"EditorTarget(TargetInfo Target) : base(Target)\n{\n\nType = TargetType.Editor;\nDefaultBuildSettings = BuildSettingsVersion.V2;\n\nIncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_1;\nExtraModuleNames.Add(\""+projectName+"\");\n}\n}\n"


UnrealProject::UnrealProject(const QString& _path, const QString& _projectName)
{
	//constructor to create a project 
	const int _index = _path.lastIndexOf("/");
	QString _projectPath = _index >= 0 ? _path.first(_index) : _path;
	projectPath = _path;
	projectName = _projectName.split(".")[0];
	

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
	projectName = _name;
	const QString _folderPath = GetProjectFolderPath();
	SetProjectPath(_folderPath.first(_folderPath.lastIndexOf("/")));
	emit OnNameChanged(_name);
}

QString UnrealProject::GetPathToUprojectFile() const
{
	return projectPath;
}

void UnrealProject::SetProjectPath(const QString& _path)
{
	projectPath = _path + "/" + projectName+"/" + projectName+ ".uproject";
	for (std::pair _pair : configFiles)
	{
		_pair.second->SetPath(_path + "/Config");
	}
	emit OnPathChanged(projectPath);
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

QString UnrealProject::GetUprojectContent(bool _addCodeModule) const
{
	//TODO clean this up
	QString _toRet = "{\n";
	_toRet += "\"FileVersion\" : 3,\n";
	_toRet += "\"EngineAssociation\" : \"" + QString::number(UnrealManager::GetEngineVersion(), 'f',1) + "\",\n";
	_toRet += "\"Category\" : \"\",\n";
	_toRet += "\"Description\" : \"\",\n";
	if (_addCodeModule)
	{
		_toRet += "\"Modules\": [\n{\n\"Name\": \"" + projectName.split(".")[0] + "\",\n";
		_toRet += "\"Type\" : \"Runtime\",\n\"LoadingPhase\" : \"Default\",\n\"AdditionalDependencies\" : [\n\"Engine\",\n\"CoreUObject\"\n]\n}\n],\n";
	}
	_toRet += GetPluginsToJson();
	_toRet += "\n}";
	//_toRet += "\"Plugins\": [\n{\n\"Name\": \"ModelingToolsEditorMode\",\n\"Enabled\" : true,\n\"TargetAllowList\" : [\n\"Editor\"\n]\n}\n]\n}";
	return _toRet;
}

void UnrealProject::CreateProjectFiles(bool _cppModule)
{

	QString _projectName = projectName.split(".")[0];
	IOToolBox::CreateFolder(projectPath.first(projectPath.lastIndexOf("/")));
	IOToolBox::CreateFolder(GetContentFolderPath());
	//CreateConfigFolder
	IOToolBox::CreateFolder(GetConfigFolderPath());
	for (ConfigFile* _file : GetConfigFiles())
	{
		IOToolBox::CreateFile(_file->GetPath(), _file->ToIniFile());
	}
	IOToolBox::CreateFile(GetPathToUprojectFile(), GetUprojectContent(_cppModule));

	if (!_cppModule) return;

	const QString _sourceFolderPath = GetSourceFolderPath();
	const QString _classFolderPath = _sourceFolderPath + "/" + _projectName;
	IOToolBox::CreateFolder(_sourceFolderPath);
	IOToolBox::CreateFolder(_classFolderPath);

	//Create Module Class or something to add this one
#pragma region Create TargetFiles
	IOToolBox::CreateFile(_sourceFolderPath + "/" + _projectName + ".Target.cs", TARGET_CONTENT(_projectName));
	IOToolBox::CreateFile(_sourceFolderPath + "/" + _projectName + "Editor.Target.cs", TARGET_EDITOR_CONTENT(_projectName));
#pragma endregion
#pragma region Create Build.CS File
	IOToolBox::CreateFile(_classFolderPath + "/" + _projectName + ".Build.cs", GetDefaultBuildCS());
#pragma endregion
#pragma region Create GameModeBase
	IOToolBox::CreateFile(_classFolderPath + "/" + _projectName + "GameModeBase.h", GAMEMODEBASE_CONTENT(_projectName));
	IOToolBox::CreateFile(_classFolderPath + "/" + _projectName + "GameModeBase.cpp", "#include \"" + _projectName + "GameModeBase.h" + "\"");
#pragma endregion
#pragma region Create ProjectFiles
	IOToolBox::CreateFile(_classFolderPath + "/" + _projectName + ".h", "// Copyright Epic Games, Inc. All Rights Reserved.\n#pragma once\n#include \"CoreMinimal.h\"");
	IOToolBox::CreateFile(_classFolderPath+ "/" + _projectName + ".cpp", "// Copyright Epic Games, Inc. All Rights Reserved.\n#include \"" + _projectName + ".h\"\n#include \"Modules/ModuleManager.h\"\n\n		IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, " + _projectName + ", \"" + _projectName + "\"); ");
#pragma endregion
}

QString UnrealProject::GetContentFolderPath() const
{
	const int _index = projectPath.lastIndexOf("/");
	QString _toRet = "";
	if (_index >= 0)
		_toRet += projectPath.first(_index);
	_toRet += "/Content";
	return _toRet;
}

QString UnrealProject::GetSourceFolderPath() const
{
	const int _index = projectPath.lastIndexOf("/");
	QString _toRet = "";
	if (_index >= 0)
		_toRet += projectPath.first(_index);
	_toRet += "/Source";
	return _toRet;
}

QString UnrealProject::GetProjectFolderPath() const
{
	return projectPath.first(projectPath.lastIndexOf("/"));
}

void UnrealProject::SetPlugins(const QStringList& _list)
{
	pluginList.clear();
	for (const QString& _plugin : _list)
	{
		pluginList.push_back(_plugin);
	}
}

std::vector<QString> UnrealProject::GetActivatedPluginList() const
{
	return pluginList;
}

void UnrealProject::SetModules(const QStringList& _list)
{
	moduleList.clear();
	for (const QString& _module : _list)
	{
		moduleList.push_back(_module);
	}
}

std::vector<QString> UnrealProject::GetActivatedModulesList() const
{
	return moduleList;
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
		QStringList _file = _path.split("\\");
		_files.insert(std::pair(_file[_file.count()-1], new ConfigFile(IOToolBox::ReadFile(_path), _path)));
	}
	return _files;
}

QString UnrealProject::GetDefaultBuildCS() const
{
	QString _toRet = "// Copyright Epic Games, Inc. All Rights Reserved.\n";
	_toRet += "\nusing UnrealBuildTool;\n";
	_toRet += "\npublic class " + projectName + " : ModuleRules\n";
	_toRet += "{\n     public " + projectName + "(ReadOnlyTargetRules Target) : base(Target)\n";
	_toRet += "     {\n          PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;\n";
	_toRet += "          PublicDependencyModuleNames.AddRange(new string[]{"; //here add modules 
	_toRet +=  GetModulesToJson() + "});\n"; //to remove and put them dynamically
	_toRet += "          }\n}";
	return _toRet;
}

QString UnrealProject::GetPluginsToJson() const
{
	QString _toRet = "\"Plugins\" : [\n";
	for (const QString& _plugin : pluginList)
	{
		_toRet += "{\n\"Name\": \"" + _plugin + "\",\n\"Enabled\" : true\n},\n";
	}
	_toRet += "]\n";
	return _toRet;
}

QString UnrealProject::GetModulesToJson() const
{
	QString _toRet = "\"Core\",\"CoreUObject\", \"Engine\", \"InputCore\"";
	for (const QString& _module : moduleList)
	{
		_toRet += ",\"" + _module + "\"";
	}
	return _toRet;
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