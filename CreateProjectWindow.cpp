#include "CreateProjectWindow.h"
#include "UnrealProject.h"
#include <qfiledialog.h>
#include "WindowProjectSettings.h"

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
	project->SetProjectPath(_toSet);
}

void CreateProjectWindow::ChangeProjectSettings()
{
	WindowProjectSettings* _newWindow = new WindowProjectSettings(project);
	_newWindow->setModal(true);
	_newWindow->exec();

	delete _newWindow;
}
