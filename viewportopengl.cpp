#include "viewportopengl.h"

////////////////////////////////////////////////////////////////////////////////////////

#include <QOpenGLWidget>
#include <QGLWidget>

class ViewportOpenGLWidget : public QOpenGLWidget
{
    Q_OBJECT
    friend class ViewportOpenGL;
public:
    ViewportOpenGLWidget(QWidget *parent = nullptr);
    ~ViewportOpenGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    void addImage(const cv::Mat &cvImage);
    void renderImage();
    void resizeImage(int width, int height);
    void bindImage();

private:
    //QImage m_image;
    QImage m_imageOriginal;
    int m_x;
    int m_y;
    GLuint m_textureId;
    GLfloat m_vertices[8];
    GLfloat m_textCoord[8];
};

ViewportOpenGLWidget::ViewportOpenGLWidget(QWidget *parent)
    : QOpenGLWidget(parent)
    , m_vertices{0.0f, 0.0f,
                   0.0f, 1.0f,
                   1.0f, 1.0f,
                   1.0f, 0.0f}
    , m_textCoord{
                0.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 0.0f
          }
{
    m_x = 0;
    m_y = 0;
}

ViewportOpenGLWidget::~ViewportOpenGLWidget()
{

}

void ViewportOpenGLWidget::initializeGL()
{
    glClearColor(0.3f, 0.3f, 0.7f, 1.0f);

    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_TEXTURE_2D);
    glPixelZoom(1.0,1.0);

    glGenTextures(1, &m_textureId);

    //test-remove
    m_imageOriginal = QPixmap(":/test.jpg").toImage();
    if(!m_imageOriginal.isNull())
    {
        bindImage();
    }
    //
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
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);

    resizeImage(width, height);
    this->update();
}

void ViewportOpenGLWidget::renderImage()
{
//    if(m_image.isNull())
//    {
//        return;
//    }

//    int w = m_imageOriginal.width();
//    int h = m_imageOriginal.height();

    glLoadIdentity();
    glPushMatrix();

//    glRasterPos2i(m_x, m_y);
//    glDrawPixels(w, h, GL_RGBA, GL_UNSIGNED_BYTE, m_imageOriginal.bits());

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, m_vertices);
    glTexCoordPointer(2, GL_FLOAT, 0, m_textCoord);
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glDrawArrays(GL_QUADS, 0, 4);

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
    int resizedHeight = static_cast<int>(width * imageRatio);
    if(resizedHeight > height)
    {
        resizedWidth = static_cast<int>(height / imageRatio);
        resizedHeight = height;
    }

    m_x = (width - resizedWidth) / 2;
    m_y = (height - resizedHeight) / 2;

//    if(w != width && h != height)
//    {
//        m_image = m_imageOriginal.scaled(QSize(resizedWidth, resizedHeight),
//                                                                                        Qt::IgnoreAspectRatio,
//                                                                                        Qt::SmoothTransformation
//                                                                                    );
//    }
//    else
//    {
//        m_image = m_imageOriginal;
//    }

    m_vertices[0] = /*10;//*/m_x;
    m_vertices[1] = /*10;//*/m_y;
    m_vertices[2] = /*10;//*/m_x;
    m_vertices[3] = /*80;//*/m_y + resizedHeight;
    m_vertices[4] = /*80;//*/m_x + resizedWidth;
    m_vertices[5] = /*80;//*/m_y + resizedHeight;
    m_vertices[6] = /*80;//*/m_x + resizedWidth;
    m_vertices[7] = /*10;//*/m_y;
}

void ViewportOpenGLWidget::addImage(const cv::Mat &cvImage)
{
    int channels = cvImage.channels();

    if(channels == 3)
    {
        m_imageOriginal = QImage(static_cast<const unsigned char*>(cvImage.data),
                                     cvImage.cols, cvImage.rows,
                                     static_cast<int>(cvImage.step), QImage::Format_RGB888).rgbSwapped();
    }
    else if(channels == 1)
    {
        m_imageOriginal = QImage(static_cast<const unsigned char*>(cvImage.data),
                                      cvImage.cols, cvImage.rows,
                                      static_cast<int>(cvImage.step), QImage::Format_Indexed8);
    }
    else
    {
        return;
    }

    bindImage();
    resizeImage(this->width(), this->height());
    this->update();
}

void ViewportOpenGLWidget::bindImage()
{
    //TODO - replacement for QGLWidget::convertToGLFormat
    m_imageOriginal = QGLWidget::convertToGLFormat(m_imageOriginal);
    //not perfect - hue problems, keep convertToGLFormat
//    QMatrix m;
//    m.scale(1, -1);
//    m_imageOriginal = m_imageOriginal.transformed(m);

    int w = m_imageOriginal.width();
    int h = m_imageOriginal.height();
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_imageOriginal.bits());
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
    m_widget->addImage(cvImage);
}

#include "viewportopengl.moc"
