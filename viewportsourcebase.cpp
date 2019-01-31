#include "viewportsourcebase.h"

ViewportSourceBase::ViewportSourceBase(QObject *parent)
    : QObject(parent)
{

}

ViewportSourceBase::~ViewportSourceBase()
{

}

void ViewportSourceBase::open(void *source)
{
    Q_UNUSED(source)
}

void ViewportSourceBase::close()
{

}

void *ViewportSourceBase::source()
{
    return nullptr;
}

void ViewportSourceBase::resend()
{

}
