#include "viewportvtk.h"

////////////////////////////////////////////////////////////////////////////////////////

#include <QVTKOpenGLWidget.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkImageImport.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindowInteractor.h>

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

private:
    vtkSmartPointer<vtkRenderer> m_renderer;
    vtkSmartPointer<vtkImageImport> m_importer;
    vtkSmartPointer<vtkImageActor> m_actor;
    vtkSmartPointer<vtkInteractorStyleImage> m_interactorStyle;
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
    m_importer->SetWholeExtent(0, cvImage.cols - 1, 0, cvImage.rows - 1, 0, cvImage.channels() - 1);
    m_importer->SetDataExtentToWholeExtent();

    m_importer->SetNumberOfScalarComponents(cvImage.channels());
    m_importer->SetImportVoidPointer(cvImage.data);
    m_importer->Update();

    //m_actor->SetInputData(m_importer->GetOutput());
    m_renderer->ResetCamera();
    this->GetRenderWindow()->Render();
}

void ViewportVTKWidget::initImporter()
{
    m_importer = vtkSmartPointer<vtkImageImport>::New();

    m_importer->SetDataSpacing( 1, 1, 1 );
    m_importer->SetDataOrigin( 0, 0, 0 );


    m_importer->SetDataScalarTypeToUnsignedChar();

    m_actor = vtkSmartPointer<vtkImageActor>::New();
    m_actor->SetInputData(m_importer->GetOutput());
    m_renderer->AddActor(m_actor);
    m_renderer->ResetCamera();

    m_interactorStyle = vtkSmartPointer<vtkInteractorStyleImage>::New();
    this->GetRenderWindow()->GetInteractor()->SetInteractorStyle(m_interactorStyle);

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
