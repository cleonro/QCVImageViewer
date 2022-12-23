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
        return;
    }

    m_data = data;
    cv::VideoCapture &videoCapture = *static_cast<cv::VideoCapture*>(m_data);
    double width = 0;
    double height = 0;
    double fps = 0;
    width = videoCapture.get(cv::CAP_PROP_FRAME_WIDTH);
    height = videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT);
    fps = videoCapture.get(cv::CAP_PROP_FPS);
    m_info = QString("[%1, %2] %3 FPS").arg(width).arg(height).arg(fps);
    this->setActive(true);
}

void FilterInfo::onAudioTime(double time)
{
    m_audioStreamTime = time;
    m_audioStreamTimeInfo = QString::number(m_audioStreamTime, 'f', 3) + " Audio Stream Time";
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

    computeFPS();
    cv::putText(image, m_realFpsInfo.toStdString(), cv::Point(30,50),
                cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(250,200,200));
    cv::putText(image, m_realTimeInfo.toStdString(), cv::Point(30,70),
                cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(250,200,200));
    cv::putText(image, m_videoStreamTimeInfo.toStdString(), cv::Point(30,90),
                cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(250,200,200));
    cv::putText(image, m_audioStreamTimeInfo.toStdString(), cv::Point(30,110),
                    cv::FONT_HERSHEY_COMPLEX_SMALL, 1.0, cv::Scalar(250,200,200));

    emit imageReady(image);
}

void FilterInfo::computeFPS()
{
    if(m_framesCount == 0)
    {
        m_elpasedTimer.start();
        ++m_framesCount;
        return;
    }

    double duration = m_elpasedTimer.elapsed();
    m_realTimeInfo = QString::number(duration / 1000.0, 'f', 3) + " Real Video Time";

    cv::VideoCapture &videoCapture = *static_cast<cv::VideoCapture*>(m_data);
    double videoStreamTime = videoCapture.get(cv::CAP_PROP_POS_MSEC);
    m_videoStreamTimeInfo = QString::number(videoStreamTime / 1000.0, 'f', 3) + " Video Stream Time";

    if(m_framesCount % m_maxFramesForFPS != 0)
    {
        ++m_framesCount;
        return;
    }

    double fps = (1000.0 * m_maxFramesForFPS) / (duration - m_fpsLastDuration);
    m_realFpsInfo = QString::number(fps, 'f', 3) + " Real FPS";
    m_fpsLastDuration = duration;
    ++m_framesCount;
}
