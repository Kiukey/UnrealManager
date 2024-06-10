#include "UnrealProjectWidgets.h"
#include "FileClasses/UnrealProject.h"
#include <stdlib.h>
#include "Window/WindowProjectSettingsModifier.h"

UnrealProjectWidgets::UnrealProjectWidgets(UnrealProject* _project)
{
	owner = _project;

	layout = new QHBoxLayout();
	frame = new QFrame();
	projectName = new QLabel(_project->GetProjectName());
	projectPath = new QLabel(_project->GetPathToUprojectFile());
	openProjectButton = new QPushButton("Open project");
	openProjectSettingsButton = new QPushButton("Open Project settings");
	frame->setLayout(layout);
	layout->addWidget(projectName);
	layout->addWidget(projectPath);
	layout->addWidget(openProjectButton);
	layout->addWidget(openProjectSettingsButton);
	connect(openProjectButton, &QPushButton::clicked, this, &UnrealProjectWidgets::OpenProject);
	connect(openProjectSettingsButton, &QPushButton::clicked, this, &UnrealProjectWidgets::OpenProjectSettings);
	connect(owner, &UnrealProject::OnNameChanged, this, &UnrealProjectWidgets::OnNameChanged);
	connect(owner, &UnrealProject::OnPathChanged, this, &UnrealProjectWidgets::OnPathChanged);
	//openProjectButton->addAction()
}

UnrealProjectWidgets::~UnrealProjectWidgets()
{
	delete projectName;
	delete projectPath;
	delete openProjectButton;
	delete openProjectSettingsButton;
	delete layout;
	delete frame;
}

QFrame* UnrealProjectWidgets::GetWidget() const
{
	return frame;
}

QPushButton* UnrealProjectWidgets::GetOpenProjectSettingsButton()
{
	return openProjectSettingsButton;
}

void UnrealProjectWidgets::OpenProjectSettings()
{
	WindowProjectSettingsModifier* _window = new WindowProjectSettingsModifier(owner);
	_window->setModal(true);
	_window->exec();
	emit OnProjectSettingsWindowClosed();
	delete _window;
}

void UnrealProjectWidgets::OnNameChanged(const QString& _name)
{
	projectName->setText(_name);
}

void UnrealProjectWidgets::OnPathChanged(const QString& _path)
{
	projectPath->setText(_path);
}

void UnrealProjectWidgets::OpenProject()
{
	system(projectPath->text().toStdString().c_str());
}
