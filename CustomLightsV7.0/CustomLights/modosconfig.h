#ifndef MODOSCONFIG_H
#define MODOSCONFIG_H

#include "datosusuario.h"


//includes sonido
#include "audioinfo.h"

//includes pantalla
#include <windows.h>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsView>
#include "pantalla.h"
#include "iluminacionled.h"
#include <QTime>
#include "configuracionesp32.h"
#include "configuracionserie.h"

// Definiciones
#define TIME_D 10 // Tiempo entre cada trama que envio (en mS)
#define LEDS_MODULO 1  // CANT DE LEDS POR MODULO

enum{FADE,FIJO,NINGUNO,PUNCHI,PANTALLA,RAINBOW,MOVI,DAY_lIGHT,APAGAR,AUDIO};

enum{INDEX_NINGUNO=0,INDEX_FADE_R,INDEX_FADE_V,INDEX_FADE_A,INDEX_FADE,INDEX_FIJO,
     INDEX_PANTALLA,INDEX_PUNCHI,INDEX_SONIDO,INDEX_MOVI,INDEX_DAY_LIGHT,INDEX_RAINBOW}; // Modos interfaz

#define ON 1
#define OFF 0

typedef struct{
    char r;
    char g;
    char b;
    char brillo;
    int cant;
}Led_data;


// ------ Miembros clase

class modosConfig
{

public:

    modosConfig(QObject *parent = nullptr);
    ~modosConfig();

    //-----------------seteadores
    void Set_modo(const unsigned char m); // setea el modo que se quiere enviar (utilizar defines de modos)
    void Set_colores(const unsigned char r,const unsigned char g,const unsigned char b); // setea colores que se estan usando
    void Set_Com(const ComInfo a); // setea tipo de comunicacion
    void Set_leds_config(const HwConfig a); //setea configuracion leds
    inline void Set_cant_leds(const int a){cant_led = a;}; // setea la cantidad de leds que se estan utilizando
    inline void Set_brillo_volumen(const bool a){brillo_volumen = a;}; // setea si se desea modificar el brillo con el volumen o no (para activar enviar true)
    inline void Set_color_verde(const unsigned char v){color_g = v;}; // setea colores de forma individual
    inline void Set_color_rojo(const unsigned char r){color_r = r;};
    inline void Set_color_azul(const unsigned char a){color_b = a;};
    inline void Set_borde(const unsigned char ancho){ ancho_pantalla= ancho;}; // setea borde de la pantalla que se quiere leer
    inline void Set_brillo(const unsigned char b){ brillo =((b*255.0)/99.0); };// setea brillo que se desea(convierto de max = 100 --> max = 255)

    //----------------funciones de actualizacion
    void actualizar_volumen(unsigned int max,unsigned int prom); // funcion que actualiza los niveles maximos y promedio de cada paquete de audip
    void actualizar_luces(); // funcion que envia las tramas y actualiza los modos ¡ejecutar cada 10 ms!
    inline void actualizarUDP(){ esp32->ME_configuracion();}; // funcion que actualiza el estado/configuracion UDP (actualizar cada 1seg aprox)
    void enviar_prueba(const int izq,const int aba,const int der,const int arr); // funcion para el modo test

    //----------------geteadores
    inline int Get_modo(){return modo;}; // devuelve el modo que esta configurado
    inline QString Get_IP(){return esp32->getIp();}; // devuelve la ip a la que se conecto el QT (ip del esp32)
    inline bool Get_Estado_wifi(){return esp32->estaConectado();}; // devuelve si esta conectado o no al wifi

private:

    //----------------variables
    unsigned char brillo;
    unsigned char modo;
    unsigned char ancho_pantalla;
    unsigned char color_r,color_g,color_b;
    int cant_led;
    HwConfig configuracion_leds;
    int valor_vumetro;
    bool brillo_volumen;

    //-----------------comunicacion serie
    configuracionserie serie;

    //-----------------comunicacion UDP
    ComInfo com_info;
    ConfiguracionEsp32 * esp32;

    //------------------Enviar tramas especiales
    void enviar_sonido();
    void enviar_fijo();
    void enviar_rainbow();
    void enviar_fade();
    void enviar_movimiento();
    void enviar_fiesta();
    void enviar_dayLight();
    void enviar_apagar();
    void enviar_pantalla();
    void enviar_trama();

    //---------------------Ordenamiento y tratamiento de tramas
    void ordenar_trama(); // ordena la trama segun la configuracion de leds
    QByteArray Dar_vuelta_trama(QByteArray a); // invierte la trama respetando el orden RGB
    void reset_trama(); // borra todo el buffer de trama

    void agregar_trama(char r, char g, char b,int cant, char brillo_t); // agrega datos al buffer de la trama que se quiere enviar
    void agregar_trama_apagar(int cant);
    void agregar_led(QColor pixel);

    //------------------Funciones y variables de pantalla

    HWND desktop;
    Pantalla *p;
    QColor getAverageRGB(QImage img, int x, int y, int w, int h);

    //----------------------Extras / Buffers
    QList<int> mi_buff; // la uso para promedios del sonido
    QByteArray trama_completa;
    int cant_cargada;
    QTime time;

};

#endif // MODOSCONFIG_H
