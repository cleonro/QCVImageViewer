#include "mainwindow2.h"
#include "ui_mainwindow2.h"
#include "viewportcontroller.h"
#include "imagecontrols2.h"

#include <QFileDialog>
#include <QDir>
#include <QDockWidget>
#include <QFileInfo>

#include "ffmpegtest.h"

MainWindow2::MainWindow2(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow2)
{
    ui->setupUi(this);
    bool b = connect(ui->actionQuit, &QAction::triggered, qApp, &QApplication::quit);
    Q_ASSERT( b );
    b = connect(ui->actionOpen, &QAction::triggered, this, &MainWindow2::onOpenActionTriggered);
    Q_ASSERT(b);

    m_controller.reset(new ViewportController(this));
#if USE_VTK
    this->setCentralWidget(m_controller->initViewport(ViewportController::VTK));
#else
    this->setCentralWidget(m_controller->initViewport(ViewportController::OPENGL));
#endif //USE_VTK


    addControlWidgets();
    setWindowTitle("QCVImageViewer");

    m_fileDialog = new QFileDialog(this, Qt::Dialog);
    m_fileDialog->setFileMode(QFileDialog::ExistingFile);
    m_fileDialog->setNameFilter(tr("Images (*.png *.xpm *.jpg *.mpeg *.mpg *.mp4 *.avi)"));
    m_imageExtensions << "png" << "xpm" << "jpg";
    m_movieExtensions << "mpeg" << "mpg" << "mp4" << "avi";
    m_fileDialog->setWindowTitle(tr("Open Image File"));

    //ffmpeg test
    m_ffmpeg.reset(new FfmpegTest());
    m_ffmpeg->init();
}

MainWindow2::~MainWindow2()
{
    delete ui;
}

void MainWindow2::onOpenActionTriggered()
{
    QString fileName;// = QFileDialog::getOpenFileName(this, tr("Open Image File"), QDir::currentPath(), tr("Images (*.png *.xpm *.jpg)"));
    if(m_fileDialog->exec())
    {
        auto selectedFiles = m_fileDialog->selectedFiles();
        fileName = selectedFiles.first();
    }
    if(!fileName.isEmpty())
    {
        QString extension = QFileInfo(fileName).completeSuffix();
        bool isImage = m_imageExtensions.contains(extension);
        bool fileOpened = isImage
                ? m_controller->openImageFile(fileName)
                : (m_controller->openMovieFile(fileName) && m_ffmpeg->openStreamFile(fileName));
        if(fileOpened)
        {
            QString windowTitle = QFileInfo(fileName).fileName();
            setWindowTitle(windowTitle);
            emit imageFileOpened(fileName);
        }
    }
}

void MainWindow2::addControlWidgets()
{
    QDockWidget* dock = new QDockWidget(tr("Image Controls"), this);
    ImageControls2* controls = new ImageControls2(this);
    connect(controls, &ImageControls2::cameraOpened, this, &MainWindow2::onCameraOpened);
    connect(this, &MainWindow2::imageFileOpened, controls, &ImageControls2::onImageFileOpened);
    controls->setController(m_controller.get());
    dock->setWidget(controls);

    this->addDockWidget(Qt::LeftDockWidgetArea, dock);
    dock->setFloating(false);
    dock->hide();

    ui->menuWindow->addAction(dock->toggleViewAction());
}

void MainWindow2::onCameraOpened(const QString &cameraTitle)
{
    this->setWindowTitle(cameraTitle);
}
