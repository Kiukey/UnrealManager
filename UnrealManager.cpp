#include "UnrealManager.h"
#include "IOToolBox.h"
#include <QJsonDocument>
#include <QFileDialog>
#include "UnrealProject.h"
#include <qlabel.h>
#include <qerrormessage.h>
#include "ConfigFile.h"
#include "UnrealProjectWidgets.h"

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
    ui.ProjectsList->widget()->layout()->addWidget(_widget);
    //if (ui.projectsPages->children().last()->children().count() < 1)
    //{
    //    QWidget* _widgetToAdd = (QWidget*)ui.projectsPages->children().last();
    //    _widgetToAdd->layout()->addWidget(_widget);
    //}
    //else
    //{
    //    QWidget* _newPage = new QWidget();
    //    ui.projectsPages->addWidget(_newPage);
    //    //_newPage->
    //}
    //TODO Add widget to page widget (stackWidget) corresponding to number of projects max
    projects.push_back(_project);
}

bool UnrealManager::IsProjectRegisterd(const QString& _path)
{
    for (UnrealProject* _project : projects)
    {
        if (_project->GetProjectPath() == _path)
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

}