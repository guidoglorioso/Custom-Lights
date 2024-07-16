#ifndef TEST_H
#define TEST_H

#include <QDialog>
#include <QTimer>
#include "modosconfig.h"

namespace Ui {
class Test;
}

class Test : public QDialog
{
    Q_OBJECT

public:
    explicit Test(modosConfig *mc_test_ptr,QWidget *parent = nullptr);
    ~Test();

private slots:
    void on_Izquerda_Prender_clicked();
    void on_Izquierda_Apagar_clicked();
    void on_Arriba_Prender_clicked();
    void on_Arriba_Apagar_clicked();
    void on_Derecha_Prender_clicked();
    void on_Derecha_Apagar_clicked();
    void on_Abajo_Prender_clicked();
    void on_Abajo_Apagar_2_clicked();
    void on_Test_Cerrar_clicked();


private:
    Ui::Test *ui;
    modosConfig *mc_test;
    bool prueba_izq,prueba_arr,prueba_aba,prueba_der;
};

#endif // TEST_H
