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
    emit imageChanged(m_cvImage);
}

void ViewportSourceFile::close()
{

}
