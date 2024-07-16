#include "configuracionesp32.h"

ConfiguracionEsp32::ConfiguracionEsp32(QObject *parent, bool debug) : Esp32Udp(parent, debug)
{
    // inicializacion ME
    ip_esp32 = "NO_IP";
    s_empezarConfiguracion = 0;
    s_reiniciarConfiguracion = 0;
    s_udpIp = 0;
    configurado = 0;
    flag_ack=0;
    mensajes_timeout=0;
}

void ConfiguracionEsp32::m_broadcastIpget()
{
    Comando cmd = {"ipget", "0"};
    this->enviarBroadcast(cmd);
}

void ConfiguracionEsp32::m_transmitoAck()
{
    Comando cmd = {"ack", "00"};
    this->enviarComando(cmd);
}

void ConfiguracionEsp32::m_transmitoCmd01()
{
    Comando cmd = {"cmd", "01"};
    this->enviarComando(cmd);
}

void ConfiguracionEsp32::ME_configuracion()
{
    static uint8_t estado = RESET;

    mensajesUdp();

    switch(estado)
    {
        case RESET:
            configurado = 0;
            estado = ESPERANDO;
            break;

        case ESPERANDO:
            if(s_empezarConfiguracion)
            {
                s_empezarConfiguracion = 0;
                m_broadcastIpget();
                m_transmitoCmd01();
                estado = BROADCAST_IPGET;
            }
            break;

        case BROADCAST_IPGET:
            if(s_udpIp)
            {
                s_udpIp = 0;
                m_transmitoAck();
                configurado = 1;
                estado = CONFIGURACION_LISTA;
            }
            else
            {
                m_broadcastIpget();
            }
            break;

        case CONFIGURACION_LISTA:
            if((s_reiniciarConfiguracion) || (mensajes_timeout >= NUMERO_MENSAJES_TIMEOUT ))
            {
                s_reiniciarConfiguracion = 0;
                mensajes_timeout=0;
                m_transmitoCmd01();
                configurado = 0;
                estado = BROADCAST_IPGET;

            }
            if(flag_ack==1){
                flag_ack = 0;
                mensajes_timeout = 0;
            }
            else
                mensajes_timeout++;

            break;

        default:
            configurado = 0;
            estado = ESPERANDO;
    }
}

void ConfiguracionEsp32::mensajesUdp()
{
    Comando cmd;

    if(this->recibirComando(cmd))
    {
        if(cmd.direccion == "ip")
        {
            s_udpIp = 1;
            ip_esp32 = QString::fromStdString(cmd.argumento);
            this->setHostIp(ip_esp32);
        }
        else if(cmd.direccion == "ack") // no implementado un uso para este comando actualmente
        {
            flag_ack=1;
            borrarComandos();
        }
    }
}

bool ConfiguracionEsp32::mandar_datos(QString data)
{
    if(!configurado)
    {
        return false;
    }

    Comando cmd = {"data", data.toStdString()};

    this->enviarComando(cmd);

    return true;
}


