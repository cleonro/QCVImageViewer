#include "viewportopengl.h"

////////////////////////////////////////////////////////////////////////////////////////

#include <QOpenGLWidget>

class ViewportOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT
public:
    ViewportOpenGLWidget(QWidget *parent = nullptr);
    ~ViewportOpenGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    void renderImage();
    void resizeImage(int width, int height);

private:
    QImage m_image;
    QImage m_imageOriginal;
    int m_x;
    int m_y;
};

ViewportOpenGLWidget::ViewportOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    m_x = 0;
    m_y = 0;
}

ViewportOpenGLWidget::~ViewportOpenGLWidget()
{

}

void ViewportOpenGLWidget::initializeGL()
{
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

void ViewportOpenGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderImage();
}

void ViewportOpenGLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, 0, 1);
    glMatrixMode(GL_MODELVIEW);

    resizeImage(width, height);
    this->update();
}

void ViewportOpenGLWidget::renderImage()
{
    if(m_image.isNull())
    {
        return;
    }

    int w = m_image.width();
    int h = m_image.height();

    glLoadIdentity();
    glPushMatrix();

    glRasterPos2i(m_x, m_y);
    glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, m_image.bits());

    glPopMatrix();
}

void ViewportOpenGLWidget::resizeImage(int width, int height)
{
    if(m_imageOriginal.isNull())
    {
        return;
    }

    int w = m_imageOriginal.width();
    int h = m_imageOriginal.height();
    double imageRatio = (h + 0.0) / w;

    int resizedWidth = width;
    int resizedHeight = static_cast<int>(width / imageRatio);
    if(resizedHeight > height)
    {
        resizedWidth = static_cast<int>(height * imageRatio);
        resizedHeight = height;
    }

    m_x = (width - resizedWidth) / 2;
    m_y = (height - resizedHeight) / 2;

    if(w != width && h != height)
    {
        m_image = m_imageOriginal.scaled(QSize(resizedWidth, resizedHeight),
                                                                                        Qt::IgnoreAspectRatio,
                                                                                        Qt::SmoothTransformation
                                                                                    );
    }
    else
    {
        m_image = m_imageOriginal;
    }
}

////////////////////////////////////////////////////////////////////////////////////////

ViewportOpenGL::ViewportOpenGL(QWidget *parent)
    : ViewportBase (parent)
{
    m_widget = new ViewportOpenGLWidget(parent);
}

ViewportOpenGL::~ViewportOpenGL()
{

}

QWidget *ViewportOpenGL::viewport()
{
    return m_widget;
}

void ViewportOpenGL::addImage(const cv::Mat &cvImage)
{

}

#include "viewportopengl.moc"
