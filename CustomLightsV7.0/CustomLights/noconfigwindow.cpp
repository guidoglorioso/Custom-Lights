#include "noconfigwindow.h"
#include "ui_noconfigwindow.h"



//---------------------------------------------------------------Constructor / Destructor
NoConfigWindow::NoConfigWindow(QWidget *parent) : QDialog(parent), ui(new Ui::NoConfigWindow) {
    ui->setupUi(this);

    //No se puede minimizar
    setFixedSize(width(), height());


    //Seteo en solo lectura
    ui->Bienvenida_TE->setReadOnly(true);
    ui->Red_TE->setReadOnly(true);


    //Deshabilito pantallas
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setTabEnabled(1,0);
    ui->tabWidget->setTabEnabled(2,0);
    ui->tabWidget->setTabEnabled(3,0);


    //Inicializo


    izq = 0;
    der = 0;
    arr = 0;
    aba = 0;

    f_sentido = false;
    f_cant = false;
    f_esquina = false;

    //conecto udp
    n_com = "NOCOM";
    t_com = WIFI;
}

NoConfigWindow::~NoConfigWindow() {
    delete ui;
}

//---------------------------------------------------------------Tab Bienvenida (0)
void NoConfigWindow::on_Bienvenida_sig_clicked() {
    ui->tabWidget->setCurrentIndex(1);
    ui->tabWidget->setTabEnabled(0,0);
    ui->tabWidget->setTabEnabled(1,1);
    ui->tabWidget->setTabEnabled(2,0);
    ui->tabWidget->setTabEnabled(3,0);
}



//---------------------------------------------------------------Tab Red (1)
void NoConfigWindow::on_Red_volver_clicked() {
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->setTabEnabled(0,1);
    ui->tabWidget->setTabEnabled(1,0);
    ui->tabWidget->setTabEnabled(2,0);
    ui->tabWidget->setTabEnabled(3,0);

}

void NoConfigWindow::on_Red_sig_clicked() {
    ui->tabWidget->setCurrentIndex(2);
    ui->tabWidget->setTabEnabled(0,0);
    ui->tabWidget->setTabEnabled(1,0);
    ui->tabWidget->setTabEnabled(2,1);
    ui->tabWidget->setTabEnabled(3,0);
    if(!f_cant||!f_sentido||!f_esquina)
        ui->HW_finalizar->setDisabled(true);
    ui->Boton_comunicacion->setText("WIFI");
    ui->Com_in->setVisible(0);

}


//---------------------------------------------------------------Tab HW (2)
void NoConfigWindow::on_HW_volver_clicked() {
    ui->tabWidget->setCurrentIndex(1);
    ui->tabWidget->setTabEnabled(0,0);
    ui->tabWidget->setTabEnabled(1,1);
    ui->tabWidget->setTabEnabled(2,0);
    ui->tabWidget->setTabEnabled(3,0);
}


//Esquina
void NoConfigWindow::on_RB_1_clicked() {
    if( ui->RB_1->isChecked() ) {
        ui->RB_2->setChecked(false);
        ui->RB_3->setChecked(false);
        ui->RB_4->setChecked(false);
        f_esquina = true;
        esquina = 0;
        if(f_cant&&f_sentido&&f_esquina)
            ui->HW_finalizar->setDisabled(false);
        else
            ui->HW_finalizar->setDisabled(true);
    } else
        ui->RB_1->setChecked(true);
}

void NoConfigWindow::on_RB_2_clicked() {
    if( ui->RB_2->isChecked() ) {
        ui->RB_1->setChecked(false);
        ui->RB_3->setChecked(false);
        ui->RB_4->setChecked(false);
        f_esquina = true;
        esquina = 1;

        if(f_cant&&f_sentido&&f_esquina)
            ui->HW_finalizar->setDisabled(false);
        else
            ui->HW_finalizar->setDisabled(true);
    } else
        ui->RB_2->setChecked(true);
}

void NoConfigWindow::on_RB_3_clicked() {
    if( ui->RB_3->isChecked() ) {
        ui->RB_1->setChecked(false);
        ui->RB_2->setChecked(false);
        ui->RB_4->setChecked(false);
        f_esquina = true;
        esquina = 2;

        if(f_cant&&f_sentido&&f_esquina)
            ui->HW_finalizar->setDisabled(false);
        else
            ui->HW_finalizar->setDisabled(true);
    } else
        ui->RB_3->setChecked(true);
}

void NoConfigWindow::on_RB_4_clicked() {
    if( ui->RB_4->isChecked() ) {
        ui->RB_1->setChecked(false);
        ui->RB_2->setChecked(false);
        ui->RB_3->setChecked(false);
        f_esquina = true;
        esquina = 3;

        if(f_cant&&f_sentido&&f_esquina)
            ui->HW_finalizar->setDisabled(false);
        else
            ui->HW_finalizar->setDisabled(true);
    } else
        ui->RB_4->setChecked(true);
}

//Sentido
void NoConfigWindow::on_RB_Horario_clicked() {
    if( ui->RB_Horario->isChecked() ) {
        ui->RB_Antihorario->setChecked(false);
        f_sentido = true;

        sentido = HORARIO;

        if(f_cant&&f_sentido&&f_esquina)
            ui->HW_finalizar->setDisabled(false);
        else
            ui->HW_finalizar->setDisabled(true);
    } else
        ui->RB_Horario->setChecked(true);
}

void NoConfigWindow::on_RB_Antihorario_clicked() {
    if( ui->RB_Antihorario->isChecked() ) {
        ui->RB_Horario->setChecked(false);
        f_sentido = true;

        sentido = ANTIHORARIO;

        if(f_cant&&f_sentido&&f_esquina)
            ui->HW_finalizar->setDisabled(false);
        else
            ui->HW_finalizar->setDisabled(true);
    } else
        ui->RB_Antihorario->setChecked(true);
}

//Cantidad
void NoConfigWindow::on_Izquierda_TE_textChanged() {
    f_cant = checkCantModulos();

    if(f_cant&&f_sentido&&f_esquina)
        ui->HW_finalizar->setDisabled(false);
    else
        ui->HW_finalizar->setDisabled(true);
}

void NoConfigWindow::on_Arriba_TE_textChanged() {
    f_cant = checkCantModulos();

    if(f_cant&&f_sentido&&f_esquina)
        ui->HW_finalizar->setDisabled(false);
    else
        ui->HW_finalizar->setDisabled(true);
}

void NoConfigWindow::on_Derecha_TE_textChanged() {
    f_cant = checkCantModulos();

    if(f_cant&&f_sentido&&f_esquina)
        ui->HW_finalizar->setDisabled(false);
    else
        ui->HW_finalizar->setDisabled(true);
}

void NoConfigWindow::on_Abajo_TE_textChanged() {
    f_cant = checkCantModulos();

    if(f_cant&&f_sentido&&f_esquina)
        ui->HW_finalizar->setDisabled(false);
    else
        ui->HW_finalizar->setDisabled(true);
}

//Metodos auxiliares
bool NoConfigWindow::checkCantModulos() {
    QString str_izq = ui->Izquierda_TE->toPlainText();
    QString str_der = ui->Derecha_TE->toPlainText();
    QString str_arr = ui->Arriba_TE->toPlainText();
    QString str_aba = ui->Abajo_TE->toPlainText();

    if( !checkNum(str_izq) || !checkNum(str_der) || !checkNum(str_arr) || !checkNum(str_aba) )
        return false;

    izq = str_izq.toInt();
    der = str_der.toInt();
    arr = str_arr.toInt();;
    aba = str_aba.toInt();;

    if( (izq==0)&&(der==0)&&(arr==0)&&(aba==0) )
        return false;

    return true;
}

bool NoConfigWindow::checkNum(QString) {
    QString str_izq = ui->Izquierda_TE->toPlainText();
    QString str_der = ui->Derecha_TE->toPlainText();
    QString str_arr = ui->Arriba_TE->toPlainText();
    QString str_aba = ui->Abajo_TE->toPlainText();
    int size,i;

    if( str_izq.isEmpty()||str_der.isEmpty()||str_arr.isEmpty()||str_aba.isEmpty() )
        return false;

    size = str_izq.size();
    for(i=0; i<size;i++) {
        if( !(str_izq.at(i).isNumber()) )
            return false;
    }

    size = str_der.size();
    for(i=0; i<size;i++) {
        if( !(str_der.at(i).isNumber()) )
            return false;
    }

    size = str_arr.size();
    for(i=0; i<size;i++) {
        if( !(str_arr.at(i).isNumber()) )
            return false;
    }

    size = str_aba.size();
    for(i=0; i<size;i++) {
        if( !(str_aba.at(i).isNumber()) )
            return false;
    }
    return true;
}

void NoConfigWindow::on_HW_finalizar_clicked()
{
    DatosUsuario datos;
    HwConfig hw;

    //Hw Config
    hw.izq= izq;
    hw.der= der;
    hw.arr= arr;
    hw.aba= aba;
    hw.sentido = sentido;
    hw.esquina=esquina;
    datos.setHwConfig(hw);

    //Hw info
    HwInfo mod_info;
    mod_info.cant_leds = MODULO_CANT_LEDS;
    mod_info.largo = MODULO_LARGO;
    mod_info.max_brillo = 255;
    datos.setHwInfo(mod_info);

    ComInfo com_info;
    com_info.nombre_com = n_com;
    com_info.tipo_com = t_com;
    datos.SetComInfo(com_info);

    datos.escribirConfig();

    this->close();
}




void NoConfigWindow::on_Boton_comunicacion_clicked()
{
    if(ui->Boton_comunicacion->text()=="Serie"){
        ui->Boton_comunicacion->setText("WIFI");
        ui->Com_in->setVisible(0);
        t_com = WIFI;
        n_com = "NOCOM";
    }
    else{
        if(ui->Boton_comunicacion->text()=="WIFI"){
            int buff = ui->Com_in->count();
            for(int i2 =0;i2<=buff;i2++)
                ui->Com_in->removeItem(0);
            QList<QSerialPortInfo> puertos = QSerialPortInfo::availablePorts();
            if(puertos.length()!=0){
                for(int i=0;i<puertos.length();i++){
                   ui->Com_in->addItem(puertos.at(i).portName(),puertos.at(i).portName());
                }
            }
            ui->Boton_comunicacion->setText("Serie");
            ui->Com_in->setVisible(1);
            t_com = SERIE;
            n_com = ui->Com_in->currentText().toLatin1().data();
        }
    }

}



void NoConfigWindow::on_Com_in_currentIndexChanged(const QString &arg1)
{
    t_com = SERIE;
    n_com = arg1.toLatin1().data();
}

