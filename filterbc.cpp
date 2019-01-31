#include "filterbc.h"

FilterBC::FilterBC(QObject *parent)
    : FilterBase(parent)
{
    m_brightness = 0.0;
    m_contrast = 1.0;
}

FilterBC::~FilterBC()
{

}

void FilterBC::setData(void *data)
{
    if(data == nullptr)
    {
        this->setActive(false);
        return;
    }

    double *bc = static_cast<double*>(data);
    m_brightness = bc[0];
    m_contrast = bc[1];
    this->setActive(true);
}

void FilterBC::addImage(cv::Mat &image)
{
    if(!this->isActive())
    {
        emit imageReady(image);
        return;
    }

    applyBrightnessContrast(image);
    emit imageReady(image);
}

void FilterBC::applyBrightnessContrast(cv::Mat &image)
{
    //v1
//    for( int y = 0; y < image.rows; y++ )
//    {
//        for( int x = 0; x < image.cols; x++ )
//        {
//            for( int c = 0; c < 3; c++ )
//            {
//                image.at<cv::Vec3b>(y,x)[c] = cv::saturate_cast<uchar>(
//                                                    m_contrast * (image.at<cv::Vec3b>(y,x)[c] ) + m_brightness
//                                                    );
//            }
//        }
//    }

    //v2
    image.convertTo(image, -1, m_contrast, m_brightness);

    //v3
//    image = m_contrast * (image + m_brightness);
}
