#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_UnrealManager.h"


class IOToolBox;
class UnrealProject;

class UnrealManager : public QMainWindow
{
    Q_OBJECT
private:
    Ui::UnrealManagerClass ui;
    /*stocking projects to be able to delete them a the end of the program*/
    std::vector<UnrealProject*> projects = std::vector<UnrealProject*>();
public:
    UnrealManager(QWidget *parent = nullptr);
    ~UnrealManager();


private:
    /*add the project to the ui*/
    void AddProject(UnrealProject* _project);
    /*either if the path is already registered in the projects or not*/
    bool IsProjectRegisterd(const QString& _path);

    void InitializeTemplateFolder();
private slots:
    /*the localize method*/
    void on_ActionFindProject_triggered();
};
