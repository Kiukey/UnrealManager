#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_UnrealManager.h"

class UnrealManager : public QMainWindow
{
    Q_OBJECT

public:
    UnrealManager(QWidget *parent = nullptr);
    ~UnrealManager();

private:
    Ui::UnrealManagerClass ui;
};
