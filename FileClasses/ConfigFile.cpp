#include "ConfigFile.h"
#include "../IOToolBox.h"
#include "qjsonarray.h"

ConfigFile::ConfigFile(const QString& _filePath)
{
	filePath = _filePath;
	QStringList _list = _filePath.split("/");
	fileName = _list[_list.count()-1];
}

ConfigFile::ConfigFile(const QString& _toParse,const QString& _filePath)
{
	ParseFile(_toParse);
	filePath = _filePath;
	QStringList _list = _filePath.split("/");
	fileName = _list[_list.count() - 1];
}

ConfigFile::ConfigFile(const QJsonObject& _toGet, const QString& _filePath)
{
	filePath = _filePath;
	QStringList _list = _filePath.split("/");
	fileName = _list[_list.count() - 1];
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
		QStringList _valueString = _value.toString().split("=", Qt::SplitBehavior::enum_type::SkipEmptyParts);
		if (_valueString[0].trimmed().contains(_settingName.trimmed(), Qt::CaseInsensitive))
		{
			_retValue = _valueString[1];
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

int ConfigFile::GetValueIndexInCategory(const QString& _categoryName, const QString& _setting)
{
	const QJsonArray& _array = value(_categoryName).toArray();
	if (_array.isEmpty()) return -1;
	for(int i = 0; i < _array.size() ; i++)
	{
		const QJsonValue& _value = _array[i];
		if (_value.toString().contains(_setting))
			return i;
	}
	return -1;
}

bool ConfigFile::ChangeValueInCategory(const QString& _categoryName, const QString& _setting, const QString& _newValue)
{
	int _index = GetValueIndexInCategory(_categoryName, _setting);
	QJsonArray _currentArray = take(_categoryName).toArray(); //
	if (_currentArray.isEmpty()) return false;
	QString _toAdd = _setting + "=" + _newValue;
	if (_index == -1)
		_currentArray.push_back(_toAdd);
	else
		_currentArray[_index] = _toAdd;
	insert(_categoryName, _currentArray);
	return true;
}

void ConfigFile::CreateCategory(const QString& _category, const QString& _setting, const QString& _newValue)
{
	QJsonArray _array = QJsonArray();
	_array.push_back(_setting + "=" + _newValue);
	insert(_category, _array);
}

QString ConfigFile::ToString(const QString& _category)const
{
	QString _toRet = "";
	std::string _retValue = "";
	QJsonArray _array = QJsonArray();
	if (!GetAllValuesFromCategory(_category, _array)) return _toRet;
	_toRet += _category + "\n";
	//_retValue += _category.toStdString() + "\n";
	for (QJsonValue _value : _array)
	{
		_toRet += _value.toString() + "\n";
		//_retValue += _value.toString().toStdString() + "\n";
	}
	return _toRet;
}

bool ConfigFile::SaveFileInProject()
{
	return IOToolBox::WriteInFile(filePath, ToIniFile(), true);
}

QString ConfigFile::GetPath() const
{
	return filePath;
}

void ConfigFile::SetPath(const QString& _path)
{
	filePath = _path + "/" +fileName;
}

QString ConfigFile::ToIniFile() const
{
	QString _toReturn = "";
	QStringList _list = GetAllCategories();
	for (QString _category : _list)
		_toReturn += ToString(_category) + "\n";
	return _toReturn;
}

int ConfigFile::FindCategory(std::vector<QString> _list, int _currentIndex)
{
	for (int i = _currentIndex; i < _list.size(); i++)
	{
		if (_list[i].contains("[/"))
			return i;
	}
	return -1;
}

void ConfigFile::ParseFile(const QString& _toParse)
{
	std::vector<QString> _lines = IOToolBox::GetAllLines(_toParse);
	if (_lines.size() <= 0) return;
	for (int i = 0; i < _lines.size(); i++)
	{
		const int _category = FindCategory(_lines, i);
		if (_category != -1)
		{
			int y = _category + 1;
			int _nextCategory = FindCategory(_lines, y);
			_nextCategory = _nextCategory == -1 ? _lines.size() : _nextCategory;
			std::vector<QString> _debug = std::vector<QString>();
			QJsonArray _array = QJsonArray();
			for (y; y < _nextCategory; y++)
			{
				_array.push_back(_lines[y]);
				_debug.push_back(_lines[y]);
			}
			insert(_lines[i], _array);
			i = y - 1;
		}
		else return;
	}
}
