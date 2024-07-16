#include "configuracionserie.h"

configuracionserie::configuracionserie()
{
    port = NULL;
    comunicacion_serie_ok = 0;
}


void configuracionserie::in_com_serie(string puerto){
    if(port){
        delete port;
        port = NULL;
    }
    if(!port){
        port_name = puerto.c_str();
        port_name.remove(port_name.length()-1,1); // saco el /n  que mete string
        port = new QSerialPort(port_name.toLatin1().data()); // hardcodeo puerto serie :)
        port->setBaudRate(QSerialPort::Baud115200);
        port->setParity(QSerialPort::NoParity);
        port->setFlowControl(QSerialPort::NoFlowControl);
        port->setDataBits(QSerialPort::Data8);
        port->setStopBits(QSerialPort::OneStop);
        if(!port->open(QIODevice::ReadWrite))
        {
            delete port;
            port = NULL;
            comunicacion_serie_ok=0;
        }
        else{
            comunicacion_serie_ok=1;
            qDebug() << "conectado";
           // connect(port,SIGNAL(readyRead()),this,SLOT(on_Port_rx()));
        }
    }
    else{
        delete port;
        port = NULL;
    }
}

void configuracionserie::mandar_datos (QByteArray datos){
    if(comunicacion_serie_ok==1)
        port->write(datos);
}

void configuracionserie::on_Port_rx(){
    QByteArray aux;
    aux.resize(port->bytesAvailable());
    port->read(aux.data(),aux.size());
    qDebug()<< "recibi: " << aux;

}

