#include "CreateProjectWindow.h"
#include "UnrealProject.h"

CreateProjectWindow::CreateProjectWindow(QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::CreateProjectWindowClass())
{
	ui->setupUi(this);
}

CreateProjectWindow::~CreateProjectWindow()
{
	delete ui;
}

UnrealProject* CreateProjectWindow::CreateProject()
{
	setModal(true);
	exec(); // <- is here while window is open
	return project;
}
