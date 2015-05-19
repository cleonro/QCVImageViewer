#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "viewport.h"

#include <QFileDialog>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bool b = connect(ui->actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    Q_ASSERT( b );
    b = connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenActionTriggered);
    Q_ASSERT(b);

    m_viewPort = new ViewPort(this);
    this->setCentralWidget(m_viewPort);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpenActionTriggered()
{
    QString imageFileName = QFileDialog::getOpenFileName(this, tr("Open Image File"), QDir::currentPath(), tr("Images (*.png *.xpm *.jpg)"));
    if(!imageFileName.isEmpty())
    {
        m_viewPort->openImageFile(imageFileName);
    }
}
