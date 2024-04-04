#pragma once
#include <qlabel.h>
#include <qpushbutton.h>
#include <QHboxLayout>
#include <vector>
class UnrealProject;

class UnrealProjectWidgets : public QObject
{
	Q_OBJECT
private:
	UnrealProject* owner = nullptr;

	QHBoxLayout* layout = nullptr;
	QLabel* projectName = nullptr;
	QLabel* projectPath = nullptr;
	QPushButton* openProjectButton = nullptr;
	QPushButton* openProjectSettingsButton = nullptr;

public:
	UnrealProjectWidgets(UnrealProject* _project);
	~UnrealProjectWidgets();
	QHBoxLayout* GetWidget() const;

private slots:
	void OpenProject();
	void OpenProjectSettings();
};