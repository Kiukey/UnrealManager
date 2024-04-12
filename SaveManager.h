#pragma once
#include <qobject.h>
#include <vector>

class UnrealProject;

class SaveManager : public QObject
{
	Q_OBJECT
	std::vector<QString> projectsPath = std::vector<QString>();
	QString unrealFolderPath = "";

public:
	SaveManager(QObject* _parent = nullptr);
	std::vector<QString> GetProjectsPath() const;
	QString GetUnrealFolderPath() const;
private:
	void CreateSave();
	void LoadSave();
	std::vector<QString> GetProjectsPathFromSave(class QJsonObject& _object) const;
	void Save();
	void AddProjectToSave(UnrealProject* _project);
	void ChangeUnrealFolderPath(const QString& _path);
};

