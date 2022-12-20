#include "ffmpegtest.h"

#include <QFileInfo>
#include <QAudioOutput>
#include <QAudioFormat>
#include <QAudioDeviceInfo>
#include <QByteArray>
#include <QDebug>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
}

FfmpegTest::FfmpegTest(QObject *parent)
    : QObject(parent)
{
    this->moveToThread(&m_thread);

    connect(this, &FfmpegTest::startReadingAudioData, this, &FfmpegTest::onStartReadingAudioData);

    m_thread.start();
}

FfmpegTest::~FfmpegTest()
{
    m_needToStop = true;

    m_thread.quit();
    m_thread.wait();

    clear();
}

void FfmpegTest::init()
{
    qDebug() << "FFmpeg license: " << avformat_license();

    pFormatContext = avformat_alloc_context();
}

bool FfmpegTest::openStreamFile(QString &filePath)
{
    init();

    if(!QFileInfo::exists(filePath))
    {
        qDebug() << "FfmpegTest couldn't find file " << filePath;
        return false;
    }

    int status = avformat_open_input(&pFormatContext, filePath.toStdString().c_str(), nullptr, nullptr);
    if(status != 0)
    {
        qDebug() << "FfmpegTest couldn't open file " << filePath;
        return false;
    }

    status = avformat_find_stream_info(pFormatContext, nullptr);
    if(status < 0)
    {
        qDebug() << "FfmpegTest couldn't get the stream info from file " << filePath;
        return false;
    }

    int audioFreq = 0;
    int audioChannels = 0;

    m_audio_stream_index = -1;
    m_video_stream_index = -1;

    for(int i = 0; i < pFormatContext->nb_streams; ++i)
    {
        AVCodecParameters *pLocalCodecParameters = pFormatContext->streams[i]->codecpar;


        const AVCodec *pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);
        if(pLocalCodec == nullptr)
        {
            qDebug() << "FfmpegTest error: unsupported codec in file " << filePath;
            return false;
        }

        switch(pLocalCodecParameters->codec_type)
        {
        case AVMEDIA_TYPE_VIDEO:

            break;

        case AVMEDIA_TYPE_AUDIO:
            if(m_audio_stream_index == -1)
            {
                m_audio_stream_index = i;
                pCodec = pLocalCodec;
                pCodecParameters = pLocalCodecParameters;
            }

            audioFreq = pLocalCodecParameters->sample_rate;
            audioChannels = pLocalCodecParameters->ch_layout.nb_channels;

            break;

        default:

            break;
        }
    }

    pCodecContext = avcodec_alloc_context3(pCodec);
    if(pCodecContext == nullptr)
    {
        qDebug() << "failed to allocated memory for AVCodecContext";
        return false;
    }

    status = avcodec_parameters_to_context(pCodecContext, pCodecParameters);
    if(status < 0)
    {
        qDebug() << "failed to copy codec params to codec context";
        return false;
    }

    status = avcodec_open2(pCodecContext, pCodec, nullptr);
    if(status < 0)
    {
        qDebug() << "failed to open codec through avcodec_open2";
        return false;
    }

    pFrame = av_frame_alloc();
    if(pFrame == nullptr)
    {
        qDebug() << "failed to allocated memory for AVFrame";
        return false;
    }

    pPacket = av_packet_alloc();
    if(pPacket == nullptr)
    {
        qDebug() << "failed to allocated memory for AVPacket";
        return false;
    }

    initAudioOutput(audioFreq, audioChannels);

    qDebug() << Q_FUNC_INFO << " -> current thread: " << QThread::currentThread();
    emit startReadingAudioData();

    return true;
}

void FfmpegTest::onStartReadingAudioData()
{
    qDebug() << Q_FUNC_INFO << " -> current thread: " << QThread::currentThread()
             << "; working thread: " << &m_thread;

    readAudioData();
}

void FfmpegTest::readAudioData()
{
    int frameRead = 0;
    while(frameRead >= 0 && !needToStop())
    {
        // 1 - read frame
        frameRead = av_read_frame(pFormatContext, pPacket);
        if(frameRead < 0)
        {
            // error or stream end
            continue;
        }

        int packet_stream_index = pPacket->stream_index;
        if(packet_stream_index != m_audio_stream_index)
        {
            // not audio data
            continue;
        }

        // 2 - send frame for decoding
        int response = avcodec_send_packet(pCodecContext, pPacket);
        if(response < 0)
        {
            // error
            continue;
        }

        // 3 - receive decoded frame
        response = avcodec_receive_frame(pCodecContext, pFrame);
        if(response < 0)
        {
            // error
            continue;
        }

        prepareAudioDataForOutput();

        av_packet_unref(pPacket);
    }

    clear();
}

int FfmpegTest::prepareAudioDataForOutput()
{
    int response = 0;
    int data_size = av_samples_get_buffer_size(nullptr,
                               pCodecContext->ch_layout.nb_channels,
                               pFrame->nb_samples,
                               pCodecContext->sample_fmt,
                               1);

    QByteArray byteArray;
    byteArray.reserve(data_size);

    // recalculate this for correct sample format
    for(int i = 0; i < pFrame->nb_samples; ++i)
    {
        for(int j = 0; j < pCodecContext->ch_layout.nb_channels; ++j)
        {
            for(int k = 0; k < 4; ++k)
            {
                int index = i * 4 + k;
                byteArray.push_back(pFrame->data[j][index]);
            }
        }
    }

    int q_data_size = byteArray.size();
    writeToAudioOutput(byteArray.data(), q_data_size);

    return response;
}

bool FfmpegTest::needToStop()
{
    return m_needToStop;
}

void FfmpegTest::initAudioOutput(int freq, int channels)
{
    int sampleRate = freq;
    int channelCount = channels;
    int sampleSize = 32; //bits per sample

    QAudioDeviceInfo deviceInfo = QAudioDeviceInfo::defaultOutputDevice();

    QAudioFormat format;
    format.setSampleRate(sampleRate);
    format.setChannelCount(channelCount);
    format.setSampleSize(sampleSize);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::Float);

    if(!deviceInfo.isFormatSupported(format))
    {
        qDebug() << "Audio format not supported!";
        format = deviceInfo.nearestFormat(format);
    }

    m_audioOutput.reset(new QAudioOutput(deviceInfo, format));
    m_audioOutput->setBufferSize(10000);
    m_audioDevice = m_audioOutput->start();
}

void FfmpegTest::writeToAudioOutput(const char *buffer, qint64 length)
{
    int bufferSize = m_audioOutput->bufferSize();
    int perSize = m_audioOutput->periodSize();
    (void)bufferSize;
    (void)perSize;

    int bytesFree = m_audioOutput->bytesFree();
    int totalWrittenBytes = 0;
    while(totalWrittenBytes < length)
    {
        int lengthLeft = length - totalWrittenBytes;
        int bytesToWrite = bytesFree > lengthLeft ? lengthLeft : bytesFree;

        int bytesWritten = m_audioDevice->write(buffer + totalWrittenBytes, bytesToWrite);
        totalWrittenBytes += bytesWritten;
        bytesFree = m_audioOutput->bytesFree();
    }
}

void FfmpegTest::clear()
{
    av_packet_free(&pPacket);
    av_frame_free(&pFrame);

    avformat_close_input(&pFormatContext);
    avformat_free_context(pFormatContext);

    pFormatContext = nullptr;
    pCodec = nullptr;
    pCodecParameters = nullptr;
    pCodecContext = nullptr;

    pFrame = nullptr;
    pPacket = nullptr;

    m_audio_stream_index = -1;
    m_video_stream_index = -1;
    m_needToStop = false;
}
