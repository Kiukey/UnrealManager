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
    /// <summary>
     /// either if the path is already registered in the projects or not
    /// </summary>
    bool IsProjectRegisterd(const QString& _path);
    /// <summary>
    /// create a template folder with all it's files in it if it doesn't exist
    /// </summary>
    void InitializeTemplateFolder();
private slots:
    /// <summary>
    /// open the localize menu
    /// </summary>
    void on_LocalizeButton_clicked();
    /// <summary>
    /// open the Create project menu
    /// </summary>
    void on_CreateButton_clicked();
};
