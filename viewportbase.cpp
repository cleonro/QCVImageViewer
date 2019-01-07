#include "viewportbase.h"

ViewportBase::ViewportBase(QObject *parent)
    : QObject(parent)
{

}

ViewportBase::~ViewportBase()
{

}

QWidget *ViewportBase::viewport()
{
    return nullptr;
}

void ViewportBase::addImage(const cv::Mat &cvImage)
{
    Q_UNUSED(cvImage)
}
