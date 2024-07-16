#include "esp32udp.h"

using namespace std;

Esp32Udp::Esp32Udp(QObject *parent, bool debug) : QObject(parent)
{
    socket = new QUdpSocket(this);

    socket->bind(udp_port);

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()));

    this->host_ip = "NO_IP";

    this->debug = debug;
}

string Esp32Udp::crearComando(const Comando c)
{
    string cmd = "";

    cmd += "<";
    cmd += c.direccion;
    cmd += ">";
    cmd += c.argumento;
    cmd += "</";
    cmd += c.direccion;
    cmd += ">";

    return cmd;
}

string Esp32Udp::crearComando(const string direccion, const string argumento)
{
    string cmd = "";

    cmd += "<";
    cmd += direccion;
    cmd += ">";
    cmd += argumento;
    cmd += "</";
    cmd += direccion;
    cmd += ">";

    return cmd;
}

Comando Esp32Udp::obtenerComando(const string cmd)
{
    Comando c;

    regex regex_comando("<([a-zA-Z]+)>([a-zA-Z0-9]*)</\\1>");
    regex regex_ip("<(ip)>([0-9\\.]+)</\\1>");

    c.direccion = "NO_CMD";
    c.argumento = "NO_ARG";

    if(regex_match(cmd, regex_comando))
    {
        c.direccion = regex_replace(cmd, regex_comando, "$1");
        c.argumento = regex_replace(cmd, regex_comando, "$2");
    }
    else if(regex_match(cmd, regex_ip))
    {
        c.direccion = regex_replace(cmd, regex_ip, "$1");
        c.argumento = regex_replace(cmd, regex_ip, "$2");
    }

    return c;
}

void Esp32Udp::broadcast(QByteArray s)
{
    socket->writeDatagram(s, QHostAddress::Broadcast, udp_port);
    if(this->debug)
        qDebug() << "<<-- " + s;
}

void Esp32Udp::sendToHost(QByteArray s)
{
    socket->writeDatagram(s, QHostAddress(host_ip), udp_port);
    if(this->debug)
        qDebug() << "<- " + s;
}

void Esp32Udp::readyRead()
{
         QByteArray buffer;
         buffer.resize(socket->pendingDatagramSize());

         socket->readDatagram(buffer.data(), buffer.size());

         Comando c = Esp32Udp::obtenerComando(buffer.constData());

         this->comandos.push_back(c);

         emit comandosDisponibles();

         if(this->debug)
             qDebug() << "-> " + buffer;
}

void Esp32Udp::enviarComando(Comando c)
{
    if(this->host_ip == "NO_IP")
    {
        if(this->debug)
            qDebug() << "No se seteo la ip del host. Se hara un broadcast.";
        this->broadcast(Esp32Udp::crearComando(c).c_str());
    }
    else
    {
        this->sendToHost(Esp32Udp::crearComando(c).c_str());
    }
}

void Esp32Udp::enviarBroadcast(Comando c)
{
    this->broadcast(Esp32Udp::crearComando(c).c_str());
}

bool Esp32Udp::recibirComando(Comando &c)
{
    if(this->comandos.empty())
    {
        return false;
    }
    else
    {
        c = this->comandos.front();
        this->comandos.erase(this->comandos.begin());

        return true;
    }
}

void Esp32Udp::borrarComandos(){
    comandos.clear();
}
