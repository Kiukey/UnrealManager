#pragma once

#include <QDialog>
#include "ui_CreateProjectWindow.h"

class UnrealProject;

QT_BEGIN_NAMESPACE
namespace Ui { class CreateProjectWindowClass; };
QT_END_NAMESPACE

class CreateProjectWindow : public QDialog
{
	Q_OBJECT
private:
	Ui::CreateProjectWindowClass* ui;
	UnrealProject* project = nullptr;
	QString selectedPath = "C:";
public:
	CreateProjectWindow(QWidget *parent = nullptr);
	~CreateProjectWindow();
	/// <summary>
	/// return the newly created project
	/// </summary>
	/// <returns></returns>
	UnrealProject* CreateProject();
private:
	void OnProjectNameChanged();
	void OnChangePathClicked();
	void ChangeProjectSettings();
	void CreateButtonClicked();
	void OnManagePluginsClicked();
	void OnManageModulesClicked();
	void OnCppModuleCheckBoxStateChanged();
};
