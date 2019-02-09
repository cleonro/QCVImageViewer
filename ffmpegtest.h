#ifndef FFMPEG_TEST_H
#define FFMPEG_TEST_H

#include <QObject>

//http://dranger.com/ffmpeg/tutorial01.html

class FfmpegTest : public QObject
{
    Q_OBJECT
public:
    FfmpegTest(QObject *parent = nullptr);
    ~FfmpegTest();

    void init();

private:

};

#endif //FFMPEG_TEST_H
