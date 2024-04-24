#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_UnrealManager.h"


class IOToolBox;
class UnrealProject;
class SaveManager;

class UnrealManager : public QMainWindow
{
    Q_OBJECT
private:
    Ui::UnrealManagerClass ui;
    /*stocking projects to be able to delete them a the end of the program*/

    SaveManager* saveManager = nullptr;
    std::vector<UnrealProject*> projects = std::vector<UnrealProject*>();
    std::vector<UnrealProject*> loadedProjects = std::vector<UnrealProject*>();
    int maxProjectPerPage = 5;
    static inline float engineVersion = 5.2;
public:
    UnrealManager(QWidget *parent = nullptr);
    ~UnrealManager();

    static float GetEngineVersion();
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
    void InitializeConfigFolder();

    void AddLoadedProject(UnrealProject* _project);
    void AddLoadedProjects(const int _from);
    void UnloadCurrentProjects();
    void LoadSave();
    void SetUnrealPath(const QString& _path);
    QWidget* CreateNewPage();
private slots:
    /// <summary>
    /// open the localize menu
    /// </summary>
    void on_LocalizeButton_clicked();
    /// <summary>
    /// open the Create project menu
    /// </summary>
    void on_CreateButton_clicked();

    void on_nextPage_clicked();
    void on_previousPage_clicked();

    void on_localizeUnrealFolderButton_clicked();
signals:

    void OnProjectLocalized(UnrealProject* _project);
    void OnProjectCreated(UnrealProject* _project);
    void OnUnrealFolderChanged(const QString& _path);
};
