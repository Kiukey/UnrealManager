#include "WindowProjectSettings.h"
#include "IOToolBox.h"
#include "UnrealProject.h"
#include <qjsondocument.h>
#include "ConfigFile.h"
#include <qjsonarray.h>
#include "qgroupbox.h"
#include "CustomLineEdit.h"
#include <qerrormessage.h>

WindowProjectSettings::WindowProjectSettings(UnrealProject* _project, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	project = _project;
	//Get the templates
	templateSettings.push_back(CreateDisplayedSettings("Templates/DefaultEditorTemplate.ini",ui.EditorArea));
	templateSettings.push_back(CreateDisplayedSettings("Templates/DefaultEngineTemplate.ini",ui.EngineArea));
	templateSettings.push_back(CreateDisplayedSettings("Templates/DefaultGameTemplate.ini",ui.GameArea));
	templateSettings.push_back(CreateDisplayedSettings("Templates/DefaultInputTemplate.ini",ui.InputArea));
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
	if (!project) return;
	std::vector<ConfigFile*> _files = project->GetConfigFiles();
	const int _num = _files.size();
	for (int i = 0; i < _num; i++)
	{
		//get existing file if there is one or create a new one if none were found
		/*ConfigFile* _file = _num > 0 ? new ConfigFile(_paths[i], IOToolBox::ReadFile(_paths[i])) : new ConfigFile(project->GetConfigFolderPath() + templateSettings[i].file->GetPath().replace("Template",""));*/
		std::cout << project->GetProjectPath().toStdString() << std::endl;
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
		QGroupBox* _groupBox = new QGroupBox(_category,_settings.area);
		_groupBox->setLayout(new QGridLayout(_groupBox));
		_settings.area->widget()->layout()->addWidget(_groupBox);
		int i = 0;
		for (QJsonValue _value : _settingsList)
		{
			CreateSetting(_value.toString(), _groupBox,_file,i);
			i++;
		}
	}
}

void WindowProjectSettings::CreateSetting(const QString& _settingLine, QGroupBox* _groupBox,const ConfigFile* _file, int _i)
{
	//here _value = SettingsName=value so i need to parse it heeeeeelp
	//TODO detect if setting exist in current project and display it's value if it does
	if (!_groupBox || !_groupBox->layout()) return;
	QStringList _setting = _settingLine.split("=", Qt::SplitBehavior::enum_type::SkipEmptyParts);
	QString _settingName = _setting[0];
	QString _valueString = GetDefaultValue(_setting, _file);
	QLabel* _settingNameLabel = new QLabel(_settingName.trimmed(), _groupBox);
	_settingNameLabel->setMinimumSize(50, 50);
	CustomLineEdit* _settingValueEdit = new CustomLineEdit(_valueString.trimmed(), _groupBox);
	_settingValueEdit->setObjectName(_groupBox->title() + _settingName.trimmed());
	_settingValueEdit->setMinimumSize(50, 20);
	QGridLayout* _layout = (QGridLayout*)_groupBox->layout();
	if (!_layout) return;

	_layout->addWidget(_settingNameLabel, _i, 0);
	_layout->addWidget(_settingValueEdit, _i, 1);
	connect(_settingValueEdit, &CustomLineEdit::EditFinishedCustomLineEdit, this, &WindowProjectSettings::OnLineEditChanged);
}

ConfigFile* WindowProjectSettings::LoadSavedFile(const QString& _path,const QByteArray& _array)
{
	QJsonDocument _doc = QJsonDocument::fromJson(_array);
	ConfigFile* _config = new ConfigFile(_doc.object(),_path);
	return _config;
}

DisplayedSettings WindowProjectSettings::CreateDisplayedSettings(const QString& _path, QScrollArea* _area)
{
	QByteArray _array = QByteArray(IOToolBox::ReadFile(_path).toStdString().c_str());
	return DisplayedSettings(LoadSavedFile(_path,_array), _area);
}

QString WindowProjectSettings::GetDefaultValue(const QStringList& _settingLine, const  ConfigFile* _file)
{
	QJsonValue _value;
	if (_file->FindSetting(_settingLine[0], _value))
		return _value.toString();
	else
		return _settingLine[1];
}

void WindowProjectSettings::OnLineEditChanged(CustomLineEdit* _self, const QString& _text)
{
	//	//get le nom de la checkbox qui va indiquer le nom du parametre, 
	//get le nom du parent de la checkbox qui correspond au groupbox donc au nom de la category, 
	//et get le nom de la currentTab pour avoir le file a acceder
	QGroupBox* _groupBox = (QGroupBox*)_self->parentWidget();
	if (!_groupBox) return;
	const QString& _settingName = _self->objectName();
	const QString& _categoryName = _groupBox->title();
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

void WindowProjectSettings::on_applyButton_clicked()
{
	for (std::pair<QString, ConfigFile*> _pair : files)
	{
		if (!_pair.second->SaveFileInProject())
		{
			QErrorMessage _message = QErrorMessage(this);
			_message.showMessage(QString("Error saving file in path %1").arg(_pair.second->GetPath()));
			_message.exec();
		}
	}
}