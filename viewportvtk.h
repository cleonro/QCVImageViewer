#ifndef VIEWPORT_VTK_H
#define VIEWPORT_VTK_H

#include "viewportbase.h"

class ViewportVTKWidget;

class ViewportVTK : public ViewportBase
{
    Q_OBJECT
public:
    ViewportVTK(QWidget *parent = nullptr);
    ~ViewportVTK();

    QWidget *viewport();

    void addImage(const cv::Mat &cvImage);

private:
    ViewportVTKWidget *m_widget;
};

#endif //VIEWPORT_VTK_H
