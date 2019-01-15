#include "viewportsourcecamera.h"

ViewportSourceCamera::ViewportSourceCamera(QObject *parent)
    : ViewportSourceBase (parent)
{
    m_cameraIndex = -1;
}

ViewportSourceCamera::~ViewportSourceCamera()
{

}

void ViewportSourceCamera::open(void *source)
{

}

void ViewportSourceCamera::close()
{
    m_videoCapture.release();
    m_cameraIndex = -1;
}
