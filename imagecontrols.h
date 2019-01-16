#ifndef IMAGE_CONTROLS_H
#define IMAGE_CONTROLS_H

#include <QWidget>

namespace Ui {
class ImageControls;
}

class ViewPort;
class ViewportController;

//http://docs.opencv.org/doc/tutorials/core/basic_linear_transform/basic_linear_transform.html
//http://opencv-srf.blogspot.ro/2013/07/change-brightness.html
//http://opencv-srf.blogspot.ro/2013/07/change-contrast-of-image-or-video.html

class ImageControls : public QWidget
{
    Q_OBJECT

public:
    ImageControls(QWidget* parent = nullptr);
    ~ImageControls();

    //void setImageViewPort(ViewPort* imageViewPort);
    int selectedCamera();

    void setController(ViewportController *controller);

signals:
    //void showCamera(bool);
    //void cameraIndexChanged(int);

private slots:
    void onSliderValueChanged(int value);
    void onReset();

    void on_cameraList_currentIndexChanged(int index);

    void on_cameraShow_stateChanged(int arg1);

private:
    void listCameras();

private:
    Ui::ImageControls* ui;
    //ViewPort*   m_imageViewPort;
    ViewportController *m_controller;

    int         m_brightness;
    float       m_contrast;

    int m_cameraIndex;
};

#endif //_IMAGE_CONTROLS_H
