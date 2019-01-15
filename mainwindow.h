#ifndef _MAIN_WINDOW_H_
#define _MAIN_WINDOW_H_

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class ViewPort;
class QFileDialog;

class ViewportBase;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void onOpenActionTriggered();

private:
    void addControlWidgets();

private:
    Ui::MainWindow *ui;

    //ViewPort*       m_viewPort;
    ViewportBase *m_viewPort;
    QFileDialog     *m_fileDialog;
};

#endif //_MAIN_WINDOW_H_
