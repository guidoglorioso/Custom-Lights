#ifndef CONFIGHW_H
#define CONFIGHW_H

#include <QDialog>
#include "datosusuario.h"
#include <QSerialPortInfo>


namespace Ui {
class ConfigHw;
}

class ConfigHw : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigHw(QWidget *parent = nullptr);
    ~ConfigHw();

private slots:
    void on_Izquierda_TE_textChanged();
    void on_Arriba_TE_textChanged();
    void on_Derecha_TE_textChanged();
    void on_Abajo_TE_textChanged();
    void on_RB_1_clicked();
    void on_RB_2_clicked();
    void on_RB_3_clicked();
    void on_RB_4_clicked();
    void on_RB_Horario_clicked();
    void on_RB_Antihorario_clicked();
    void on_Descartar_clicked();
    void on_Guardar_clicked();
    void on_Boton_comunicacion_clicked();
    void on_Com_in_currentIndexChanged(const QString &arg1);

private:
    Ui::ConfigHw *ui;
    void cargarInit();
    void printEstadoRed(bool);
    bool checkCantModulos();
    bool checkNum(QString);

    DatosUsuario datos;

    int izq;
    int der;
    int arr;
    int aba;
    int esquina;
    int sentido;

    bool f_cant;
    bool f_esquina;
    bool f_sentido;

    bool t_com;
    string n_com;

};

#endif // CONFIGHW_H
