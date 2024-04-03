#include "UnrealProject.h"
#include "IOToolBox.h"
#include "UnrealProjectWidgets.h"

UnrealProject::UnrealProject(const QString& _path, const QString& _projectName)
{
	//constructor to create a project 
	QString _projectPath = _path.first(_path.lastIndexOf("/"));
	projectPath = _path;
	projectName = _projectName;


	//if config folder exist Get the .ini files 
	//if not create them and stock them
	QString _projectConfigFolder = _projectPath.append("/Config");
	if (IOToolBox::IsFolderExisting(_projectConfigFolder))
		configFilesPath = IOToolBox::GetFilesInFolder(_projectConfigFolder);
	//
	widgets = new UnrealProjectWidgets(this);
}

UnrealProject::~UnrealProject()
{
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

std::vector<QString> UnrealProject::GetIniFilePaths()
{
	return configFilesPath;
}

void UnrealProject::CreateConfigFiles(const QString& _path)
{
	IOToolBox::CreateFolder(_path);
	IOToolBox::WriteInFile(_path + "DefaultEditor.ini","");
	IOToolBox::WriteInFile(_path + "DefaultEngine.ini","");
	IOToolBox::WriteInFile(_path + "DefaultGame.ini","");
	IOToolBox::WriteInFile(_path + "DefaultInput.ini","");
}


