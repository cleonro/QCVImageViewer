#ifndef VIEWPORT_OPENGL_H
#define VIEWPORT_OPENGL_H

#include "viewportbase.h"

class ViewportOpenGL : public ViewportBase
{
    Q_OBJECT
public:
    ViewportOpenGL(QObject *parent = nullptr);
    ~ViewportOpenGL();

    virtual QWidget *viewport();

    virtual void addImage(const cv::Mat &cvImage);

protected:


private:

};

#endif //VIEWPORT_OPENGL_H
