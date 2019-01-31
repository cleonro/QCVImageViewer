#include "viewportsourcefile.h"

#include <opencv2/highgui.hpp>

ViewportSourceFile::ViewportSourceFile(QObject *parent)
    : ViewportSourceBase(parent)
{

}

ViewportSourceFile::~ViewportSourceFile()
{

}

void ViewportSourceFile::open(void *source)
{
    QString *path = static_cast<QString*>(source);
    m_cvImage = cv::imread(path->toStdString());
    m_cvImage.copyTo(m_cvImageCopy);
    emit imageChanged(m_cvImage);
}

void ViewportSourceFile::close()
{

}

void ViewportSourceFile::resend()
{
    m_cvImageCopy.copyTo(m_cvImage);
    emit imageChanged(m_cvImage);
}
