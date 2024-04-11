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

UnrealManager::UnrealManager(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    InitializeTemplateFolder();
}

UnrealManager::~UnrealManager()
{
    const int _size = projects.size();

    if (_size == 0) return;
    for (int i = 0; i < _size; i++)
    {
        delete projects[i];
    }
    projects.clear();
}

void UnrealManager::AddProject(UnrealProject* _project)
{
    if (!_project) return;
    QWidget* _widget = new QWidget();
    _widget->setLayout(_project->GetProjectWidgetLayout());
    //ui.ProjectsList->widget()->layout()->addWidget(_widget);
    const int _widgetIndex = ui.projectsPages->count()-1;
    const int _count = ui.projectsPages->widget(_widgetIndex)->layout()->count();
    if (_count < maxProjectPerPage)
    {
        QWidget* _widgetToAdd = (QWidget*)ui.projectsPages->children().last();
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
    //TODO Add widget to page widget (stackWidget) corresponding to number of projects max
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

void UnrealManager::InitializeTemplateFolder()
{
    //TODO Upgrade this so that it detect if one file is missing and regen it , and Add settings at first 
    if (IOToolBox::IsFolderExisting("Templates")) return;
    IOToolBox::CreateFolder("Templates");
    IOToolBox::CreateFile("Templates/DefaultEditorTemplate.ini");
    IOToolBox::CreateFile("Templates/DefaultEngineTemplate.ini");
    IOToolBox::CreateFile("Templates/DefaultGameTemplate.ini");
    IOToolBox::CreateFile("Templates/DefaultInputTemplate.ini");
    IOToolBox::CreateFile("Templates/Plugins&ModulesTemplate.ini");
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
}

void UnrealManager::on_CreateButton_clicked()
{
    CreateProjectWindow* _window = new CreateProjectWindow();
    UnrealProject* _project = _window->CreateProject();
    AddProject(_project);
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
