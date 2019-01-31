#ifndef FILTER_BC_H
#define FILTER_BC_H

#include "filterbase.h"

class FilterBC : public FilterBase
{
    Q_OBJECT

public:
    FilterBC(QObject *parent = nullptr);
    ~FilterBC() override;

    void setData(void *data) override;

public slots:
    void addImage(cv::Mat &image) override;

private:
    void applyBrightnessContrast(cv::Mat &image);

private:
    double m_brightness;
    double m_contrast;
};

#endif
