#include "audioinfo.h"

AudioInfo::AudioInfo(const QAudioFormat &format): m_format(format)
{

}

void AudioInfo::start()// comienzo a escuchar lo que se manda
{
    open(QIODevice::WriteOnly);
}

void AudioInfo::stop()// cierro el canal
{
    close();
}

qint64 AudioInfo::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data)
    Q_UNUSED(maxlen)

    return 0;
}

/*
 * tomo los muestreos realizados y a partir de ellos obtengo el valor promedio y el valor maximo
*/
qint64 AudioInfo::writeData(const char *data, qint64 len)
{
    const char *ptr = data;
    const int channelBytes = m_format.sampleSize() / 8;
    const int sampleBytes = m_format.channelCount() * channelBytes;
    const int numSamples = len / sampleBytes;
    int value =0 ;
    double promedio_local=0;
    int max_value_local=0;

    for (int i = 0; i < numSamples; i++){
        for(int j=0; j < m_format.channelCount(); j++){
            value = qAbs(qFromLittleEndian<qint16>(ptr)); // pongo todos como positivos
            ptr+=channelBytes;
            promedio_local += value;
            if(max_value_local<value){
                max_value_local = value;
            }
         }
    }

    promedio = promedio_local/(numSamples * m_format.channelCount());  // promedio de los valores del paquete
    max_value = max_value_local; // maximo valor del paquete

    emit update(); // emito un update de que ya esta listo el paquete

    return len;
}
