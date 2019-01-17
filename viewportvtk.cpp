#include "viewportvtk.h"

////////////////////////////////////////////////////////////////////////////////////////

#include <QVTKOpenGLWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageData.h>

class ViewportVTKWidget : public QVTKOpenGLWidget
{
    Q_OBJECT
    friend class ViewportVTK;
public:
    ViewportVTKWidget(QWidget *parent = nullptr);
    ~ViewportVTKWidget();

private:
    void addImage(const cv::Mat &cvImage);

    void initImporter();
    void convertTovtkImageData(const cv::Mat& frame);

private:
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkImageActor> m_actor;
    vtkSmartPointer<vtkInteractorStyleImage> m_interactorStyle;
    vtkSmartPointer<vtkImageData> m_imageData;
};

ViewportVTKWidget::ViewportVTKWidget(QWidget *parent)
    : QVTKOpenGLWidget(parent)
{
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renWin = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    this->SetRenderWindow(renWin);
    m_renderer =  vtkSmartPointer<vtkRenderer>::New();
    renWin->AddRenderer(m_renderer);
    m_renderer->SetBackground(0.3, 0.3, 0.7);

    this->setAttribute(Qt::WA_DontCreateNativeAncestors);

    initImporter();
}

ViewportVTKWidget::~ViewportVTKWidget()
{

}

void ViewportVTKWidget::addImage(const cv::Mat &cvImage)
{
    convertTovtkImageData(cvImage);
    m_actor->SetInputData(m_imageData);

    m_renderer->ResetCamera();
    this->GetRenderWindow()->Render();
}

void ViewportVTKWidget::initImporter()
{
    m_actor = vtkSmartPointer<vtkImageActor>::New();

    m_imageData = vtkSmartPointer<vtkImageData>::New();
    m_actor->SetInputData(m_imageData);

    m_renderer->AddActor(m_actor);
    m_renderer->ResetCamera();

    m_interactorStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
    this->GetRenderWindow()->GetInteractor()->SetInteractorStyle(m_interactorStyle);

}

void ViewportVTKWidget::convertTovtkImageData(const cv::Mat &frame)
{
    vtkSmartPointer<vtkImageData> retval = vtkSmartPointer<vtkImageData>::New();

    retval->SetDimensions(frame.cols, frame.rows, 1);
    retval->SetSpacing(1,1,1);

    retval->AllocateScalars(VTK_UNSIGNED_CHAR, frame.channels());

    unsigned char* dest = reinterpret_cast<unsigned char*> (retval->GetScalarPointer());
    uchar* src = frame.data;
    int N = frame.rows * frame.cols;

    if (frame.channels() == 3)
    {
        if (frame.isContinuous())
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
            for (int i=0; i < frame.rows; ++i)
            {
                 const uchar* src = frame.ptr<uchar>(i);
                 for (int j=0; j < frame.cols; ++j)
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
    if (frame.channels() == 1)
    {
        if (!frame.isContinuous())
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
