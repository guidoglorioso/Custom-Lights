#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define TIME 1000
#define ACTUALIZAR 5
#define TIME_PANTALLA 1000/70

//Ventanas
#include "noconfigwindow.h"
#include "test.h"
#include "ayudared.h"
#include "confighw.h"
#include <QMessageBox>
#include <QMessageBox>
#include <QFileDialog>

//Datos
#include "datosusuario.h"
#include "imprimir.h"


//General
#include <QMainWindow>
#include <QString>
#include <QContextMenuEvent>
#include <QColor>
#include <QIODevice>
#include <QScrollBar>
#include <QTextCursor>
#include <QTextCharFormat>


//Puerto serie
#include "modosconfig.h"
#include <QtSerialPort>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_Red_Ayuda_clicked();
    void on_Dir_Cambiar_clicked();
    void on_Test_PB_clicked();
    void on_HW_Config_clicked();
    void on_Modo_CB_currentTextChanged(const QString &arg1);
    void on_Conf_PB_Abrir_clicked();
    void on_Conf_PB_Cerrar_clicked();
    void on_Borde_Ancho_valueChanged(int value);
    void on_Color_B_SLI_valueChanged(int value);
    void on_Color_G_SLI_valueChanged(int value);
    void on_Color_R_SLI_valueChanged(int value);
    void on_Brillo_SLI_valueChanged(int value);
    void on_Brillo_CHB_stateChanged(int arg1);
    void on_SONIDO_CB_currentIndexChanged(int index);

    void interruptTimer(); //Timer 1 seg
    void TimerData1ms(); // Timer 1 mseg

private:
    Ui::MainWindow *ui;

    bool f_ConfigRed;                       //Flag para si esta conectado a la red
    bool f_ConfigHW;                        //Flag para saber si esta bien cargada la info
    bool f_aux;                             //Flag para saber si debo actualizar el TE del modo
    int cantBroadcast;                      //Cantidad de veces que envio un broadcast sin que respondan

    DatosUsuario datos;                     //Clase para almacenar datos e interactuar con archivo
    Imprimir print;                         //Clase para hacer impresiones

    //Inicializacion
    void setTextEditReadOnlyAll();          //Setea todos los Text Edit en read only
    void agregarModos();                    //Agrega las opciones al combo box de elegir modo

    //Levantar datos
    void cargarConfig();                    //Hace las validaciones del archivo de configuracion
    void setPersonalizacion();              //Levanta la personalizacion
    bool existeDir( QString );              //Verifica si existe un directorio. Retorna 1 si existe
    void mensajeError();                    //Tira error con QMessage

    //Hab de funcionalidades                //Ponen en gris o habilitan las diferentes secciones
    void habGeneral(bool);
    void setHab();
    void habBorde(bool);
    void habColor(bool);
    void habBrillo(bool);

    modosConfig mc;

    // sonido
    QScopedPointer<AudioInfo> m_audioInfo;
    QScopedPointer<QAudioInput> m_audioInput;
    bool  inicializacion_audio_ok;
    void TimerOff();
    void initializeAudio(const QAudioDeviceInfo &deviceInfo);
    void inicializacion_audio();

    // pantalla
    Pantalla *p;


};
#endif // MAINWINDOW_H
