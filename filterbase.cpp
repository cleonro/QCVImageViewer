#include "filterbase.h"

FilterBase::FilterBase(QObject *parent)
    : QObject(parent)
{

}

FilterBase::~FilterBase()
{

}

void FilterBase::setActive(const bool &active)
{
    m_active = active;
}

bool FilterBase::isActive()
{
    return m_active;
}

void FilterBase::setData(void *data)
{
    Q_UNUSED(data)
}

void FilterBase::addImage(cv::Mat &image)
{
    Q_UNUSED(image)
}
