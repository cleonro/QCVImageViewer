#ifndef FFMPEG_TEST_H
#define FFMPEG_TEST_H

#include <QThread>
#include <QScopedPointer>

#include <memory>

//http://dranger.com/ffmpeg/tutorial01.html

extern "C"
{
    struct AVFormatContext;
    struct AVCodec;
    struct AVCodecParameters;
    struct AVCodecContext;
    struct AVFrame;
    struct AVPacket;
}

class QIODevice;
class QAudioOutput;

class FfmpegTest : public QObject
{
    Q_OBJECT
public:
    explicit FfmpegTest(QObject *parent = nullptr);
    ~FfmpegTest();

    void init();

    bool openStreamFile(QString &filePath);

signals:
    void startReadingAudioData();

private slots:
    void onStartReadingAudioData();

private:
    void readAudioData();
    int prepareAudioDataForOutput();
    bool needToStop();

    void initAudioOutput(int freq, int channels);
    void writeToAudioOutput(const char *buffer, qint64 length);

    void clear();

private:
    QThread m_thread;

    AVFormatContext *pFormatContext = nullptr;
    const AVCodec *pCodec = nullptr;
    AVCodecParameters *pCodecParameters = nullptr;
    AVCodecContext *pCodecContext = nullptr;

    AVFrame *pFrame = nullptr;
    AVPacket *pPacket = nullptr;

    int m_audio_stream_index = -1;
    int m_video_stream_index = -1;
    bool m_needToStop = false;

    // audio output
    QScopedPointer<QAudioOutput> m_audioOutput;
    QIODevice *m_audioDevice;
};

#endif //FFMPEG_TEST_H
