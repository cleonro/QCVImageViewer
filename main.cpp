#include "mainwindow.h"
#include "mainwindow2.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    //MainWindow mw;
    MainWindow2 mw;
    mw.show();

    int retVal = app.exec();
    return retVal;
}
