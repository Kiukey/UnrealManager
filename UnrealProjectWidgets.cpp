#include "UnrealProjectWidgets.h"
#include "UnrealProject.h"
#include <stdlib.h>
#include "WindowProjectSettingsModifier.h"

UnrealProjectWidgets::UnrealProjectWidgets(UnrealProject* _project)
{
	owner = _project;

	layout = new QHBoxLayout();
	projectName = new QLabel(_project->GetProjectName());
	projectPath = new QLabel(_project->GetProjectPath());
	openProjectButton = new QPushButton("Open project");
	openProjectSettingsButton = new QPushButton("Open Project settings");
	layout->addWidget(projectName);
	layout->addWidget(projectPath);
	layout->addWidget(openProjectButton);
	layout->addWidget(openProjectSettingsButton);
	connect(openProjectButton, &QPushButton::clicked, this, &UnrealProjectWidgets::OpenProject);
	connect(openProjectSettingsButton, &QPushButton::clicked, this, &UnrealProjectWidgets::OpenProjectSettings);
	//openProjectButton->addAction()
}

UnrealProjectWidgets::~UnrealProjectWidgets()
{
	delete projectName;
	delete projectPath;
	delete openProjectButton;
	delete openProjectSettingsButton;
}

QHBoxLayout* UnrealProjectWidgets::GetWidget() const
{
	return layout;
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

void UnrealProjectWidgets::OpenProject()
{
	system(projectPath->text().toStdString().c_str());
}
