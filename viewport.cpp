#include "viewport.h"

ViewPort::ViewPort(QWidget *parent)  :
    QOpenGLWidget(parent)
{

}

ViewPort::~ViewPort()
{

}

void ViewPort::initializeGL()
{

}

void ViewPort::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderImage();
}

void ViewPort::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
}

void ViewPort::renderImage()
{

}
