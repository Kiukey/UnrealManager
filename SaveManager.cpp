#include "FileClasses/UnrealProject.h"
#include "Window/UnrealManager.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include "SaveManager.h"
#include <qjsonarray.h>
#include "IOToolBox.h"
#define VAR_NAME(_var) #_var

SaveManager::SaveManager(QObject* _parent) : QObject(_parent)
{
	if (!IOToolBox::FileExist("Config/Save.save"))
		CreateSave();
	else
		LoadSave();

	UnrealManager* _mainWindow = (UnrealManager*)_parent;
	if (!_mainWindow) return;
	connect(_mainWindow, &UnrealManager::OnProjectCreated, this, &SaveManager::AddProjectToSave);
	connect(_mainWindow, &UnrealManager::OnProjectLocalized, this, &SaveManager::AddProjectToSave);
	connect(_mainWindow, &UnrealManager::OnUnrealFolderChanged, this, &SaveManager::ChangeUnrealFolderPath);
}

std::vector<QString> SaveManager::GetProjectsPath() const
{
	return projectsPath;
}

QString SaveManager::GetUnrealFolderPath() const
{
	return unrealFolderPath;
}

void SaveManager::CreateSave()
{
	if(!IOToolBox::IsFolderExisting("Config"))
		IOToolBox::CreateFolder("Config");
	IOToolBox::CreateFile("Config/Save.save");
}

void SaveManager::LoadSave()
{
	QJsonDocument _doc = QJsonDocument();
	QByteArray _array = QByteArray(IOToolBox::ReadFile("Config/Save.save").toStdString().c_str());
	QJsonObject _object = _doc.fromJson(_array).object();
	unrealFolderPath = _object.value(VAR_NAME(unrealFolderPath)).toString();
	projectsPath = GetProjectsPathFromSave(_object);
	Save();
}

std::vector<QString> SaveManager::GetProjectsPathFromSave(QJsonObject& _object) const
{
	std::vector<QString> _toRet = std::vector<QString>();
	QJsonArray _valueArray = _object.value(VAR_NAME(projectsPath)).toArray();
	for (const QJsonValue& _value : _valueArray)
	{
		if (!IOToolBox::FileExist(_value.toString())) continue;
		_toRet.push_back(_value.toString());
	}
	return _toRet;
}

void SaveManager::AddProjectToSave(UnrealProject* _project)
{
	projectsPath.push_back(_project->GetPathToUprojectFile());
	Save();
}

void SaveManager::ChangeUnrealFolderPath(const QString& _path)
{
	unrealFolderPath = _path;
	Save();
}

void SaveManager::Save()
{
	QJsonObject _save = QJsonObject();
	QJsonDocument _doc = QJsonDocument();
	_save.insert(VAR_NAME(unrealFolderPath), unrealFolderPath);
	QJsonArray _array = QJsonArray();
	for (const QString& _path : projectsPath)
		_array.push_back(_path);
	_save.insert(VAR_NAME(projectsPath), _array);
	_doc.setObject(_save);
	QString _toSave = _doc.toJson();
	IOToolBox::WriteInFile("Config/Save.save", _toSave, true);
}
