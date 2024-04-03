#include "WindowProjectSettings.h"
#include "IOToolBox.h"
#include "UnrealProject.h"

WindowProjectSettings::WindowProjectSettings(UnrealProject* _project, QWidget* parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	//Get the templates
	project = _project;
	//templatePaths.push_back(SettingsTemplate("Templates/DefaultEditorTemplate.ini", ui.EditorScrollArea));
	//templatePaths.push_back(SettingsTemplate("Templates/DefaultEngineTemplate.ini", ui.EngineScrollArea));
	//templatePaths.push_back(SettingsTemplate("Templates/DefaultGameTemplate.ini", ui.GameScrollArea));
	//templatePaths.push_back(SettingsTemplate("Templates/DefaultInputTemplate.ini", ui.InputScrollArea));

	//TODO generate Settings here
	GenerateAllTabs();
}

WindowProjectSettings::~WindowProjectSettings()
{
}

void WindowProjectSettings::GenerateAllTabs()
{
	if (!project) return;
	std::vector<QString> _paths = project->GetIniFilePaths();
	const int _num = _paths.size();

	for (int i = 0; i < _num; i++)
	{
		GenerateTab(IOToolBox::ReadFile(_paths[i]), templatePaths[i]);
	}

}

void WindowProjectSettings::GenerateTab(const QString& _currentSettings, const SettingsTemplate& _template)
{
	std::vector<QString> _currentLines = IOToolBox::GetAllLines(_currentSettings);
	std::vector<QString> _currentTemplate = IOToolBox::GetAllLines(IOToolBox::ReadFile(_template.path));
	if (_currentLines.size() <= 0 || _currentTemplate.size() <= 0) return;

	for (const QString& _templateParam : _currentTemplate)
	{

		//_template.container->layout()->addWidget()
	}
}
