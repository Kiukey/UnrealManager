#include "ConfigFile.h"
#include "IOToolBox.h"
#include "qjsonarray.h"

ConfigFile::ConfigFile()
{
	QJsonArray _array;
	_array.push_back("bAuthorizeAutomaticWidgetVariableCreation = False");
	insert("[/Script/Engine.UserInterfaceSettings]", QJsonValue(_array));
}

ConfigFile::ConfigFile(const QString& _toParse)
{
	std::vector<QString> _lines = IOToolBox::GetAllLines(_toParse);
	if (_lines.size() <= 0) return;
	for (int i = 0; i < _lines.size(); i++)
	{
		const int _category = FindCategory(_lines, i);
		if (_category != -1)
		{
			int y = _category +1;
			const int _nextCategory = FindCategory(_lines, y);
			std::vector<QString> _debug = std::vector<QString>();
			QJsonArray _array = QJsonArray();
			for (y; y < _nextCategory; y++)
			{
				_array.push_back(_lines[y]);
				_debug.push_back(_lines[y]);
			}
			insert(_lines[i], _array);
			i = y-1;
		}
		else return;
	}
}

ConfigFile::ConfigFile(const QJsonObject& _toGet)
{
	QStringList _keys = _toGet.keys();
	for (const QString& _key : _keys)
	{
		insert(_key, _toGet.value(_key));
	}
}

bool ConfigFile::FindSetting(const QString& _settingName, QJsonValue& _retValue) const
{
	QStringList _keys = keys();
	for (QString _key : _keys)
	{
		if (FindSetting(_key, _settingName, _retValue)) return true;
	}
	return false;
}

bool ConfigFile::FindSetting(const QString& _categoryName,const QString& _settingName, QJsonValue& _retValue) const
{
	const QJsonArray& _array = value(_categoryName).toArray();
	if (_array.isEmpty()) return false;
	for (QJsonValue _value : _array)
	{
		if (_value.toString().contains(_settingName))
		{
			_retValue = _value;
			return true;
		}
	}
	return false;
}

bool ConfigFile::GetAllValuesFromCategory(const QString& _categoryName, QJsonArray& _array) const
{
	if (!contains(_categoryName)) return false;
	_array = value(_categoryName).toArray();
	return true;
}

QStringList ConfigFile::GetAllCategories() const
{
	return keys();
}

//bool ConfigFile::ChangeValueInCategory(const QString& _category, QString& _setting, bool _newValue)
//{
//	QString _test;
//	_test.
//	_test.arg(_newValue)
//}

int ConfigFile::FindCategory(std::vector<QString> _list, int _currentIndex)
{
	for (int i = _currentIndex; i < _list.size(); i++)
	{
		if (_list[i].contains("[/"))
			return i;
	}
	return -1;
}
