#ifndef FILTER_INFO_H
#define FILTER_INFO_H

#include "filterbase.h"

#include <QString>

class FilterInfo : public FilterBase
{
    Q_OBJECT

public:
    FilterInfo(QObject *parent = nullptr);
    ~FilterInfo() override;

    void setData(void *data) override;

public slots:
    void addImage(cv::Mat &image) override;

private:
    QString m_info;
};

#endif //FILTER_INFO_H
