#ifndef CONFIGURACIONESP32_H
#define CONFIGURACIONESP32_H

#include "esp32udp.h"

#define NUMERO_MENSAJES_TIMEOUT 10

enum estados{RESET, ESPERANDO, BROADCAST_IPGET, CONFIGURACION_LISTA};

class ConfiguracionEsp32 : public Esp32Udp
{
private:
    // variables ME
    QString ip_esp32;
    uint8_t s_empezarConfiguracion, s_reiniciarConfiguracion, s_udpIp;
    uint8_t flag_ack;
    bool configurado;
    int mensajes_timeout;
    // metodos ME
    void m_broadcastIpget();
    void m_transmitoAck();
    void m_transmitoCmd01();

    // menajes udp
    void mensajesUdp();

public:
    explicit ConfiguracionEsp32(QObject *parent = nullptr, bool debug = false);

    inline void empezarConfig() { s_empezarConfiguracion = 1; };
    inline void desconectarWifi(){s_reiniciarConfiguracion = 1;};

    /*  Maquina de estados para la rutina de configuracion del ESP
     *  Dentro de la funcion esta la rutina de buscar mensajes UDP del dispositivo
     *  Solo se reciben dos tipos de mensajes: <ip> y <ack>
     *  Ejecutar cada 1 segundo
     */

    void ME_configuracion();

    inline bool estaConectado() { return configurado; };
    inline QString getIp(){return ip_esp32;};
    bool mandar_datos(QString datos);
};

#endif // CONFIGURACIONESP32_H
