#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWindow mw;
    mw.show();

    int retVal = app.exec();
    return retVal;
}
