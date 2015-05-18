#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <opencv2/highgui.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bool b = connect(ui->actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    Q_ASSERT( b );

    cv::namedWindow("central_widget");
   // cv::
}

MainWindow::~MainWindow()
{
    delete ui;
}
