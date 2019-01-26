#include "mainwindow2.h"
#include "ui_mainwindow2.h"
#include "viewportcontroller.h"
#include "imagecontrols2.h"

#include <QFileDialog>
#include <QDir>
#include <QDockWidget>
#include <QFileInfo>

MainWindow2::MainWindow2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow2)
{
    ui->setupUi(this);
    bool b = connect(ui->actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    Q_ASSERT( b );
    b = connect(ui->actionOpen, &QAction::triggered, this, &MainWindow2::onOpenActionTriggered);
    Q_ASSERT(b);

    m_controller = new ViewportController(this);
    //this->setCentralWidget(m_controller->initViewport(ViewportController::OPENGL));
    this->setCentralWidget(m_controller->initViewport(ViewportController::VTK));

    addControlWidgets();
    setWindowTitle("QCVImageViewer");

    m_fileDialog = new QFileDialog(this, Qt::Dialog);
    m_fileDialog->setFileMode(QFileDialog::ExistingFile);
    m_fileDialog->setNameFilter(tr("Images (*.png *.xpm *.jpg)"));
    m_fileDialog->setWindowTitle(tr("Open Image File"));
}

MainWindow2::~MainWindow2()
{
    delete ui;
}

void MainWindow2::onOpenActionTriggered()
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

void MainWindow2::addControlWidgets()
{
    QDockWidget* dock = new QDockWidget(tr("Image Controls"), this);
    ImageControls2* controls = new ImageControls2(this);
    controls->setController(m_controller);
    dock->setWidget(controls);

    this->addDockWidget(Qt::LeftDockWidgetArea, dock);
    dock->setFloating(false);
    dock->hide();

    ui->menuWindow->addAction(dock->toggleViewAction());
}
