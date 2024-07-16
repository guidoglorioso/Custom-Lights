#include "test.h"
#include "ui_test.h"

Test::Test(modosConfig *mc_test_ptr,QWidget *parent):QDialog(parent),ui(new Ui::Test) {
    ui->setupUi(this);
    prueba_izq=0;
    prueba_arr=0;
    prueba_aba=0;
    prueba_der=0;
    //No se puede minimizar
    setFixedSize(width(), height());

    ui->Pantalla_izq->setStyleSheet("");
    ui->Pantalla_der->setStyleSheet("");
    ui->Pantalla_arr->setStyleSheet("");
    ui->Pantalla_aba->setStyleSheet("");
    mc_test = mc_test_ptr;
}

Test::~Test()
{
    delete ui;
}


void Test::on_Izquerda_Prender_clicked(){
    //Enviar Prender
    prueba_izq=1;
    ui->Pantalla_izq->setStyleSheet("background-color:rgb(255,0,0);");
    mc_test->enviar_prueba(prueba_izq,prueba_aba,prueba_der,prueba_arr);
}

void Test::on_Izquierda_Apagar_clicked() {
    //Enviar Apagar
    prueba_izq=0;
    ui->Pantalla_izq->setStyleSheet("");
    mc_test->enviar_prueba(prueba_izq,prueba_aba,prueba_der,prueba_arr);
}

void Test::on_Arriba_Prender_clicked() {
    //Enviar Prender
    prueba_arr=1;
    ui->Pantalla_arr->setStyleSheet("background-color:rgb(255,0,0);");
    mc_test->enviar_prueba(prueba_izq,prueba_aba,prueba_der,prueba_arr);
}

void Test::on_Arriba_Apagar_clicked() {
    //Enviar Apagar
    prueba_arr=0;
    ui->Pantalla_arr->setStyleSheet("");
    mc_test->enviar_prueba(prueba_izq,prueba_aba,prueba_der,prueba_arr);
}

void Test::on_Derecha_Prender_clicked() {
    //Enviar Prender
    prueba_der=1;
    ui->Pantalla_der->setStyleSheet("background-color:rgb(255,0,0);");
    mc_test->enviar_prueba(prueba_izq,prueba_aba,prueba_der,prueba_arr);
}

void Test::on_Derecha_Apagar_clicked() {
    //Enviar Apagar
    prueba_der=0;
    ui->Pantalla_der->setStyleSheet("");
    mc_test->enviar_prueba(prueba_izq,prueba_aba,prueba_der,prueba_arr);
}

void Test::on_Abajo_Prender_clicked() {
    //Enviar Prender
    prueba_aba=1;
    ui->Pantalla_aba->setStyleSheet("background-color:rgb(255,0,0);");
    mc_test->enviar_prueba(prueba_izq,prueba_aba,prueba_der,prueba_arr);
}

void Test::on_Abajo_Apagar_2_clicked() {
    //Enviar Apagar
    prueba_aba=0;
    ui->Pantalla_aba->setStyleSheet("");
    mc_test->enviar_prueba(prueba_izq,prueba_aba,prueba_der,prueba_arr);
}

void Test::on_Test_Cerrar_clicked() {
    close();
}
