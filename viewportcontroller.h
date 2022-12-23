#ifndef VIEWPORT_CONTROLLER_H
#define VIEWPORT_CONTROLLER_H

#include <opencv2/highgui.hpp>

#include <QVector>
#include <QObject>

#include <memory>

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

    void onAudioStreamTime(double time);

signals:
    void filterStateChanged(QVector<bool> state);

private slots:
    void onImageChanged(const cv::Mat &cvImage);

private:
    QWidget *m_parent;
    std::unique_ptr<ViewportBase> m_viewport;
    std::unique_ptr<ViewportSourceBase> m_viewportSource;

    // Remark: QVector<std::unique_ptr<FilterBase>> can't use "resize".
    std::vector<std::unique_ptr<FilterBase>> m_filters;

    double m_brigtnessContrast[2];
};

#endif //VIEWPORT_CONTROLLER_H
