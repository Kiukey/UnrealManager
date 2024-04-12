#include "CreateProjectWindow.h"
#include "../FileClasses/UnrealProject.h"
#include <qfiledialog.h>
#include "WindowProjectSettings.h"
#include "../IOToolBox.h"
#include "../FileClasses/ConfigFile.h"
#include "ManagerExternalFileWIndow.h"

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
	connect(ui->managePluginsButton, &QPushButton::clicked, this, &CreateProjectWindow::OnManagePluginsClicked);
	connect(ui->manageModulesButton, &QPushButton::clicked, this, &CreateProjectWindow::OnManageModulesClicked);
	connect(ui->CodeModuleCheckBox, &QCheckBox::stateChanged, this, &CreateProjectWindow::OnCppModuleCheckBoxStateChanged);
	ui->manageModulesButton->setEnabled(ui->CodeModuleCheckBox->isChecked());
}

CreateProjectWindow::~CreateProjectWindow()
{
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
	project->CreateProjectFiles(ui->CodeModuleCheckBox->isChecked());
	accept();
}

void CreateProjectWindow::OnManagePluginsClicked()
{
	//here create a window to manage the plugins of a project
	ManagerExternalFileWIndow* _window = new ManagerExternalFileWIndow("Plugins", project->GetActivatedPluginList());
	QStringList _list = _window->ShowSettingsWindow();
	project->SetPlugins(_list);
	delete _window;
}

void CreateProjectWindow::OnManageModulesClicked()
{
	ManagerExternalFileWIndow* _window = new ManagerExternalFileWIndow("Modules", project->GetActivatedModulesList());
	QStringList _list = _window->ShowSettingsWindow();
	project->SetModules(_list);
	delete _window;
}

void CreateProjectWindow::OnCppModuleCheckBoxStateChanged()
{
	ui->manageModulesButton->setEnabled(ui->CodeModuleCheckBox->isChecked());
}

void CreateProjectWindow::reject()
{
	delete project;
	project = nullptr;
	close();
}
