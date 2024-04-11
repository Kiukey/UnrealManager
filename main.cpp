#include "Window/UnrealManager.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    UnrealManager w;
    w.show();
    return a.exec();
}
