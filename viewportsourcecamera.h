#ifndef VIEWPORT_SOURCE_CAMERA_H
#define VIEWPORT_SOURCE_CAMERA_H

#include "viewportsourcebase.h"

#include <opencv2/highgui.hpp>

#include <QTimer>

class ViewportSourceCamera : public ViewportSourceBase
{
    Q_OBJECT
public:
    ViewportSourceCamera(QObject *parent = nullptr);
    ~ViewportSourceCamera();

    void open(void *source);
    void close();

private slots:
    void onTimer();

private:
    void openCamera();
    void takeImage();

private:
    QTimer m_timer;
    int m_cameraIndex;
    cv::VideoCapture m_videoCapture;
    cv::Mat m_cvImage;
};

#endif //VIEWPORT_SOURCE_CAMERA_H
