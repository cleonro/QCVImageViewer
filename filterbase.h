#ifndef FILTER_BASE_H
#define FILTER_BASE_H

#include <opencv2/core/mat.hpp>

#include <QObject>

class FilterBase : public QObject
{
    Q_OBJECT

public:
    FilterBase(QObject *parent = nullptr);
    virtual ~FilterBase();

    virtual void setData(void *data);

    void setActive(const bool &active);
    bool isActive();

public slots:
    virtual void addImage(cv::Mat &image);

signals:
    void imageReady(cv::Mat &image);

private:
    bool m_active;
};

#endif //FILTER_BASE_H
