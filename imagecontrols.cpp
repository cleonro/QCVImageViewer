#include "imagecontrols.h"
#include "ui_imagecontrols.h"
#include "viewport.h"

ImageControls::ImageControls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageControls),
    m_imageViewPort(nullptr),
    m_brightness(0),
    m_contrast(1.0)
{
    ui->setupUi(this);


}

ImageControls::~ImageControls()
{

}

void ImageControls::setImageViewPort(ViewPort *imageViewPort)
{
    m_imageViewPort = imageViewPort;
    bool b = connect(imageViewPort, &ViewPort::resetControls, this, &ImageControls::onReset);
    Q_ASSERT(b);
}

void ImageControls::onSliderValueChanged(int value)
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
    if(transformImage && m_imageViewPort != nullptr)
    {
        m_imageViewPort->changeBrightnessContrast(m_brightness, m_contrast);
    }
}

void ImageControls::onReset()
{
    m_brightness = 0;
    m_contrast = 1.0;
    ui->sliderBrightness->setValue(0);
    ui->sliderContrast->setValue(100);
}
