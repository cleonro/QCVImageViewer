#ifndef VIEWPORT_OPENGL_H
#define VIEWPORT_OPENGL_H

#include "viewportbase.h"

#include <QOpenGLWidget>

class ViewportOpenGLWidget;

class ViewportOpenGL : public ViewportBase
{
    Q_OBJECT
public:
    ViewportOpenGL(QWidget *parent = nullptr);
    ~ViewportOpenGL();

    QWidget *viewport();

    void addImage(const cv::Mat &cvImage);

protected:


private:
    ViewportOpenGLWidget *m_widget;
};

#endif //VIEWPORT_OPENGL_H
