#include "viewportvtk.h"

////////////////////////////////////////////////////////////////////////////////////////

#include <QVTKOpenGLNativeWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageData.h>
#include <vtkCamera.h>

class ViewportVTKWidget : public QVTKOpenGLNativeWidget
{
    Q_OBJECT
    friend class ViewportVTK;
public:
    ViewportVTKWidget(QWidget *parent = nullptr);
    ~ViewportVTKWidget();

protected:
    void resizeGL(int width, int height);

private:
    void addImage(const cv::Mat &cvImage);
    void resizeImage(int width, int height);

    void init();
    void convertTovtkImageData1(const cv::Mat& cvImage);
    void convertTovtkImageData2(const cv::Mat& cvImage);

private:
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkImageActor> m_actor;
    vtkSmartPointer<vtkInteractorStyleImage> m_interactorStyle;
    vtkSmartPointer<vtkImageData> m_imageData;
};

ViewportVTKWidget::ViewportVTKWidget(QWidget *parent)
    : QVTKOpenGLNativeWidget(parent)
{
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renWin = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    this->SetRenderWindow(renWin);
    m_renderer =  vtkSmartPointer<vtkRenderer>::New();
    renWin->AddRenderer(m_renderer);
    m_renderer->SetBackground(0.3, 0.3, 0.7);

    this->setAttribute(Qt::WA_DontCreateNativeAncestors);

    init();
}

ViewportVTKWidget::~ViewportVTKWidget()
{

}

void ViewportVTKWidget::addImage(const cv::Mat &cvImage)
{
    convertTovtkImageData1(cvImage);
    m_actor->SetInputData(m_imageData);

    //m_renderer->ResetCamera();
    resizeImage(this->width(), this->height());
    this->GetRenderWindow()->Render();
}

void ViewportVTKWidget::init()
{
    m_actor = vtkSmartPointer<vtkImageActor>::New();

    m_imageData = vtkSmartPointer<vtkImageData>::New();
    m_actor->SetInputData(m_imageData);

    m_renderer->AddActor(m_actor);
    vtkCamera* camera = m_renderer->GetActiveCamera();
    camera->ParallelProjectionOn();
    //m_renderer->ResetCamera();

    m_interactorStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
    this->GetRenderWindow()->GetInteractor()->SetInteractorStyle(m_interactorStyle);

}

void ViewportVTKWidget::convertTovtkImageData1(const cv::Mat &cvImage)
{
    m_imageData->SetDimensions(cvImage.cols, cvImage.rows, 1);
    m_imageData->SetSpacing(1,1,1);

    int channels = cvImage.channels();
    m_imageData->AllocateScalars(VTK_UNSIGNED_CHAR, channels);

    if (channels == 3)
    {
        for(int i = 0; i < cvImage.cols; ++i)
        {
            for(int j = 0; j < cvImage.rows; ++j)
            {
                int mj = cvImage.rows - 1 - j;
                unsigned char* d = reinterpret_cast<unsigned char*> (m_imageData->GetScalarPointer(i, j, 0));
                *d = cvImage.at<cv::Vec3b>(mj, i)[2];
                *(d + 1) = cvImage.at<cv::Vec3b>(mj, i)[1];
                *(d + 2) = cvImage.at<cv::Vec3b>(mj, i)[0];
            }
        }
    }
    if (channels == 1)
    {
        for(int i = 0; i < cvImage.cols; ++i)
        {
            for(int j = 0; j < cvImage.rows; ++j)
            {
                int mj = cvImage.rows - 1 - j;
                unsigned char* d = reinterpret_cast<unsigned char*> (m_imageData->GetScalarPointer(i, j, 0));
                *d = cvImage.at<uchar>(mj, i);
            }
        }
    }

    m_imageData->Modified();
}

void ViewportVTKWidget::convertTovtkImageData2(const cv::Mat &cvImage)
{
    vtkSmartPointer<vtkImageData> retval = vtkSmartPointer<vtkImageData>::New();

    retval->SetDimensions(cvImage.cols, cvImage.rows, 1);
    retval->SetSpacing(1,1,1);

    retval->AllocateScalars(VTK_UNSIGNED_CHAR, cvImage.channels());

    unsigned char* dest = reinterpret_cast<unsigned char*> (retval->GetScalarPointer());
    uchar* src = cvImage.data;
    int N = cvImage.rows * cvImage.cols;

    if (cvImage.channels() == 3)
    {
        if (cvImage.isContinuous())
        {
            // 3-channel continous colors
            for (int i = 0; i < N; ++i)
            {
                *dest++ = src[2]; // R
                *dest++ = src[1]; // G
                *dest++ = src[0]; // B
                src += 3;
            }
        }
        else
        {
//			std::cout << "noncontinous conversion, rows=" << size[1] << std::endl;
            for (int i=0; i < cvImage.rows; ++i)
            {
                 const uchar* src = cvImage.ptr<uchar>(i);
                 for (int j=0; j < cvImage.cols; ++j)
                 {
                        *dest++ = src[2];
                        *dest++ = src[1];
                        *dest++ = src[0];
                        src += 3;
                 }
            }
        }
//		colorFormat = "ARGB";
    }
    if (cvImage.channels() == 1)
    {
        if (!cvImage.isContinuous())
        {
            std::cout << "Error: Non-continous frame data." << std::endl;
            return;
        }

        // BW camera
        for (int i = 0; i < N; ++i)
        {
            *dest++ = *src++;
        }
//		colorFormat = "R";
    }

    m_imageData = retval;
}

void ViewportVTKWidget::resizeGL(int width, int height)
{
    QVTKOpenGLNativeWidget::resizeGL(width, height);
    resizeImage(width, height);
}

void ViewportVTKWidget::resizeImage(int width, int height)
{
    int extent[6];
    double origin[3];
    double spacing[3];

    m_imageData->GetExtent(extent);
    m_imageData->GetOrigin(origin);
    m_imageData->GetSpacing(spacing);

    vtkCamera *camera = m_renderer->GetActiveCamera();
    double xc = origin[0] + 0.5 * (extent[0] + extent[1]) * spacing[0];
    double yc = origin[1] + 0.5 * (extent[2] + extent[3]) * spacing[1];
    double xd = (extent[1] - extent[0] + 1) * spacing[0];
    double yd = (extent[3] - extent[2] + 1) * spacing[1];
    double imageRatio = yd / xd;
    double d = camera->GetDistance();
    if(imageRatio * width > height)
    {
        camera->SetParallelScale(0.5 * yd);
    }
    else
    {
        camera->SetParallelScale(0.5 * xd * (height + 0.0) / width);
    }
    camera->SetFocalPoint(xc, yc, 0.0);
    camera->SetPosition(xc, yc, +d);
}

////////////////////////////////////////////////////////////////////////////////////////

ViewportVTK::ViewportVTK(QWidget *parent)
    : ViewportBase(parent)
{
    m_widget = new ViewportVTKWidget(parent);
}

ViewportVTK::~ViewportVTK()
{

}

QWidget *ViewportVTK::viewport()
{
    return m_widget;
}

void ViewportVTK::addImage(const cv::Mat &cvImage)
{
    m_widget->addImage(cvImage);
}

#include "viewportvtk.moc"
