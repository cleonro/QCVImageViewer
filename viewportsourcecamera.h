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
    ~ViewportSourceCamera() override;

    void open(void *source) override;
    void close() override;

    void* source() override;
    virtual void resend() override;

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
    cv::Mat m_cvImageCopy;
};

#endif //VIEWPORT_SOURCE_CAMERA_H
