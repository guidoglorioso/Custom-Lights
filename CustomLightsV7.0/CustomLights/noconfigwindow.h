#ifndef NOCONFIGWINDOW_H
#define NOCONFIGWINDOW_H

#include <QDialog>
#include "datosusuario.h"
#include "imprimir.h"
#include <QTimer>
#include <QSerialPortInfo>
#include "configuracionesp32.h"

namespace Ui {
class NoConfigWindow;
}

class NoConfigWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NoConfigWindow(QWidget *parent = nullptr);
    ~NoConfigWindow();

private slots:
    void on_Bienvenida_sig_clicked();
    void on_Red_volver_clicked();
    void on_Red_sig_clicked();
    void on_HW_volver_clicked();
    void on_RB_1_clicked();
    void on_RB_2_clicked();
    void on_RB_3_clicked();
    void on_RB_4_clicked();
    void on_RB_Horario_clicked();
    void on_RB_Antihorario_clicked();
    void on_Izquierda_TE_textChanged();
    void on_Arriba_TE_textChanged();
    void on_Derecha_TE_textChanged();
    void on_Abajo_TE_textChanged();
    void on_HW_finalizar_clicked();

    void on_Boton_comunicacion_clicked();

    void on_Com_in_currentIndexChanged(const QString &arg1);


private:
    Ui::NoConfigWindow *ui;
    bool checkCantModulos();
    bool checkNum(QString);

    int izq;
    int der;
    int arr;
    int aba;
    int esquina;
    int sentido;

    bool f_sentido;
    bool f_cant;
    bool f_esquina;
    bool t_com;
    string n_com;
};

#endif // NOCONFIGWINDOW_H
