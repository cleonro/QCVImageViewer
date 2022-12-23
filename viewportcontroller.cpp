#include "viewportcontroller.h"
#include "viewportopengl.h"
#include "viewportvtk.h"
#include "viewportsourcecamera.h"
#include "viewportsourcefile.h"
#include "viewportsourcemovie.h"
#include "filterinfo.h"
#include "filterbc.h"
#include "filterfr.h"

ViewportController::ViewportController(QWidget *parent)
    : QObject(parent)
    , m_parent(parent)
    , m_viewport(nullptr)
    , m_viewportSource(nullptr)
    , m_brigtnessContrast{0.0, 1.0}
{

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
    m_filters.clear();

    //viewport
    switch (type)
    {
        case OPENGL:
            m_viewport.reset(new  ViewportOpenGL(m_parent));
            break;
        case VTK:
            m_viewport.reset(new ViewportVTK(m_parent));
    }

    //filters
    m_filters.resize(FILTERS::COUNT);
    m_filters[FILTERS::INFO].reset(new FilterInfo(this));
    m_filters[FILTERS::BRIGHTNESS_CONTRAST].reset(new FilterBC(this));
    m_filters[FILTERS::FACE_RECOGNITION].reset(new FilterFR(this));


    for(int i = 0; i < m_filters.size() - 1; ++i)
    {
        connect(m_filters[i].get(), &FilterBase::imageReady, m_filters[i + 1].get(), &FilterBase::addImage);
    }
    connect(m_filters[FILTERS::COUNT - 1].get(), &FilterBase::imageReady, this, &ViewportController::onImageChanged);


    return viewport();
}

bool ViewportController::openCamera(int cameraIndex)
{
    m_viewportSource.reset(new ViewportSourceCamera(this));

    if(m_filters.size() == 0)
    {
        connect(m_viewportSource.get(), &ViewportSourceBase::imageChanged, this, &ViewportController::onImageChanged);
    }
    else
    {
        connect(m_viewportSource.get(), &ViewportSourceBase::imageChanged, m_filters[0].get(), &FilterBase::addImage);
        //m_filters[FILTERS::INFO]->setData(sourceCamera->source());
    }

    int ci = cameraIndex;
    m_viewportSource->open(static_cast<void*>(&ci));
    if(m_filters.size() > 0)
    {
        m_filters[FILTERS::INFO]->setActive(true);
        m_filters[FILTERS::BRIGHTNESS_CONTRAST]->setActive(true);
        m_filters[FILTERS::FACE_RECOGNITION]->setActive(true);
        m_filters[FILTERS::INFO]->setData(m_viewportSource->source());
        m_filters[FILTERS::BRIGHTNESS_CONTRAST]->setData(m_brigtnessContrast);
        QVector<bool> state;
        state.resize(COUNT);
        state[BRIGHTNESS_CONTRAST] = true;
        state[FACE_RECOGNITION] = true;
        state[INFO] = true;
        emit filterStateChanged(state);
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
    if(dynamic_cast<ViewportSourceCamera*>(m_viewportSource.get()) != nullptr)
    {
//        delete m_viewportSource;
//        m_viewportSource = nullptr;
        m_viewportSource->close();
    }
}

bool ViewportController::openImageFile(QString &fileName)
{
    m_viewportSource.reset(new ViewportSourceFile(this));

    if(m_filters.size() == 0)
    {
        connect(m_viewportSource.get(), &ViewportSourceBase::imageChanged, this, &ViewportController::onImageChanged);
    }
    else
    {
        connect(m_viewportSource.get(), &ViewportSourceBase::imageChanged, m_filters[0].get(), &FilterBase::addImage);
        m_filters[FILTERS::INFO]->setActive(false);
        m_filters[FILTERS::BRIGHTNESS_CONTRAST]->setActive(true);
        m_filters[FILTERS::FACE_RECOGNITION]->setActive(false);
        m_filters[FILTERS::BRIGHTNESS_CONTRAST]->setData(m_brigtnessContrast);
        QVector<bool> state;
        state.resize(COUNT);
        state[BRIGHTNESS_CONTRAST] = true;
        state[FACE_RECOGNITION] = false;
        state[INFO] = false;
        emit filterStateChanged(state);
    }

    m_viewportSource->open(&fileName);

    return true;
}

bool ViewportController::openMovieFile(QString &fileName)
{
    m_viewportSource.reset(new ViewportSourceMovie(this));

    if(m_filters.size() == 0)
    {
        connect(m_viewportSource.get(), &ViewportSourceBase::imageChanged, this, &ViewportController::onImageChanged);
    }
    else
    {
        connect(m_viewportSource.get(), &ViewportSourceBase::imageChanged, m_filters[0].get(), &FilterBase::addImage);
    }

    m_viewportSource->open(static_cast<void*>(&fileName));
    if(m_filters.size() > 0)
    {
        m_filters[FILTERS::INFO]->setActive(true);
        m_filters[FILTERS::BRIGHTNESS_CONTRAST]->setActive(true);
        m_filters[FILTERS::FACE_RECOGNITION]->setActive(false);
        m_filters[FILTERS::INFO]->setData(m_viewportSource->source());
        m_filters[FILTERS::BRIGHTNESS_CONTRAST]->setData(m_brigtnessContrast);
        QVector<bool> state;
        state.resize(COUNT);
        state[BRIGHTNESS_CONTRAST] = true;
        state[FACE_RECOGNITION] = false;
        state[INFO] = true;
        emit filterStateChanged(state);
    }

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

void ViewportController::setFilterActive(const int &index, const bool &active)
{
    m_filters[index]->setActive(active);
}

void ViewportController::onAudioStreamTime(double time)
{
    static_cast<FilterInfo*>(m_filters[FILTERS::INFO].get())->onAudioTime(time);
}
