#ifndef VIEWPORT_CONTROLLER_H
#define VIEWPORT_CONTROLLER_H

#include <opencv2/highgui.hpp>

#include <QObject>

class ViewportBase;
class ViewportSourceBase;

class ViewportController : public QObject
{
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

    bool openCamera(int cameraIndex);
    void closeCamera();

    bool openImageFile(QString& fileName);

private slots:
    void onImageChanged(const cv::Mat &cvImage);

private:
    QWidget *m_parent;
    ViewportBase *m_viewport;
    ViewportSourceBase *m_viewportSource;
};

#endif //VIEWPORT_CONTROLLER_H
