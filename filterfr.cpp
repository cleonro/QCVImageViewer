#include "filterfr.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <QDebug>

FilterFR::FilterFR(QObject *parent)
    : FilterBase(parent)
{
    initClassifiers();
}

FilterFR::~FilterFR()
{

}

void FilterFR::setData(void *data)
{
    Q_UNUSED(data)
}

void FilterFR::addImage(cv::Mat &image)
{
    if(!this->isActive())
    {
        emit imageReady(image);
        return;
    }

    detectFace(image);
    emit imageReady(image);
}

void FilterFR::initClassifiers()
{
    // Load classifiers from "opencv/data/haarcascades" directory
    std::string faceClassifierFile = "../data/haarcascades/haarcascade_frontalface_default.xml";
                                    //"../data/haarcascades/haarcascade_frontalcatface.xml"
    std::string eyeClassifierFile = "../data/haarcascades/haarcascade_eye.xml";
                                    //"../data/haarcascades/haarcascade_eye_tree_eyeglasses.xml"
    if(!m_nestedCascade.load(eyeClassifierFile))
    {
        qDebug() << "Nested cascade classifier not loaded";
    }
    if(!m_cascade.load(faceClassifierFile))
    {
       qDebug() << "Cascade classifier not loaded";
    }
}


void FilterFR::detectFace(cv::Mat &image)
{
    static double scale = 1.0;

    std::vector<cv::Rect> faces, faces2;
    cv::Mat gray, smallImg;

    cvtColor(image, gray, cv::COLOR_BGR2GRAY ); // Convert to Gray Scale
//    double fx = 1 / scale;

    // Resize the Grayscale Image
    //cv::resize(gray, smallImg, cv::Size(), fx, fx, cv::INTER_LINEAR );
    //cv::equalizeHist(smallImg, smallImg);

    // Detect faces of different sizes using cascade classifier
    //m_cascade.detectMultiScale(smallImg, faces, 1.1, 2, 0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30));
    m_cascade.detectMultiScale(gray, faces, 1.1, 2);

    // Draw circles around the faces
    for (size_t i = 0; i < faces.size(); ++i)
    {
        cv::Rect r = faces[i];
        cv::Mat smallImgROI;
        std::vector<cv::Rect> nestedObjects;
        cv::Point center;
        cv::Scalar color = cv::Scalar(255, 0, 0); // Color for Drawing tool
//        int radius;

//        double aspect_ratio = static_cast<double>(r.width/r.height);
//        if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
//        {
//            center.x = cvRound((r.x + r.width * 0.5) * scale);
//            center.y = cvRound((r.y + r.height * 0.5) * scale);
//            radius = cvRound((r.width + r.height) * 0.25 * scale);
//            cv::circle(m_cvImage, center, radius, color, 3, 8, 0);
//        }
//        else
//        {
            cv::rectangle(image, cv::Point(cvRound(r.x * scale), cvRound(r.y * scale)),
                    cv::Point(cvRound((r.x + r.width - 1) * scale),
                    cvRound((r.y + r.height - 1) * scale)), color, 3, 8, 0);
//        }

//        if(m_nestedCascade.empty())
//        {
//            continue;
//        }
//        smallImgROI = smallImg(r);

//        // Detection of eyes int the input image
//        m_nestedCascade.detectMultiScale( smallImgROI, nestedObjects, 1.1, 2,
//                                        0 | cv::CASCADE_SCALE_IMAGE, cv::Size(30, 30) );

//        // Draw circles around eyes
//        for ( size_t j = 0; j < nestedObjects.size(); j++ )
//        {
//            cv::Rect nr = nestedObjects[j];
//            center.x = cvRound((r.x + nr.x + nr.width * 0.5) * scale);
//            center.y = cvRound((r.y + nr.y + nr.height * 0.5) * scale);
//            radius = cvRound((nr.width + nr.height) * 0.25 * scale);
//            circle(m_cvImage, center, radius, color, 3, 8, 0);
//        }
    }
}
