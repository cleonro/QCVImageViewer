#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "viewport.h"
#include "viewportopengl.h"
#include "imagecontrols.h"

#include <QFileDialog>
#include <QDir>
#include <QDockWidget>
#include <QFileInfo>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    bool b = connect(ui->actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    Q_ASSERT( b );
    b = connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::onOpenActionTriggered);
    Q_ASSERT(b);

    //m_viewPort = new ViewPort(this);
    //this->setCentralWidget(m_viewPort);
    m_viewPort = new ViewportOpenGL(this);
    this->setCentralWidget(m_viewPort->viewport());

    addControlWidgets();
    setWindowTitle("QCVImageViewer");

    m_fileDialog = new QFileDialog(this, Qt::Dialog);
    m_fileDialog->setFileMode(QFileDialog::ExistingFile);
    m_fileDialog->setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    m_fileDialog->setWindowTitle(tr("Open Image File"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onOpenActionTriggered()
{
//    QString imageFileName;// = QFileDialog::getOpenFileName(this, tr("Open Image File"), QDir::currentPath(), tr("Images (*.png *.xpm *.jpg)"));
//    if(m_fileDialog->exec())
//    {
//        imageFileName = m_fileDialog->selectedFiles().first();
//    }
//    if(!imageFileName.isEmpty())
//    {
//        bool fileOpened = m_viewPort->openImageFile(imageFileName);
//        if(fileOpened)
//        {
//            QString windowTitle = QFileInfo(imageFileName).fileName();
//            setWindowTitle(windowTitle);
//        }
//    }
}

void MainWindow::addControlWidgets()
{
//    QDockWidget* dock = new QDockWidget(tr("Image Controls"), this);
//    ImageControls* controls = new ImageControls(this);
//    controls->setImageViewPort(m_viewPort);
//    dock->setWidget(controls);

//    this->addDockWidget(Qt::LeftDockWidgetArea, dock);
//    dock->setFloating(false);
//    dock->hide();

//    ui->menuWindow->addAction(dock->toggleViewAction());
}
