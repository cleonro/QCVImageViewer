#ifndef _IMAGE_CONTROLS_H_
#define _IMAGE_CONTROLS_H_

#include <QWidget>

namespace Ui {
class ImageControls;
}

class ViewPort;

//http://docs.opencv.org/doc/tutorials/core/basic_linear_transform/basic_linear_transform.html
//http://opencv-srf.blogspot.ro/2013/07/change-brightness.html
//http://opencv-srf.blogspot.ro/2013/07/change-contrast-of-image-or-video.html

class ImageControls : public QWidget
{
public:
    ImageControls(QWidget* parent = nullptr);
    ~ImageControls();

    void setImageViewPort(ViewPort* imageViewPort);

private:
    Ui::ImageControls* ui;
    ViewPort*   m_imageViewPort;
};

#endif //_IMAGE_CONTROLS_H_
