#pragma once

#include <QDialog>
#include "ui_AddTemplateSettingWindow.h"
#include <map>
#include "../FileClasses/ConfigFile.h"

class AddTemplateSettingWindow : public QDialog
{
	Q_OBJECT
private:
	Ui::AddTemplateSettingWindowClass ui;

	std::map<QString, ConfigFile*> templateFiles = std::map<QString, ConfigFile*>();
public:
	AddTemplateSettingWindow(QWidget *parent = nullptr);
	~AddTemplateSettingWindow();

private:
	void AddTemplateFile(const QString& _templateName, const QString& _templatePath);

private slots:
	void on_ConfirmButton_clicked();
	void on_CancelButton_clicked();
};
