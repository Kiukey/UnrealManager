#pragma once
#include <qstring.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <QHBoxLayout>

class UnrealProjectWidgets;

class UnrealProject
{

	UnrealProjectWidgets* widgets = nullptr;

	QString projectName = "";
	//path to project
	QString projectPath = "";
	//all the config files in Folder Config
	std::vector<QString> configFilesPath = std::vector<QString>();
	//TODO plugins
public:

	UnrealProject(const QString& _path,const QString& _projectName);
	~UnrealProject();
	
	QString GetProjectName() const;
	QString GetProjectPath() const;
	QHBoxLayout* GetProjectWidgetLayout() const;
	std::vector<QString> GetIniFilePaths();
private:
	void CreateConfigFiles(const QString& _path);
};

