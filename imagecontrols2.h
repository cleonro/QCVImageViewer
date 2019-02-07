#ifndef IMAGE_CONTROLS2_H
#define IMAGE_CONTROLS2_H

#include <QWidget>
#include <QVector>

namespace Ui {
class ImageControls2;
}

class ViewportController;
class QCheckBox;

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

    void onFilterStateChanged(QVector<bool> state);

    void on_cameraList_currentIndexChanged(int index);

    void on_cameraShow_stateChanged(int arg1);

    void on_cbBC_toggled(bool checked);

    void on_cbFR_toggled(bool checked);

    void on_cbInfo_toggled(bool checked);

private:
    void listCameras();

private:
    Ui::ImageControls2* ui;
    ViewportController *m_controller;

    double         m_brightness;
    double       m_contrast;

    int m_cameraIndex;

    QVector<QCheckBox*> m_filters;
};

#endif //IMAGE_CONTROLS2_H
