#ifndef _VIEW_PORT_H_
#define _VIEW_PORT_H_

#include <QOpenGLWidget>
#include <QImage>

#include <opencv2/highgui.hpp>

//https://wiki.qt.io/OpenCV_with_Qt

class ViewPort : public QOpenGLWidget
{
    Q_OBJECT

public:
    ViewPort(QWidget *parent = 0);
    ~ViewPort();

    bool openImageFile(const QString& fileName);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    void renderImage();
    void resizeImage(int width, int height);

private:
    QImage    m_renderImage;                // Qt image built from loaded cv image
    QImage    m_resizedRenderImage;   // resized image that will be rendered in the viewport
    cv::Mat     m_cvImage;                      // original image loaded with OpenCV

    int            m_resizedWidth;   // resized image width
    int            m_resizedHeight;  // resized image height
    float         m_imageRatio;     // height / width ratio
    int            m_positionX;        // top X position that allows the image to be rendered in the center of the viewport
    int            m_positionY;        // top Y position that allows the image to be rendered in the center of the viewport

};

#endif //_VIEW_PORT_H_
