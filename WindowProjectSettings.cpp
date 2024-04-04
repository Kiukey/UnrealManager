#include "WindowProjectSettings.h"
#include "IOToolBox.h"
#include "UnrealProject.h"
#include <qjsondocument.h>
#include "ConfigFile.h"
//#include "CustomCheckBox.h"
#include <qjsonarray.h>
#include "qgroupbox.h"
#include <qlineedit.h>

WindowProjectSettings::WindowProjectSettings(UnrealProject* _project, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	//Get the templates
	project = _project;


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
	for (std::pair<QString, ConfigFile*> _file : files)
		delete _file.second;
	templateSettings.clear();
	files.clear();
}

void WindowProjectSettings::GenerateAllTabs()
{
	if (!project) return;
	std::vector<QString> _paths = project->GetIniFilePaths();
	const int _num = _paths.size();

	for (int i = 0; i < _num; i++)
	{
		GenerateTab(IOToolBox::ReadFile(_paths[i]),templateSettings[i]);
	}

}

void WindowProjectSettings::GenerateTab(const QString& _currentSettings, DisplayedSettings& _settings)
{
	ConfigFile* _file = new ConfigFile(_currentSettings);
	
	QStringList _categoryList = _settings.file->GetAllCategories();
	QVBoxLayout* _layout = new QVBoxLayout();
	_settings.area->setLayout(_layout);
	for (const QString& _category : _categoryList)
	{
		QJsonArray _settingsList = QJsonArray();
		if (!_settings.file->GetAllValuesFromCategory(_category, _settingsList)||_settingsList.isEmpty()) continue;
		QGroupBox* _groupBox = new QGroupBox(_category,_settings.area);
		_groupBox->setLayout(new QGridLayout());
		_settings.area->layout()->addWidget(_groupBox);
		for (QJsonValue _value : _settingsList)
		{
			CreateSetting(_value.toString(), _groupBox);
		}
	}

	files.insert(std::pair<QString, ConfigFile*>(ui.tabWidget->currentWidget()->objectName(), _file));
	//delete _file; //TODO maybe move it in destructor of window to apply the new settings when user is finished
}
void WindowProjectSettings::CreateSetting(const QString& _settingLine, QGroupBox* _groupBox)
{
	//here _value = SettingsName=value so i need to parse it heeeeeelp
	if (!_groupBox || !_groupBox->layout()) return;

	//const int _equalSignIndex = _settingLine.lastIndexOf("=");
	QStringList _setting = _settingLine.split("=", Qt::SplitBehavior::enum_type::SkipEmptyParts);
	QString _settingName = _setting[0];
	QString _valueString = _setting[1];

	QLabel* _settingNameLabel = new QLabel(_settingName.trimmed(), _groupBox);
	QLineEdit* _settingValueEdit = new QLineEdit(_valueString.trimmed(), _groupBox);
	//CustomCheckBox* _checkBox = new CustomCheckBox(_settingName, _groupBox);
	//_checkBox->setChecked(_settingValue);
	QGridLayout* _layout = (QGridLayout*)_groupBox->layout();
	if (!_layout) return;
	_layout->addWidget(_settingNameLabel,_groupBox->children().count(), 0);
	_layout->addWidget(_settingValueEdit,_groupBox->children().count(), 1);
	//connect(_checkBox, &CustomCheckBox::OnCheckBoxStateChanged, this, &WindowProjectSettings::OnCheckBoxValueChanged);
}
//
ConfigFile* WindowProjectSettings::LoadSavedFile(const QByteArray& _array)
{
	QJsonDocument _doc = QJsonDocument::fromJson(_array);
	ConfigFile* _config = new ConfigFile(_doc.object());
	return _config;
}

DisplayedSettings WindowProjectSettings::CreateDisplayedSettings(const QString& _path, QScrollArea* _area)
{
	QByteArray _array = QByteArray(IOToolBox::ReadFile(_path).toStdString().c_str());
	return DisplayedSettings(LoadSavedFile(_array), _area);
}

#pragma region Working but reworking
//QWidget* _widget, bool _value
//void WindowProjectSettings::OnCheckBoxValueChanged(CustomCheckBox* _checkBox, int _state)
//{
//	//get le nom de la checkbox qui va indiquer le nom du parametre, 
//	//get le nom du parent de la checkbox qui correspond au groupbox donc au nom de la category, 
//	//et get le nom de la currentTab pour avoir le file a acceder
//	QGroupBox* _groupBox = (QGroupBox)_checkBox->parentWidget();
//	if (!_groupBox) return;
//	QString _settingName = _checkBox->text();
//	QString _categoryName = _groupBox->title();
//	QString _currentFile = ui.tabWidget->currentWidget()->objectName();
//
//
//}

//void WindowProjectSettings::ChangeParameter(const QString& _file, const QString& _category, const QString& _setting, bool _value)
//{
//	if (!Contains<QString, ConfigFile*>(files, _file))
//		return;
//	ConfigFile* _fileObject = files[_file];
//	if (!_fileObject) return;
//
//	//_fileObject->
//}

#pragma endregion