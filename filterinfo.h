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

    void onAudioTime(double time);

public slots:
    void addImage(cv::Mat &image) override;

private:
    void computeFPS();

private:
    void* m_data = nullptr;
    QString m_info;

    QElapsedTimer m_elpasedTimer;
    const unsigned int m_maxFramesForFPS = 10;
    unsigned long int m_framesCount = 0;
    unsigned long int m_audioFramesCount = 0;
    QString m_realFpsInfo;
    QString m_realTimeInfo;
    QString m_videoStreamTimeInfo;
    double m_fpsLastDuration = 0;
    double m_audioStreamTime = 0;
    QString m_audioStreamTimeInfo;
};

#endif //FILTER_INFO_H
