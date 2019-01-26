#ifndef VIEWPORT_SOURCE_FILE_H
#define VIEWPORT_SOURCE_FILE_H

#include "viewportsourcebase.h"

class ViewportSourceFile : public ViewportSourceBase
{
    Q_OBJECT
public:
    ViewportSourceFile(QObject *parent = nullptr);
    ~ViewportSourceFile();

    void open(void *source);
    void close();

private:
    cv::Mat m_cvImage;
};

#endif //VIEWPORT_SOURCE_FILE_H
