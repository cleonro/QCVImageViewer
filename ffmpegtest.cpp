#include "ffmpegtest.h"

#include <QDebug>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

FfmpegTest::FfmpegTest(QObject *parent)
    : QObject(parent)
{

}

FfmpegTest::~FfmpegTest()
{

}

void FfmpegTest::init()
{
    qDebug() << avformat_license();

    // deprecated
    //av_register_all();
}
