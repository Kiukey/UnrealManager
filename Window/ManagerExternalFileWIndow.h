#pragma once

#include <QDialog>
#include "ui_ManagerExternalFileWIndow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ManagerExternalFileWIndowClass; };
QT_END_NAMESPACE

class ManagerExternalFileWIndow : public QDialog
{
	Q_OBJECT
private:
	Ui::ManagerExternalFileWIndowClass* ui;
	QStringList activatedPlugins = QStringList();

public:
	ManagerExternalFileWIndow(const QString& _params, const std::vector<QString>& _activatedPlugins, QWidget *parent = nullptr);
	~ManagerExternalFileWIndow();
	QStringList ShowSettingsWindow();

private:

	void CreateCheckBoxes(const QJsonArray& _array, const std::vector<QString>& _activatedPlugins);
	void CreateCheckBox(const QString _pluginName, bool _isActivated);
	void OnCheckBoxStateChanged(class CustomCheckBox* _self, bool _state);
	bool IsSettingActivated(const std::vector<QString>& _list, const QString& _plugin) const;
};
