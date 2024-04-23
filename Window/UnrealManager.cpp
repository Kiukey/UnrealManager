#include "UnrealManager.h"
#include "../IOToolBox.h"
#include <QJsonDocument>
#include <QFileDialog>
#include "../FileClasses/UnrealProject.h"
#include <qlabel.h>
#include <qerrormessage.h>
#include "../FileClasses/ConfigFile.h"
#include "../UnrealProjectWidgets.h"
#include "CreateProjectWindow.h"
#include "../SaveManager.h"


UnrealManager::UnrealManager(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    LoadSave();
    SetUnrealPath(saveManager->GetUnrealFolderPath());
    InitializeConfigFolder();
}

UnrealManager::~UnrealManager()
{
    
    delete saveManager;
    const int _size = projects.size();

    if (_size == 0) return;
    for (int i = 0; i < _size; i++)
    {
        delete projects[i];
    }
    projects.clear();
}

float UnrealManager::GetEngineVersion()
{
    return engineVersion;
}

void UnrealManager::AddProject(UnrealProject* _project)
{
    if (!_project) return;
    QWidget* _widget = _project->GetProjectWidgetFrame();
  /*  _widget->setLayout();*/
    const int _widgetIndex = ui.projectsPages->count()-1;
    const int _count = ui.projectsPages->widget(_widgetIndex)->layout()->count();
    if (_count < maxProjectPerPage)
    {
        QWidget* _widgetToAdd = (QWidget*)ui.projectsPages->widget(ui.projectsPages->count() - 1);
        if (_widgetIndex == ui.projectsPages->currentIndex())
            AddLoadedProject(_project);
        _widgetToAdd->layout()->addWidget(_widget);
    }
    else
    {
        QWidget* _newPage = new QWidget();
        QVBoxLayout* _layout = new QVBoxLayout(_newPage);
        _newPage->setLayout(_layout);
        ui.projectsPages->insertWidget(_widgetIndex+1,_newPage);
        if (!_newPage) return;
        _newPage->layout()->addWidget(_widget);
    }
    projects.push_back(_project);
}

bool UnrealManager::IsProjectRegisterd(const QString& _path)
{
    for (UnrealProject* _project : projects)
    {
        if (_project->GetPathToUprojectFile() == _path)
            return true;
    }
    return false;
}

void UnrealManager::InitializeConfigFolder()
{
    //TODO Upgrade this so that it detect if one file is missing and regen it , and Add settings at first 
    if (!IOToolBox::IsFolderExisting("Config")) return;
        IOToolBox::CreateFolder("Config");
    if(!IOToolBox::FileExist("Config/DefaultEditorTemplate.ini"))
        IOToolBox::CreateFile("Config/DefaultEditorTemplate.ini");
    if (!IOToolBox::FileExist("Config/DefaultEngineTemplate.ini"))
        IOToolBox::CreateFile("Config/DefaultEngineTemplate.ini");
    if (!IOToolBox::FileExist("Config/DefaultGameTemplate.ini"))
        IOToolBox::CreateFile("Config/DefaultGameTemplate.ini");
    if (!IOToolBox::FileExist("Config/DefaultInputTemplate.ini"))
        IOToolBox::CreateFile("Config/DefaultInputTemplate.ini");
    if (!IOToolBox::FileExist("Config/Plugins&ModulesTemplate.ini"))
        IOToolBox::CreateFile("Config/Plugins&ModulesTemplate.ini");
}

void UnrealManager::AddLoadedProject(UnrealProject* _project)
{
    if (!_project) return;
    loadedProjects.push_back(_project);
    _project->LoadConfigFile();
}

void UnrealManager::AddLoadedProjects(const int _from)
{
    const int _to = _from + maxProjectPerPage;
    for (int i = _from; i < _to; i++)
    {
        if (i >= projects.size()) return;
        AddLoadedProject(projects[i]);
    }
}

void UnrealManager::UnloadCurrentProjects()
{
    for (UnrealProject* _project : loadedProjects)
    {
        _project->UnloadConfigFiles();
    }
    loadedProjects.clear();
}

void UnrealManager::LoadSave()
{
    saveManager = new SaveManager(this);
    std::vector<QString> _paths = saveManager->GetProjectsPath();
    for (QString _path : _paths)
        AddProject(new UnrealProject(_path, _path.right(_path.count() - (_path.lastIndexOf("/")+1))));
}

void UnrealManager::SetUnrealPath(const QString& _path)
{
    if (_path.isEmpty()) return;
    ui.UnrealVersionLineEdit->setText(_path);
    QStringList _list = _path.split("/");
    engineVersion = _list[_list.count() - 1].split("_")[1].toFloat();
}

void UnrealManager::on_LocalizeButton_clicked()
{
    QString _extension = "*.uproject";
    QUrl _pathToProject = QFileDialog::getOpenFileUrl(this, QString(), QUrl(), _extension, &_extension);
    if (_pathToProject.isEmpty()) return;
    else if (IsProjectRegisterd(_pathToProject.toLocalFile()))
    {
        QErrorMessage _message;
        _message.showMessage("Project already registered");
        _message.exec();
        return;
    }
    UnrealProject* _project = new UnrealProject(_pathToProject.toLocalFile(),_pathToProject.fileName());
    AddProject(_project);
    emit OnProjectLocalized(_project);
}

void UnrealManager::on_CreateButton_clicked()
{
    CreateProjectWindow* _window = new CreateProjectWindow(this);
    UnrealProject* _project = _window->CreateProject();
    if (!_project) return;
    AddProject(_project);
    emit OnProjectCreated(_project);
}

void UnrealManager::on_nextPage_clicked()
{
    const int _current = ui.projectsPages->currentIndex();
    const int _nextPage = _current + 1;
    const int _numberOfPage = ui.projectsPages->count();
    if (_nextPage > ui.projectsPages->count()-1) return;
    ui.projectsPages->setCurrentIndex(_nextPage);
    UnloadCurrentProjects();
    AddLoadedProjects(_nextPage * maxProjectPerPage);
}

void UnrealManager::on_previousPage_clicked()
{
    const int _current = ui.projectsPages->currentIndex();
    const int _previousPage = _current - 1;
    if (_previousPage < 0) return;
    ui.projectsPages->setCurrentIndex(_previousPage);
    UnloadCurrentProjects();
    AddLoadedProjects(_previousPage*maxProjectPerPage);
}

void UnrealManager::on_localizeUnrealFolderButton_clicked()
{
    QString _newPath = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/C:", QFileDialog::ShowDirsOnly);
    if (_newPath.isEmpty()) return;
    emit OnUnrealFolderChanged(_newPath);
    SetUnrealPath(_newPath);
}