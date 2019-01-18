#include "viewportsourcecamera.h"

#include <QDebug>

ViewportSourceCamera::ViewportSourceCamera(QObject *parent)
    : ViewportSourceBase (parent)
{
    m_cameraIndex = -1;
    m_timer.setInterval(1000 / 30);
    connect(&m_timer, &QTimer::timeout, this, &ViewportSourceCamera::onTimer);
}

ViewportSourceCamera::~ViewportSourceCamera()
{
    close();
}

void ViewportSourceCamera::open(void *source)
{
    Q_ASSERT(source != nullptr);
    m_cameraIndex = *static_cast<int*>(source);
    if(m_cameraIndex < 0)
    {
        return;
    }

    openCamera();
    m_timer.start();
}

void ViewportSourceCamera::close()
{
    m_videoCapture.release();
    m_cameraIndex = -1;
    m_timer.stop();
}

void ViewportSourceCamera::onTimer()
{
    takeImage();
}

void ViewportSourceCamera::openCamera()
{
    m_videoCapture.open(m_cameraIndex);
    m_videoCapture.set(cv::CAP_PROP_FPS, 30);
    m_videoCapture.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    m_videoCapture.set(cv::CAP_PROP_FRAME_HEIGHT, 720);
}

void ViewportSourceCamera::takeImage()
{
    m_videoCapture >> m_cvImage;
    if(m_cvImage.empty())
    {
        qDebug() << "No video frame - reset frame size.";
        m_videoCapture.release();

        m_videoCapture.open(m_cameraIndex);
        m_videoCapture.set(cv::CAP_PROP_FPS, 30);
        m_videoCapture.set(cv::CAP_PROP_FRAME_WIDTH, 640);
        m_videoCapture.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
        return;
    }
    emit imageChanged(m_cvImage);
}
