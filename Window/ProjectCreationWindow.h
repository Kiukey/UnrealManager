#pragma once

#include <QDialog>
#include "ui_ProjectCreationWindow.h"

class UnrealProject;

class ProjectCreationWindow : public QDialog
{
	Q_OBJECT

	UnrealProject* project = nullptr;
	QString selectedPath = "C:";

public:
	ProjectCreationWindow(QWidget *parent = nullptr);
	~ProjectCreationWindow();
	UnrealProject* CreateProject();
private:
	Ui::ProjectCreationWindowClass ui;

	void OnProjectNameChanged();
	void OnChangePathClicked();
	void ChangeProjectSettings();
	void CreateButtonClicked();
	void OnManagePluginsClicked();
	void OnManageModulesClicked();
	void OnCppModuleCheckBoxStateChanged();
	void OnReject();
	void DeInitializeWindow();
protected:
	virtual void reject() override;
};
