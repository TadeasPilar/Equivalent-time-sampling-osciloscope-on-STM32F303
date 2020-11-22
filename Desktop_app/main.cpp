#include "scopewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ScopeWindow w;
    w.resize(1280,720);     //Set default size of main window
    w.show();
    return a.exec();
}
