#ifndef DATOSUSUARIO_H
#define DATOSUSUARIO_H

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <iostream>
#include <QString>

#define HORARIO 1
#define ANTIHORARIO 0
#define IZQ_ARR 0
#define DER_ARR 1
#define IZQ_ABA 2
#define DER_ABA 3
#define NOMBRE "CustomLightsConfig.cl"
#define MODULO_LARGO 25
#define MODULO_CANT_LEDS 15
#define WIFI 0
#define SERIE 1

using namespace std;

typedef struct{
    char izq;
    char arr;
    char der;
    char aba;
    int esquina;
    int sentido;
}HwConfig;

typedef struct{
    int cant_leds;
    int largo;
    unsigned char max_brillo;
}HwInfo;

typedef struct{
    int modo;           // indica el ultimo modo en el que se encontraba el sistema

    int ancho_borde;    // indica cuanto ancho del borde tomamos para ciertas funciones
    int alto_borde;     // indica cuanto alto del borde tomamos para ciertas funciones

    unsigned char R;    // indica ultimo valor que tenia el color rojo (0-255)
    unsigned char G;    // indica ultimo valor que tenia el color verde (0-255)
    unsigned char B;    // indica ultimo valor que tenia el color azul (0-255)

    bool flag_brillo;   // indica si el brillo esta automatico (1) o manual (0)
    unsigned char brillo;   // indica valor de brillo seteado (0-255)

}Custom;


typedef struct{
    bool tipo_com;
    string nombre_com;
}ComInfo ;


class DatosUsuario {

public:
    DatosUsuario();
    ~DatosUsuario();

    bool existeConfig(); // si esta creado el archivo de configuracion devuelve 1

    bool escribirConfig();//Escribir (y crear si no existe)
    bool leerConfig(); //Asigna variables

    bool escribirCustom(QString);
    bool leerCustom(QString);

    //Seteadores de variables (no cargan en archivo)
    inline void setHwConfig (const HwConfig a) { m_HwConfig =a; };
    inline void setHwInfo (const HwInfo a) { m_HwInfo = a; };
    inline void setCustom(const Custom a) { m_Custom = a; };
    inline void setRuta(QString a) { m_ruta = a; };
    inline void setAudioInput(QString a){audio_default = a;};
    inline void SetComInfo(ComInfo a){comunicacion_info = a;};

    //Geteadores de variables (no leen del archivo)
    inline HwConfig getHwConfig ( ) { return m_HwConfig; };
    inline HwInfo getHwInfo ( ) { return m_HwInfo; };
    inline Custom getCustom ( ) { return m_Custom; };
    inline QString getRuta() { return m_ruta; };
    inline QString getAudioInput(){return audio_default;};
    inline ComInfo getComInfo(){return comunicacion_info;};


private:
    QString m_ruta;
    Custom m_Custom;
    HwConfig m_HwConfig;
    HwInfo m_HwInfo;
    ComInfo comunicacion_info;
    QString audio_default;
};

#endif // DATOSUSUARIO_H
