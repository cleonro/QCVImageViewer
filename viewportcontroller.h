#ifndef VIEWPORT_CONTROLLER_H
#define VIEWPORT_CONTROLLER_H

#include <opencv2/highgui.hpp>

#include <QVector>
#include <QObject>

class ViewportBase;
class ViewportSourceBase;
class FilterBase;

class ViewportController : public QObject
{
    enum FILTERS {
        BRIGHTNESS_CONTRAST = 0,
        INFO = 1,
        COUNT = 2
    };

    Q_OBJECT
public:
    enum ViewportType
    {
        OPENGL = 0,
        VTK = 1
    };

    enum ViewportSourceType
    {
        CAMERA = 0
    };

public:
    ViewportController(QWidget *parent = nullptr);
    ~ViewportController();

    QWidget *initViewport(const ViewportType& type);
    QWidget *viewport();

    void setBrightnessContrast(const double &brightness, const double &contrast);

    bool openCamera(int cameraIndex);
    void closeCamera();

    bool openImageFile(QString& fileName);

private slots:
    void onImageChanged(const cv::Mat &cvImage);

private:
    QWidget *m_parent;
    ViewportBase *m_viewport;
    ViewportSourceBase *m_viewportSource;
    QVector<FilterBase*> m_filters;
    double m_brigtnessContrast[2];
};

#endif //VIEWPORT_CONTROLLER_H
