#pragma once
#include "qjsonobject.h"

class ConfigFile : public QJsonObject
{

public:
	ConfigFile();
	ConfigFile(const QString& _toParse);
	ConfigFile(const QJsonObject& _toGet);

	/// <summary>
	/// Check in itself to find the setting enterred and return true if found it, return value as QJsonValue as refParam
	/// </summary>
	/// <param name="_settingName">the name of the setting</param>
	/// <param name="_retValue">output where the settings if found will be</param>
	/// <returns>true if found false otherwise</returns>
	bool FindSetting(const QString& _settingName, QJsonValue& _retValue) const;
	bool FindSetting(const QString& _settingName, const QString& _categoryName, QJsonValue& _retValue) const;
	bool GetAllValuesFromCategory(const QString& _categoryName,QJsonArray& _array) const;
	QStringList GetAllCategories() const;
	//bool ChangeValueInCategory(const QString& _category, QString& _setting, bool _newValue);
private:

	int FindCategory(std::vector<QString> _list, int _currentIndex);
};

