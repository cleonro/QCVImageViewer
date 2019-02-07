#ifndef VIEWPORT_SOURCE_MOVIE_H
#define VIEWPORT_SOURCE_MOVIE_H

#include "viewportsourcebase.h"

#include <opencv2/highgui.hpp>

#include <QTimer>

class ViewportSourceMovie : public ViewportSourceBase
{
    Q_OBJECT

public:
    ViewportSourceMovie(QObject *parent = nullptr);
    ~ViewportSourceMovie() override;

    void open(void *source) override;
    void close() override;

    void* source() override;
    void resend() override;

private slots:
    void onTimer();

private:
    void readImage();

private:
    QTimer m_timer;
    QString m_filePath;
    cv::VideoCapture m_videoCapture;
    cv::Mat m_cvImage;
    cv::Mat m_cvImageCopy;
};

#endif //VIEWPORT_SOURCE_MOVIE_H
