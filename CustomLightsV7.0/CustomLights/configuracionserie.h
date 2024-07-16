#ifndef CONFIGURACIONSERIE_H
#define CONFIGURACIONSERIE_H

//includes serial port
#include <string.h>
#include <QtSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <iostream>

using namespace std;

class configuracionserie
{
public:
    configuracionserie();
    void on_Port_rx();
    void in_com_serie( string puerto); // inicializacion del puerto serie
    inline bool estaConectado(){return comunicacion_serie_ok;}; // chequeo de estado de la conexion
    void mandar_datos (QByteArray datos); // funcion que permite enviar datos por el puerto serie que este configurado
    inline void cerrar_com(){delete port;}; // funcion que cierra el puerto serie que se haya abierto
private:
    QSerialPort *port;
    QString port_name;
    bool comunicacion_serie_ok;
};

#endif // CONFIGURACIONSERIE_H
