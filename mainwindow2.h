#ifndef MAIN_WINDOW2_H
#define MAIN_WINDOW2_H

#include <QMainWindow>
#include <QList>

#define USE_VTK 1

namespace Ui {
class MainWindow2;
}

class QFileDialog;

class ViewportController;

class MainWindow2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow2(QWidget *parent = nullptr);
    ~MainWindow2();

signals:
    void imageFileOpened(QString filePath);

private slots:
    void onOpenActionTriggered();
    void onCameraOpened(const QString &cameraTitle);

private:
    void addControlWidgets();

private:
    Ui::MainWindow2 *ui;
    ViewportController *m_controller;

    QFileDialog     *m_fileDialog;
    QList<QString> m_imageExtensions;
    QList<QString> m_movieExtensions;
};

#endif // MAIN_WINDOW2_H
