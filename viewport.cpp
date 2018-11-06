#include "viewport.h"
#include <QGLWidget>

ViewPort::ViewPort(QWidget *parent)  :
    QOpenGLWidget(parent)
{
    m_resizedWidth = 0;
    m_resizedHeight = 0;
    m_imageRatio = 4.0f / 3.0f;
    m_positionX = 0;
    m_positionY = 0;

    m_brightness = 0;
    m_contrast = 1.0;
}

ViewPort::~ViewPort()
{

}

void ViewPort::initializeGL()
{

}

void ViewPort::paintGL()
{
    glClearColor(0.3, 0.3, 0.3, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderImage();
}

void ViewPort::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, 0, 1);
    glMatrixMode(GL_MODELVIEW);

    resizeImage(width, height);
    this->update();
}

void ViewPort::renderImage()
{
    if(m_resizedRenderImage.isNull())
    {
        return;
    }

    int w = m_resizedRenderImage.width();
    int h = m_resizedRenderImage.height();

    glLoadIdentity();
    glPushMatrix();

    glRasterPos2i(m_positionX, m_positionY);
    glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, m_resizedRenderImage.bits());

    glPopMatrix();
}

void ViewPort::resizeImage(int width, int height)
{
    if(m_renderImage.isNull())
    {
        return;
    }

    m_resizedWidth = width;
    m_resizedHeight = width / m_imageRatio;
    if(m_resizedHeight > height)
    {
        m_resizedWidth = height * m_imageRatio;
        m_resizedHeight = height;
    }

    m_positionX = (width - m_resizedWidth) / 2;
    m_positionY = (height - m_resizedHeight) / 2;

    int w = m_renderImage.width();
    int h = m_renderImage.height();
    if(w != width && h != height)
    {
        m_resizedRenderImage = m_renderImage.scaled(QSize(m_resizedWidth, m_resizedHeight),
                                                                                        Qt::IgnoreAspectRatio,
                                                                                        Qt::SmoothTransformation
                                                                                    );
    }
    else
    {
        m_resizedRenderImage = m_renderImage;
    }
}

bool ViewPort::openImageFile(const QString &fileName)
{
    //test-remove
    static cv::VideoCapture cam = cv::VideoCapture(0);
    static bool b = false;
    if(!b)
    {
        if(!cam.isOpened())
        {
            return false;
        }
        cam >> m_cvOrigImage;
        bool imageOpened = applyBrightnessContrast();
        if(imageOpened)
        {
            this->update();
        }
        emit resetControls();
        return imageOpened;
    }
    //

    m_cvOrigImage = cv::imread(fileName.toStdString());
    //test-remove
    //imshow("Output", m_cvOrigImage);
    //
    bool imageOpened = applyBrightnessContrast();
    if(imageOpened)
    {
        this->update();
    }
    emit resetControls();
    return imageOpened;
}

bool ViewPort::openImage()
{
    m_imageRatio = float(m_cvImage.cols) / float(m_cvImage.rows);

    int channels = m_cvImage.channels();

    if(channels == 3)
    {
        m_renderImage = QImage((const unsigned char*)(m_cvImage.data),
                                     m_cvImage.cols, m_cvImage.rows,
                                     m_cvImage.step, QImage::Format_RGB888).rgbSwapped();
    }
    else if(channels == 1)
    {
        m_renderImage = QImage((const unsigned char*)(m_cvImage.data),
                                      m_cvImage.cols, m_cvImage.rows,
                                      m_cvImage.step, QImage::Format_Indexed8);
    }
    else
    {
        return false;
    }

    //TODO - replacement for QGLWidget::convertToGLFormat
    m_renderImage = QGLWidget::convertToGLFormat(m_renderImage);

    int w = this->width();
    int h = this->height();
    resizeImage(w, h);

    return true;
}

bool ViewPort::changeBrightnessContrast(int brightness, float contrast)
{
    if(m_renderImage.isNull())
    {
        return false;
    }

    m_brightness = brightness;
    m_contrast = contrast;

    applyBrightnessContrast();
    this->update();

    return true;
}

bool ViewPort::applyBrightnessContrast()
{
    m_cvImage = cv::Mat::zeros( m_cvOrigImage.size(), m_cvOrigImage.type() );

//    for( int y = 0; y < m_cvOrigImage.rows; y++ )
//    {
//        for( int x = 0; x < m_cvOrigImage.cols; x++ )
//        {
//            for( int c = 0; c < 3; c++ )
//            {
//                m_cvImage.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(
//                                                    m_contrast * ( m_cvOrigImage.at<cv::Vec3b>(y,x)[c] ) + m_brightness
//                                                    );
//            }
//        }
//    }

    m_cvOrigImage.convertTo(m_cvImage, -1, m_contrast, m_brightness);

    return openImage();
}
