#include "viewportcontroller.h"
#include "viewportopengl.h"
#include "viewportvtk.h"
#include "viewportsourcecamera.h"
#include "viewportsourcefile.h"
#include "filterinfo.h"
#include "filterbc.h"
#include "filterfr.h"

ViewportController::ViewportController(QWidget *parent)
    : QObject(parent)
    ,  m_parent(parent)
{
    m_viewport = nullptr;
    m_viewportSource = nullptr;
    m_brigtnessContrast[0] = 0.0;
    m_brigtnessContrast[1] = 1.0;
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
    foreach(FilterBase *filter, m_filters)
    {
        delete filter;
    }
    m_filters.clear();

    //viewport
    switch (type)
    {
        case OPENGL:
            m_viewport = new  ViewportOpenGL(m_parent);
            break;
        case VTK:
            m_viewport = new ViewportVTK(m_parent);
    }

    //filters
    m_filters.resize(FILTERS::COUNT);
    FilterBase *filter = new FilterInfo(this);
    m_filters[FILTERS::INFO] = filter;
    filter = new FilterBC(this);
    m_filters[FILTERS::BRIGHTNESS_CONTRAST] = filter;
    filter = new FilterFR(this);
    m_filters[FILTERS::FACE_RECOGNITION] = filter;


    for(int i = 0; i < m_filters.size() - 1; ++i)
    {
        connect(m_filters[i], &FilterBase::imageReady, m_filters[i + 1], &FilterBase::addImage);
    }
    connect(m_filters[FILTERS::COUNT - 1], &FilterBase::imageReady, this, &ViewportController::onImageChanged);


    return viewport();
}

bool ViewportController::openCamera(int cameraIndex)
{
    delete m_viewportSource;
    ViewportSourceCamera *sourceCamera = new ViewportSourceCamera(this);
    m_viewportSource = sourceCamera;

    if(m_filters.size() == 0)
    {
        connect(m_viewportSource, &ViewportSourceBase::imageChanged, this, &ViewportController::onImageChanged);
    }
    else
    {
        connect(m_viewportSource, &ViewportSourceBase::imageChanged, m_filters[0], &FilterBase::addImage);
        //m_filters[FILTERS::INFO]->setData(sourceCamera->source());
    }

    int ci = cameraIndex;
    m_viewportSource->open(static_cast<void*>(&ci));
    if(m_filters.size() > 0)
    {
        m_filters[FILTERS::INFO]->setData(sourceCamera->source());
        m_filters[FILTERS::BRIGHTNESS_CONTRAST]->setData(m_brigtnessContrast);
        m_filters[FILTERS::FACE_RECOGNITION]->setActive(true);
    }

    return true;
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
//        delete m_viewportSource;
//        m_viewportSource = nullptr;
        m_viewportSource->close();
    }
}

bool ViewportController::openImageFile(QString &fileName)
{
    delete m_viewportSource;
    m_viewportSource = new ViewportSourceFile(this);

    if(m_filters.size() == 0)
    {
        connect(m_viewportSource, &ViewportSourceBase::imageChanged, this, &ViewportController::onImageChanged);
    }
    else
    {
        connect(m_viewportSource, &ViewportSourceBase::imageChanged, m_filters[0], &FilterBase::addImage);
        m_filters[FILTERS::INFO]->setData(nullptr);
        m_filters[FILTERS::BRIGHTNESS_CONTRAST]->setData(m_brigtnessContrast);
        m_filters[FILTERS::FACE_RECOGNITION]->setActive(false);
    }

    m_viewportSource->open(&fileName);

    return true;
}

void ViewportController::setBrightnessContrast(const double &brightness, const double &contrast)
{
    m_brigtnessContrast[0] = brightness;
    m_brigtnessContrast[1] = contrast;

    if(m_filters.size() > 0)
    {
        m_filters[FILTERS::BRIGHTNESS_CONTRAST]->setData(m_brigtnessContrast);
        if(m_viewportSource != nullptr)
        {
            m_viewportSource->resend();
        }
    }
}
