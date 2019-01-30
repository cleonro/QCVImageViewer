#ifndef VIEWPORT_SOURCE_FILE_H
#define VIEWPORT_SOURCE_FILE_H

#include "viewportsourcebase.h"

class ViewportSourceFile : public ViewportSourceBase
{
    Q_OBJECT
public:
    ViewportSourceFile(QObject *parent = nullptr);
    ~ViewportSourceFile() override;

    void open(void *source) override;
    void close() override;

private:
    cv::Mat m_cvImage;
};

#endif //VIEWPORT_SOURCE_FILE_H
