#include "CreateProjectWindow.h"
#include "UnrealProject.h"
#include <qfiledialog.h>
#include "WindowProjectSettings.h"
#include "IOToolBox.h"
#include "ConfigFile.h"

CreateProjectWindow::CreateProjectWindow(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::CreateProjectWindowClass())
{
	ui->setupUi(this);

	ui->pathLineEdit->setText("C:/DefaultName");
	ui->projectNameEdit->setText("DefaultName");
	connect(ui->projectNameEdit, &QLineEdit::editingFinished, this, &CreateProjectWindow::OnProjectNameChanged);
	connect(ui->selectPathButton, &QPushButton::clicked, this, &CreateProjectWindow::OnChangePathClicked);
	connect(ui->settingsButton, &QPushButton::clicked, this, &CreateProjectWindow::ChangeProjectSettings);
	connect(ui->createButton, &QPushButton::clicked, this, &CreateProjectWindow::CreateButtonClicked);
}

CreateProjectWindow::~CreateProjectWindow()
{
	delete project;
	delete ui;
}

UnrealProject* CreateProjectWindow::CreateProject()
{
	project = new UnrealProject("C:/DefaultName/DefaultName.uproject","DefaultName");
	project->LoadConfigFile();
	setModal(true);
	exec(); // <- is here while window is open
	return project;
}

void CreateProjectWindow::OnProjectNameChanged()
{
	QString _projectName = ui->projectNameEdit->text();
	if (project->GetProjectName() == _projectName) return;
	project->SetProjectName(_projectName);
	ui->pathLineEdit->setText(selectedPath + "/" + _projectName);
}

void CreateProjectWindow::OnChangePathClicked()
{
	QString _newPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly);
	selectedPath = _newPath.isEmpty() ? selectedPath : _newPath;
	QString _projectName = ui->projectNameEdit->text();
	QString _toSet = selectedPath + (_projectName.isEmpty() ? "" : "/" + ui->projectNameEdit->text());
	ui->pathLineEdit->setText(_toSet);
	project->SetProjectPath(selectedPath);
}

void CreateProjectWindow::ChangeProjectSettings()
{
	WindowProjectSettings* _newWindow = new WindowProjectSettings(project);
	_newWindow->setModal(true);
	_newWindow->exec();

	delete _newWindow;
}

void CreateProjectWindow::CreateButtonClicked()
{
#pragma region Comments
	//content, Config,.uproject
	//QString _projectName = ui->projectNameEdit->text();
	//QString _toSet = selectedPath + (_projectName.isEmpty() ? "" : "/" + ui->projectNameEdit->text());
	//IOToolBox::CreateFolder(_toSet);
	//IOToolBox::CreateFile(project->GetPathToUprojectFile(), project->ToJson(ui->CodeModuleCheckBox->isChecked()));
	//IOToolBox::CreateFolder(project->GetContentFolderPath());
	//IOToolBox::CreateFolder(project->GetConfigFolderPath());

	////SourceFolder
	//IOToolBox::CreateFolder(project->GetSourceFolderPath());
	//IOToolBox::CreateTargetFile(project->GetSourceFolderPath() + "/" + _projectName +".Target.cs", _projectName,false);
	//IOToolBox::CreateTargetFile(project->GetSourceFolderPath() + "/" + _projectName + "Editor.Target.cs", _projectName,true);
	////Create Build.CS
	//QString _folderPath = project->GetSourceFolderPath() + "/" + _projectName;
	//IOToolBox::CreateFolder(_folderPath);
	//IOToolBox::CreateBuildCS(_folderPath + "/" + _projectName + ".Build.cs", _projectName);
	//IOToolBox::CreateGameModeBaseFiles(_folderPath, _projectName);
	//IOToolBox::CreateFile(_folderPath + "/" + _projectName + ".h", "// Copyright Epic Games, Inc. All Rights Reserved.\n#pragma once\n#include \"CoreMinimal.h\"");
	//IOToolBox::CreateFile(_folderPath + "/" + _projectName + ".cpp", "// Copyright Epic Games, Inc. All Rights Reserved.\n#include \"" + _projectName + ".h\"\n#include \"Modules/ModuleManager.h\"\n\n		IMPLEMENT_PRIMARY_GAME_MODULE(FDefaultGameModuleImpl, " + _projectName + ", \"" + _projectName + "\"); ");
	////
	//for (ConfigFile* _file : project->GetConfigFiles())
	//{
	//	IOToolBox::CreateFile(_file->GetPath(), _file->ToIniFile());
	//}
#pragma endregion
	project->CreateProjectFiles(ui->CodeModuleCheckBox->isChecked());
	close();
}
