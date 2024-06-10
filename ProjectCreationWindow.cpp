#include "ProjectCreationWindow.h"
#include "../FileClasses/UnrealProject.h"
#include <qfiledialog.h>
#include "WindowProjectSettings.h"
#include "../IOToolBox.h"
#include "../FileClasses/ConfigFile.h"
#include "ManagerExternalFileWIndow.h"

ProjectCreationWindow::ProjectCreationWindow(QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::ProjectCreationWindowClass())
{
	ui.setupUi(this);
	ui.pathLineEdit->setText("C:/DefaultName");
	ui.projectNameEdit->setText("DefaultName");
	connect(ui.projectNameEdit, &QLineEdit::editingFinished, this, &ProjectCreationWindow::OnProjectNameChanged);
	connect(ui.selectPathButton, &QPushButton::clicked, this, &ProjectCreationWindow::OnChangePathClicked);
	connect(ui.settingsButton, &QPushButton::clicked, this, &ProjectCreationWindow::ChangeProjectSettings);
	connect(ui.createButton, &QPushButton::clicked, this, &ProjectCreationWindow::CreateButtonClicked);
	connect(ui.managePluginsButton, &QPushButton::clicked, this, &ProjectCreationWindow::OnManagePluginsClicked);
	connect(ui.manageModulesButton, &QPushButton::clicked, this, &ProjectCreationWindow::OnManageModulesClicked);
	connect(ui.CodeModuleCheckBox, &QCheckBox::stateChanged, this, &ProjectCreationWindow::OnCppModuleCheckBoxStateChanged);
	ui.manageModulesButton->setEnabled(ui.CodeModuleCheckBox->isChecked());
}

ProjectCreationWindow::~ProjectCreationWindow()
{
	//delete ui;
}

UnrealProject* ProjectCreationWindow::CreateProject()
{
	project = new UnrealProject("C:/DefaultName/DefaultName.uproject", "DefaultName");
	project->LoadConfigFile();
	setModal(true);
	exec(); // <- is here while window is open

	if (ui->startProjectCheckBox->isChecked())
		system(project->GetPathToUprojectFile().toStdString().c_str());

	return project;
}

void ProjectCreationWindow::OnProjectNameChanged()
{
	QString _projectName = ui->projectNameEdit->text();
	if (project->GetProjectName() == _projectName) return;
	project->SetProjectName(_projectName);
	ui->pathLineEdit->setText(selectedPath + "/" + _projectName);
}

void ProjectCreationWindow::OnChangePathClicked()
{
	QString _newPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home", QFileDialog::ShowDirsOnly);
	selectedPath = _newPath.isEmpty() ? selectedPath : _newPath;
	QString _projectName = ui->projectNameEdit->text();
	QString _toSet = selectedPath + (_projectName.isEmpty() ? "" : "/" + ui->projectNameEdit->text());
	ui->pathLineEdit->setText(_toSet);
	project->SetProjectPath(selectedPath);
}

void ProjectCreationWindow::ChangeProjectSettings()
{
	WindowProjectSettings* _newWindow = new WindowProjectSettings(project);
	_newWindow->setModal(true);
	_newWindow->exec();

	delete _newWindow;
}

void ProjectCreationWindow::CreateButtonClicked()
{
	project->CreateProjectFiles(ui.CodeModuleCheckBox->isChecked());
	accept();
}

void ProjectCreationWindow::OnManagePluginsClicked()
{
	//here create a window to manage the plugins of a project
	ManagerExternalFileWIndow* _window = new ManagerExternalFileWIndow("Plugins", project->GetActivatedPluginList());
	QStringList _list = _window->ShowSettingsWindow();
	project->SetPlugins(_list);
	delete _window;
}

void ProjectCreationWindow::OnManageModulesClicked()
{
	ManagerExternalFileWIndow* _window = new ManagerExternalFileWIndow("Modules", project->GetActivatedModulesList());
	QStringList _list = _window->ShowSettingsWindow();
	project->SetModules(_list);
	delete _window;
}

void ProjectCreationWindow::OnCppModuleCheckBoxStateChanged()
{
	ui.manageModulesButton->setEnabled(ui.CodeModuleCheckBox->isChecked());
}

void ProjectCreationWindow::reject()
{
	delete project;
	project = nullptr;
	close();
}
