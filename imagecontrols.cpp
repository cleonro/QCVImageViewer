#include "imagecontrols.h"
#include "ui_imagecontrols.h"
#include "viewport.h"

ImageControls::ImageControls(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImageControls),
    m_imageViewPort(nullptr)
{
    ui->setupUi(this);
}

ImageControls::~ImageControls()
{

}

void ImageControls::setImageViewPort(ViewPort *imageViewPort)
{
    m_imageViewPort = imageViewPort;
}

