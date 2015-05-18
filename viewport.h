#ifndef _VIEW_PORT_H_
#define _VIEW_PORT_H_

#include <QOpenGLWidget>
#include <QImage>

//https://wiki.qt.io/OpenCV_with_Qt

class ViewPort : public QOpenGLWidget
{
    Q_OBJECT

public:
    ViewPort(QWidget *parent = 0);
    ~ViewPort();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    void renderImage();

private:
    QImage  m_image;

};

#endif //_VIEW_PORT_H_
