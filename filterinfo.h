#ifndef FILTER_INFO_H
#define FILTER_INFO_H

#include "filterbase.h"

#include <QString>
#include <QElapsedTimer>

class FilterInfo : public FilterBase
{
    Q_OBJECT

public:
    explicit FilterInfo(QObject *parent = nullptr);
    ~FilterInfo() override;

    void setData(void *data) override;

public slots:
    void addImage(cv::Mat &image) override;

private:
    void computeFPS();

private:
    QString m_info;

    QElapsedTimer m_elpasedTimer;
    const unsigned int m_maxFramesForFPS = 10;
    unsigned int m_framesForFPS = 0;
    QString m_realFpsInfo;
};

#endif //FILTER_INFO_H
