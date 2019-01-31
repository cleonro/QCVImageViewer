#ifndef IMAGE_CONTROLS2_H
#define IMAGE_CONTROLS2_H

#include <QWidget>

namespace Ui {
class ImageControls2;
}

class ViewportController;

class ImageControls2 : public QWidget
{
    Q_OBJECT

public:
    ImageControls2(QWidget* parent = nullptr);
    ~ImageControls2();

    int selectedCamera();

    void setController(ViewportController *controller);

signals:
    void cameraOpened(QString cameraTitle);

public slots:
    void onImageFileOpened();

private slots:
    void onSliderValueChanged(int value);
    void onReset();

    void on_cameraList_currentIndexChanged(int index);

    void on_cameraShow_stateChanged(int arg1);

private:
    void listCameras();

private:
    Ui::ImageControls2* ui;
    ViewportController *m_controller;

    double         m_brightness;
    double       m_contrast;

    int m_cameraIndex;
};

#endif //IMAGE_CONTROLS2_H
