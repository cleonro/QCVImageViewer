#ifndef VIEWPORT_SOURCE_BASE_H
#define VIEWPORT_SOURCE_BASE_H

#include <opencv2/core/mat.hpp>

#include <QObject>

class ViewportSourceBase : public QObject
{
    Q_OBJECT
public:
    ViewportSourceBase(QObject *parent = nullptr);
    virtual ~ViewportSourceBase();

    virtual void open(void *source);
    virtual void close();

    virtual void* source();

signals:
    void imageChanged(cv::Mat &cvImage);

private:

};

#endif //VIEWPORT_SOURCE_BASE_H
