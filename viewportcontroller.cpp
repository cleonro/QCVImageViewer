#include "viewportcontroller.h"
#include "viewportopengl.h"
#include "viewportvtk.h"
#include "viewportsourcecamera.h"

ViewportController::ViewportController(QWidget *parent)
    : QObject(parent)
    ,  m_parent(parent)
{
    m_viewport = nullptr;
    m_viewportSource = nullptr;
}

ViewportController::~ViewportController()
{

}

QWidget *ViewportController::viewport()
{
    QWidget *w = m_viewport == nullptr ? nullptr : m_viewport->viewport();
    return w;
}

QWidget *ViewportController::initViewport(const ViewportController::ViewportType& type)
{
    delete m_viewport;

    switch (type)
    {
        case OPENGL:
            m_viewport = new  ViewportOpenGL(m_parent);
            break;
        case VTK:
            m_viewport = new ViewportVTK(m_parent);
    }

    return viewport();
}

void ViewportController::openCamera(int cameraIndex)
{
    Q_ASSERT(m_viewport != nullptr);
    delete m_viewportSource;

    m_viewportSource = new ViewportSourceCamera(this);
    connect(m_viewportSource, &ViewportSourceBase::imageChanged, this, &ViewportController::onImageChanged);
    int ci = cameraIndex;
    m_viewportSource->open((void*)(&ci));
}

void ViewportController::onImageChanged(const cv::Mat &cvImage)
{
    Q_ASSERT(m_viewport != nullptr);
    m_viewport->addImage(cvImage);
}

void ViewportController::closeCamera()
{
    if(dynamic_cast<ViewportSourceCamera*>(m_viewportSource) != nullptr)
    {
        delete m_viewportSource;
        m_viewportSource = nullptr;
    }
}
