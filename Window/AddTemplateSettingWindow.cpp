#include "AddTemplateSettingWindow.h"
#include "../IOToolBox.h"

AddTemplateSettingWindow::AddTemplateSettingWindow(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	AddTemplateFile("DefaultEditorTemplate", "Config/DefaultEditorTemplate.ini");
	AddTemplateFile("DefaultEngineTemplate", "Config/DefaultEngineTemplate.ini");
	AddTemplateFile("DefaultGameTemplate", "Config/DefaultGameTemplate.ini");
	AddTemplateFile("DefaultInputTemplate", "Config/DefaultInputTemplate.ini");
}

AddTemplateSettingWindow::~AddTemplateSettingWindow()
{

	for (std::pair<QString, ConfigFile*> _pair : templateFiles)
	{
		delete _pair.second;
	}
}

void AddTemplateSettingWindow::AddTemplateFile(const QString& _templateName, const QString& _templatePath)
{
	ConfigFile* _file = new ConfigFile(IOToolBox::ReadFile(_templatePath), _templatePath);
	templateFiles.insert(std::pair<QString, ConfigFile*>(_templateName, _file));
	ui.FileComboBox->addItem(_templateName);
}

void AddTemplateSettingWindow::on_CancelButton_clicked()
{
	close();
}

void AddTemplateSettingWindow::on_ConfirmButton_clicked()
{
	const QString& _category = "["+ ui.CategoryNameLineEdit->text()+"]";
	const QString& _settingName = ui.SettingNameLineEdit->text();
	const QString& _settingValue = ui.DefaultValueLineEdit->text();
	if (_category.isEmpty() || _settingName.isEmpty() || _settingValue.isEmpty()) return;
	ConfigFile* _file = templateFiles[ui.FileComboBox->currentText()];
	if (!_file) return;
	if (_file->IsCategoryExisting(_category))
		_file->AddSettingToCategory(_category, _settingName, _settingValue);
	else
		_file->CreateCategory(_category, _settingName, _settingValue);
	
	IOToolBox::WriteInFile(_file->GetPath(), _file->ToIniFile(), true);
	accept();
}