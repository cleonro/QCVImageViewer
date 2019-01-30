#include "filterinfo.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

FilterInfo::FilterInfo(QObject *parent)
    : FilterBase(parent)
{

}

FilterInfo::~FilterInfo()
{

}

void FilterInfo::setData(void *data)
{
    if(data == nullptr)
    {
        this->setActive(false);
        m_info.clear();
    }

    cv::VideoCapture &videoCapture = *static_cast<cv::VideoCapture*>(data);
    double width = 0;
    double height = 0;
    double fps = 0;
    width = videoCapture.get(cv::CAP_PROP_FRAME_WIDTH);
    height = videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT);
    fps = videoCapture.get(cv::CAP_PROP_FPS);
    m_info = QString("[%1, %2] %3FPS").arg(width).arg(height).arg(fps);
    this->setActive(true);
}

void FilterInfo::addImage(cv::Mat &image)
{
    if(!this->isActive())
    {
        emit imageReady(image);
        return;
    }

    cv::putText(image, m_info.toStdString(), cv::Point(30,30),
                cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(250,200,200));
    emit imageReady(image);
}
