#include "ffmpegtest.h"

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>

FfmpegTest::FfmpegTest(QObject *parent)
    : QObject(parent)
{

}

FfmpegTest::~FfmpegTest()
{

}

void FfmpegTest::init()
{
    av_register_all();
}
