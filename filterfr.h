#ifndef FILTER_FR_H
#define FILTER_FR_H

#include "filterbase.h"

#include <opencv2/objdetect.hpp>

class FilterFR : public FilterBase
{
    Q_OBJECT
public:
    FilterFR(QObject *parent = nullptr);
    ~FilterFR() override;

    void setData(void *data) override;

public slots:
    void addImage(cv::Mat &image) override;

private:
    void detectFace(cv::Mat &image);
    void initClassifiers();

private:
    cv::CascadeClassifier   m_cascade;
    cv::CascadeClassifier   m_nestedCascade;
};

#endif //FILTER_FR_H
