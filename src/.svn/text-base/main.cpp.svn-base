#include <iostream>
#include <QApplication>
#include <QDesktopWidget>
//#include "gui/QTWindow.h"
#include "gui/QTMainWindow.h"
using namespace std;

int main(int argc, char *argv[]) {
    //QWidget *parent;
    QApplication app(argc, argv);
    //QTWindow window;
    QTMainWindow window;
    window.resize(window.sizeHint());
    int desktopArea = QApplication::desktop()->width() * QApplication::desktop()->height();
    int widgetArea = window.width() * window.height();
    if (((float)widgetArea / (float)desktopArea) < 0.75f)
        window.show();
    else
        window.showMaximized();
    return app.exec();
}
