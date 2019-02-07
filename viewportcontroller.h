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
    Q_OBJECT

public:
    enum FILTERS {
        BRIGHTNESS_CONTRAST = 0,
        FACE_RECOGNITION = 1,
        INFO = 2,
        COUNT = 3
    };

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

    bool openImageFile(QString &fileName);
    bool openMovieFile(QString &fileName);

    void setFilterActive(const int &index, const bool &active);

signals:
    void filterStateChanged(QVector<bool> state);

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
