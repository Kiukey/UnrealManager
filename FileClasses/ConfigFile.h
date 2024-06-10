#pragma once
#include "qjsonobject.h"

class ConfigFile : public QJsonObject
{
	QString filePath = "";
	QString fileName = "";

public:
	ConfigFile(const QString& _filePath);
	ConfigFile(const QString& _toParse,const QString& _filePath);
	ConfigFile(const QJsonObject& _toGet, const QString& _filePath);

	bool IsCategoryExisting(const QString& _category) const;
	/// <summary>
	/// Check in itself to find the setting enterred and return true if found it, return value as QJsonValue as refParam
	/// </summary>
	/// <param name="_settingName">the name of the setting</param>
	/// <param name="_retValue">output where the settings if found will be</param>
	/// <returns>true if found false otherwise</returns>
	bool FindSetting(const QString& _settingName, QJsonValue& _retValue) const;
	/// <summary>
	/// Check in itself to find the setting enterred and return true if found it, return value as QJsonValue as refParam
	/// </summary>
	/// <param name="_settingName">the name of the setting</param>
	/// <param name="_retValue">output where the settings if found will be</param>
	/// <returns>true if found false otherwise</returns>
	bool FindSetting(const QString& _settingName, const QString& _categoryName, QJsonValue& _retValue) const;
	/// <summary>
	/// return all values registered in a category
	/// </summary>
	/// <param name="_categoryName"></param>
	/// <param name="_array"></param>
	/// <returns></returns>
	bool GetAllValuesFromCategory(const QString& _categoryName,QJsonArray& _array) const;
	QStringList GetAllCategories() const;
	int GetValueIndexInCategory(const QString& _categoryName, const QString& _setting);
	bool ChangeValueInCategory(const QString& _categoryName, const QString& _setting, const QString& _newValue);
	void CreateCategory(const QString& _category, const QString& _setting, const QString& _newValue);
	QString ToIniFile() const;
	QString ToString(const QString& _category) const;
	bool SaveFileInProject();
	QString GetPath() const;
	void SetPath(const QString& _path);
	bool AddSettingToCategory(const QString& _category, const QString& _setting, const QString& _value);
private:
	int FindCategory(std::vector<QString> _list, int _currentIndex);
	void ParseFile(const QString& _toParse);
};

