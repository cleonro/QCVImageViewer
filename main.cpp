#include "mainwindow.h"
#include "mainwindow2.h"
#include <QApplication>

#include <QSurfaceFormat>

int main(int argc, char* argv[])
{
#if USE_VTK
    QSurfaceFormat surfaceFormat;
    surfaceFormat.setVersion(4, 1);
    surfaceFormat.setProfile(QSurfaceFormat::CoreProfile);
    surfaceFormat.setRenderableType(QSurfaceFormat::OpenGL);
    surfaceFormat.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    QSurfaceFormat::setDefaultFormat(surfaceFormat);
#endif //USE_VTK
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_DontCreateNativeWidgetSiblings);
    //MainWindow mw;
    MainWindow2 mw;
    mw.show();

    int retVal = app.exec();
    return retVal;
}
