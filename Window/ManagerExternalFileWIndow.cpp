#include "ManagerExternalFileWIndow.h"
#include <qjsondocument.h>
#include <qjsonobject.h>
#include <qjsonarray.h>
#include "../CustomCheckBox.h"
#include "../IOToolBox.h"
#include <QGridLayout>


ManagerExternalFileWIndow::ManagerExternalFileWIndow(const QString& _params,const std::vector<QString>& _activatedPlugins, QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::ManagerExternalFileWIndowClass())
{
	ui->setupUi(this);
	QByteArray _array = QByteArray(IOToolBox::ReadFile("Templates/Plugins&ModulesTemplate.ini").toStdString().c_str());
	QJsonObject _object = QJsonDocument::fromJson(_array).object();
	CreateCheckBoxes(_object.value(_params).toArray(), _activatedPlugins);
}

ManagerExternalFileWIndow::~ManagerExternalFileWIndow()
{
	delete ui;
}

QStringList ManagerExternalFileWIndow::ShowSettingsWindow()
{
	setModal(true);
	exec(); // here while window is opened
	return activatedPlugins;
}

void ManagerExternalFileWIndow::CreateCheckBoxes(const QJsonArray& _array, const std::vector<QString>& _activatedPlugins)
{
	for (const QJsonValue& _value : _array)
	{
		const QString _pluginName = _value.toString();
		CreateCheckBox(_pluginName, IsSettingActivated(_activatedPlugins, _pluginName));
	}
}

void ManagerExternalFileWIndow::CreateCheckBox(const QString _pluginName, bool _isActivated)
{
	CustomCheckBox* _checkBox = new CustomCheckBox(this);
	connect(_checkBox, &CustomCheckBox::OnCheckBoxStateChanged, this, &ManagerExternalFileWIndow::OnCheckBoxStateChanged);
	_checkBox->setText(_pluginName);
	_checkBox->setChecked(_isActivated);
	QGridLayout* _layout = (QGridLayout*)ui->PluginCheckBoxContainer->layout();
	if (!_layout) return;
	_layout->addWidget(_checkBox, _layout->count(), 0, Qt::AlignHCenter);
}

void ManagerExternalFileWIndow::OnCheckBoxStateChanged(CustomCheckBox* _self, bool _state)
{
	if (_state)
		activatedPlugins.push_back(_self->text());
	else
		activatedPlugins.removeAt(activatedPlugins.indexOf(_self->text()));
}

bool ManagerExternalFileWIndow::IsSettingActivated(const std::vector<QString>& _list, const QString& _plugin) const
{
	for (const QString& _activatedPlugin : _list)
	{
		if (_activatedPlugin == _plugin) return true;
	}
	return false;
}
