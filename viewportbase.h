#ifndef VIEWPORT_BASE_H
#define VIEWPORT_BASE_H

#include <opencv2/core/mat.hpp>

#include <QObject>

class ViewportBase : public QObject
{
    Q_OBJECT
public:
    ViewportBase(QObject *parent = nullptr);
    virtual ~ViewportBase();

    virtual QWidget *viewport();

    virtual void addImage(const cv::Mat &cvImage);

protected:

private:

};

#endif //VIEWPORT_BASE_H
