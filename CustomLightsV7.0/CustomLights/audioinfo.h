#ifndef AUDIOINFO_H
#define AUDIOINFO_H

#include <stdlib.h>
#include <math.h>

#include <QAudioDeviceInfo>
#include <QAudioInput>
#include <qendian.h>

#include <QByteArray>
#include <QMainWindow>
#include <QScopedPointer>

class AudioInfo : public QIODevice
{
    Q_OBJECT

public:
    AudioInfo(const QAudioFormat &format);

    void start();
    void stop();

    inline int Get_levelAudio(){return promedio;}
    inline int Get_porcentajeAudio(){return promedio/327.68;}
    inline int Get_maxAudio(){return max_value;}
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;

private:

    const QAudioFormat m_format;
    float promedio;
    int max_value;

signals:
    void update(); // signal que informa cuando se termino de procesar la informacion

};


#endif // AUDIOINFO_H




