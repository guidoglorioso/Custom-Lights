#ifndef ESP32UDP_H
#define ESP32UDP_H

#include <QObject>
#include <QtNetwork/QUdpSocket>
#include <QDebug>

#include <string>
#include <regex>

struct Comando
{
    std::string direccion;
    std::string argumento;
};

class Esp32Udp : public QObject
{
    Q_OBJECT
private:
    std::vector<Comando> comandos;
    QString host_ip;
    QUdpSocket *socket;

    const int udp_port = 10800;

    void broadcast(QByteArray s);
    void sendToHost(QByteArray s);

    bool debug;

public:
    explicit Esp32Udp(QObject *parent = nullptr, bool debug = false);

    void enviarComando(Comando c);
    void enviarBroadcast(Comando c);
    bool recibirComando(Comando &c);
    void borrarComandos();
    void inline setHostIp(QString ip) { this->host_ip = ip; }

    static std::string crearComando(const Comando c);
    static std::string crearComando(const std::string direccion, const std::string argumento);
    static Comando obtenerComando(const std::string cmd);

public slots:
    void readyRead();

signals:
    void comandosDisponibles();

};

#endif // ESP32UDP_H
