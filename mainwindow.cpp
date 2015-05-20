#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "viewport.h"
#include "imagecontrols.h"

#include <QFileDialog>
#include <QDir>
#include <QDockWidget>

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

    addControlWidgets();
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

void MainWindow::addControlWidgets()
{
    QDockWidget* dock = new QDockWidget(tr("Image Controls"), this);
    ImageControls* controls = new ImageControls(this);
    controls->setImageViewPort(m_viewPort);
    dock->setWidget(controls);

    this->addDockWidget(Qt::LeftDockWidgetArea, dock);
    dock->setFloating(false);
    dock->hide();

    ui->menuWindow->addAction(dock->toggleViewAction());
}
