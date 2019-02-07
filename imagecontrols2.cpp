#include "imagecontrols2.h"
#include "ui_imagecontrols2.h"
#include "viewport.h"
#include "viewportcontroller.h"

#include <QtMultimedia/QCameraInfo>

ImageControls2::ImageControls2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageControls2),
    //m_imageViewPort(nullptr),
    m_controller(nullptr),
    m_brightness(0),
    m_contrast(1.0)
{
    ui->setupUi(this);

    ui->cameraShow->setDisabled(true);
    listCameras();
}

ImageControls2::~ImageControls2()
{

}

void ImageControls2::setController(ViewportController *controller)
{
    m_controller = controller;
    m_filters.clear();
    m_filters.resize(ViewportController::COUNT);
    m_filters[ViewportController::FILTERS::BRIGHTNESS_CONTRAST] = ui->cbBC;
    m_filters[ViewportController::FILTERS::FACE_RECOGNITION] = ui->cbFR;
    m_filters[ViewportController::FILTERS::INFO] = ui->cbInfo;
    connect(m_controller, &ViewportController::filterStateChanged, this, &ImageControls2::onFilterStateChanged);
}

void ImageControls2::onSliderValueChanged(int value)
{
    QObject* sender = this->sender();
    bool transformImage = false;
    if(sender == ui->sliderBrightness)
    {
        m_brightness = value;
        ui->groupBrightness->setTitle(QString("Brightness %1").arg(m_brightness));
        transformImage = true;
    }
    else if(sender == ui->sliderContrast)
    {
        m_contrast = value / 100.0;
        ui->groupContrast->setTitle(QString("Contrast %1").arg(m_contrast));
        transformImage = true;
    }
    if(transformImage)
    {
        m_controller->setBrightnessContrast(m_brightness, m_contrast);
    }
}

void ImageControls2::onReset()
{
    m_brightness = 0;
    m_contrast = 1.0;
    ui->sliderBrightness->setValue(0);
    ui->sliderContrast->setValue(100);

    m_controller->setBrightnessContrast(m_brightness, m_contrast);
}

int ImageControls2::selectedCamera()
{
    return m_cameraIndex;
}

void ImageControls2::on_cameraList_currentIndexChanged(int index)
{
    m_cameraIndex = index;
    if(!ui->cameraShow->isEnabled() && index >= 0)
    {
        ui->cameraShow->setEnabled(true);
    }

    if(ui->cameraShow->isEnabled() && m_controller != nullptr)
    {
        if(m_controller->openCamera(index))
        {
            QString t = ui->cameraList->currentText();
            emit cameraOpened(t);
        }
    }
}

void ImageControls2::on_cameraShow_stateChanged(int arg1)
{
    bool b = arg1 == Qt::Checked;

    if(b)
    {
        int index = ui->cameraList->currentIndex();
        if(m_controller->openCamera(index))
        {
            QString t = ui->cameraList->currentText();
            emit cameraOpened(t);
        }
    }
    else
    {
        m_controller->closeCamera();
    }
}

void ImageControls2::listCameras()
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach(const QCameraInfo &camera, cameras)
    {
        QString cameraName = camera.description();
        ui->cameraList->addItem(cameraName);
    }
}

void ImageControls2::onImageFileOpened()
{
    ui->cameraShow->setChecked(false);
}

void ImageControls2::on_cbBC_toggled(bool checked)
{
    m_controller->setFilterActive(ViewportController::BRIGHTNESS_CONTRAST, checked);
}

void ImageControls2::on_cbFR_toggled(bool checked)
{
    m_controller->setFilterActive(ViewportController::FACE_RECOGNITION, checked);
}

void ImageControls2::on_cbInfo_toggled(bool checked)
{
    m_controller->setFilterActive(ViewportController::INFO, checked);
}

void ImageControls2::onFilterStateChanged(QVector<bool> state)
{
    for(int i = 0; i < ViewportController::COUNT; ++i)
    {
        m_filters[i]->blockSignals(true);
        m_filters[i]->setEnabled(state[i]);
        m_filters[i]->setChecked(state[i]);
        m_filters[i]->blockSignals(false);
    }
}
