#include "viewportsourcemovie.h"

ViewportSourceMovie::ViewportSourceMovie(QObject *parent)
    : ViewportSourceBase(parent)
{
    connect(&m_timer, &QTimer::timeout, this, &ViewportSourceMovie::onTimer);
}

ViewportSourceMovie::~ViewportSourceMovie()
{

}

void ViewportSourceMovie::open(void *source)
{
    m_filePath = *static_cast<QString*>(source);
    m_videoCapture.open(m_filePath.toStdString());
    double fps = m_videoCapture.get(cv::CAP_PROP_FPS);
    int timeout = static_cast<int>(std::floor(1000.00 / fps));
    m_timer.setInterval(timeout);
    m_timer.start();
}

void ViewportSourceMovie::close()
{
    m_videoCapture.release();
    m_filePath = "";
    m_timer.stop();
    m_cvImage.copyTo(m_cvImageCopy);
}

void* ViewportSourceMovie::source()
{
    return &m_videoCapture;
}

void ViewportSourceMovie::resend()
{
    if(m_timer.isActive())
    {
        return;
    }

    m_cvImageCopy.copyTo(m_cvImage);
    emit imageChanged(m_cvImage);
}

void ViewportSourceMovie::onTimer()
{
    readImage();
}

void ViewportSourceMovie::readImage()
{
    if(!m_videoCapture.read(m_cvImage))
    {
        m_timer.stop();
        return;
    }

    emit imageChanged(m_cvImage);
}
