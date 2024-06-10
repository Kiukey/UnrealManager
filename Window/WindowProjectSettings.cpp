#include "WindowProjectSettings.h"
#include "../IOToolBox.h"
#include "../FileClasses/UnrealProject.h"
#include <qjsondocument.h>
#include "../FileClasses/ConfigFile.h"
#include <qjsonarray.h>
#include "qgroupbox.h"
#include "../CustomLineEdit.h"
#include <qerrormessage.h>

WindowProjectSettings::WindowProjectSettings(UnrealProject* _project, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	project = _project;
	//Get the templates
	templateSettings.push_back(CreateDisplayedSettings("Config/DefaultEditorTemplate.ini",ui.EditorArea));
	templateSettings.push_back(CreateDisplayedSettings("Config/DefaultEngineTemplate.ini",ui.EngineArea));
	templateSettings.push_back(CreateDisplayedSettings("Config/DefaultGameTemplate.ini",ui.GameArea));
	templateSettings.push_back(CreateDisplayedSettings("Config/DefaultInputTemplate.ini",ui.InputArea));
	//TODO generate Settings here
	GenerateAllTabs();
}

WindowProjectSettings::~WindowProjectSettings()
{
	for (DisplayedSettings& _file : templateSettings)
		delete _file.file;

	templateSettings.clear();
	files.clear();
}

void WindowProjectSettings::GenerateAllTabs()
{
	if (!project)
	{
		std::cout << "Project not found, Impossible to genetate tabs" << std::endl;
		return;
	}
	std::vector<ConfigFile*> _files = project->GetConfigFiles();
	const int _num = _files.size();
	for (int i = 0; i < _num; i++)
	{
		//get existing file if there is one or create a new one if none were found
		/*ConfigFile* _file = _num > 0 ? new ConfigFile(_paths[i], IOToolBox::ReadFile(_paths[i])) : new ConfigFile(project->GetConfigFolderPath() + templateSettings[i].file->GetPath().replace("Template",""));*/
		std::cout << project->GetPathToUprojectFile().toStdString() << std::endl;
		GenerateTab(_files[i], templateSettings[i]);
		files.insert(std::pair<QString, ConfigFile*>(ui.tabWidget->widget(files.size())->objectName(), _files[i]));
	}
}

void WindowProjectSettings::GenerateTab(ConfigFile* _file, DisplayedSettings& _settings)
{
	//ConfigFile* _file = new ConfigFile(_currentSettings, _filePath);
	QStringList _categoryList = _settings.file->GetAllCategories();
    QVBoxLayout* _layout = new QVBoxLayout(_settings.area);
    _settings.area->widget()->setLayout(_layout);
	for (const QString& _category : _categoryList)
	{
		QJsonArray _settingsList = QJsonArray();
		if (!_settings.file->GetAllValuesFromCategory(_category, _settingsList)||_settingsList.isEmpty()) continue;
		QGroupBox* _groupBox = new QGroupBox(QString(_category),_settings.area);
		_groupBox->setLayout(new QGridLayout(_groupBox));
		_settings.area->widget()->layout()->addWidget(_groupBox);
		int i = 0;
		for (QJsonValue _value : _settingsList)
		{
			AddFrameToLayout(_groupBox, CreateSetting(_value.toString(), _groupBox, _file, i));
			i++;
		}
	}
}

QFrame* WindowProjectSettings::CreateSetting(const QString& _settingLine, QGroupBox* _groupBox,ConfigFile* _file, int _i)
{
	//here _value = SettingsName=value so i need to parse it heeeeeelp
	//TODO detect if setting exist in current project and display it's value if it does
	if (!_groupBox || !_groupBox->layout()) return nullptr;
	QFrame* _toRet = new QFrame(_groupBox);
	//get the values to set in the params
	QStringList _setting = _settingLine.split("=", Qt::SplitBehavior::enum_type::SkipEmptyParts);
	QString _settingName = _setting[0];
	QString _valueString = GetDefaultValue(_groupBox->title(), _setting, _file);
	//create the params
	QHBoxLayout* _layout = new QHBoxLayout();
	_toRet->setLayout(_layout);
	QLabel* _settingNameLabel = new QLabel(_settingName.trimmed(), _groupBox);
	CustomLineEdit* _settingValueEdit = new CustomLineEdit(_valueString.trimmed(), _groupBox);
	_settingNameLabel->setMinimumSize(50, 50);
	_settingValueEdit->setObjectName(_groupBox->title() + "|" + _settingName.trimmed());
	_settingValueEdit->setMinimumSize(50, 20);
	connect(_settingValueEdit, &CustomLineEdit::EditFinishedCustomLineEdit, this, &WindowProjectSettings::OnLineEditChanged);
	_layout->addWidget(_settingNameLabel);
	_layout->addWidget(_settingValueEdit);
	return _toRet;
}

ConfigFile* WindowProjectSettings::LoadSavedFile(const QString& _path,const QString& _content)
{
	ConfigFile* _config = new ConfigFile(_content,_path);
	return _config;
}

DisplayedSettings WindowProjectSettings::CreateDisplayedSettings(const QString& _path, QScrollArea* _area)
{
	QString _fileContent = IOToolBox::ReadFile(_path);
	return DisplayedSettings(LoadSavedFile(_path, _fileContent), _area);
}

QString WindowProjectSettings::GetDefaultValue(const QString _category,const QStringList& _settingLine, ConfigFile* _file)
{
	QJsonValue _value;
	if (_file->FindSetting(_settingLine[0], _value))
		return _value.toString();
	else
	{
		if (!_settingLine[1].toLower().contains("false") && _settingLine[1].toLower() != "0")
			_file->CreateCategory(_category, _settingLine[0], _settingLine[1]);
		return _settingLine[1];
	}
}

void WindowProjectSettings::OnLineEditChanged(CustomLineEdit* _self, const QString& _text)
{
	//get le nom de la checkbox qui va indiquer le nom du parametre, 
	//get le nom du parent de la checkbox qui correspond au groupbox donc au nom de la category, 
	//et get le nom de la currentTab pour avoir le file a acceder
	//QGroupBox* _groupBox = (QGroupBox*)_self->parentWidget();
	//if (!_groupBox) return;
	if (!this) return;
	QStringList _nameSplitted = _self->objectName().split("|");
	const QString & _settingName = _nameSplitted[1];
	const QString& _categoryName = _nameSplitted[0];
	const QString& _currentFile = ui.tabWidget->currentWidget()->objectName();
	ChangeParameter(_currentFile, _categoryName, _settingName, _text);
}

void WindowProjectSettings::ChangeParameter(const QString& _file, const QString& _category, const QString& _setting, const QString& _value)
{
	if (!Contains<QString, ConfigFile*>(files, _file))
		return;
	ConfigFile* _fileObject = files[_file];
	if (!_fileObject) return;
	if (!_fileObject->ChangeValueInCategory(_category, _setting, _value))
		_fileObject->CreateCategory(_category, _setting, _value);
}

void WindowProjectSettings::AddFrameToLayout(QGroupBox* _group, QFrame* _frame)
{
	QGridLayout* _layout = (QGridLayout*)_group->layout();
	if (!_layout) return;
	_layout->addWidget(_frame);
}

void WindowProjectSettings::AddWidgetToLayout(QWidget* _widget)
{
	if (!layout()) return;
	layout()->addWidget(_widget);
}
