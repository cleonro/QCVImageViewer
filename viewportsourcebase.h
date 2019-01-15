#ifndef VIEWPORT_SOURCE_BASE_H
#define VIEWPORT_SOURCE_BASE_H

#include <QObject>

class ViewportSourceBase : public QObject
{
    Q_OBJECT
public:
    ViewportSourceBase(QObject *parent = nullptr);
    virtual ~ViewportSourceBase();

    virtual void open(void *source);
    virtual void close();

signals:
    void imageChanged();

private:

};

#endif //VIEWPORT_SOURCE_BASE_H
