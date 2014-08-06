#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QTextEdit>

//Pointer to the debug window
QTextEdit* DebugWindow;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    //Center the main window
    QDesktopWidget* desktop = QApplication::desktop();
    int x = 0;
    int y = 0;

    x = (desktop->width() - w.width()) / 2;
    y = (desktop->height() - w.height()) / 2;

    w.setGeometry(x,y,w.width(),w.height());

    //Show the main window
    w.show();

    return a.exec();
}
