#include "confighw.h"
#include "ui_confighw.h"

ConfigHw::ConfigHw(QWidget *parent):QDialog(parent),ui(new Ui::ConfigHw) {
    ui->setupUi(this);

    //No se puede minimizar
    setFixedSize(width(), height());

    //Leo config previa y la preseteo
    datos.leerConfig();

    f_cant = false;
    f_sentido= false;
    f_esquina = false;

    cargarInit();
}

void ConfigHw::cargarInit() {
    //Cargo cantidad
    aba = datos.getHwConfig().aba;
    arr = datos.getHwConfig().arr;
    izq = datos.getHwConfig().izq;
    der = datos.getHwConfig().der;
    if((aba>=0)&&(izq>=0)&&(der>=0)&&(arr>=0)){
        ui->Abajo_TE->setText(QString::number(aba));
        ui->Arriba_TE->setText(QString::number(arr));
        ui->Derecha_TE->setText(QString::number(der));
        ui->Izquierda_TE->setText(QString::number(izq));
    }
    f_cant = checkCantModulos();

    //Cargo sentido
    int sentido_ = datos.getHwConfig().sentido;
    if( sentido_ == HORARIO) {
        f_sentido = true;
        ui->RB_Horario->setChecked(true);
        sentido = HORARIO;
    }
    if( sentido_ == ANTIHORARIO) {
        f_sentido = true;
        ui->RB_Antihorario->setChecked(true);
        sentido = ANTIHORARIO;
    }

    //Cargo esquina
    int esquina_ = datos.getHwConfig().esquina;
    f_esquina = (esquina_<3&&esquina_>0);
    if( esquina_ == IZQ_ARR ) {
        ui->RB_1->setChecked(true);
        esquina = IZQ_ARR;
    }
    if(esquina_ == DER_ARR) {
        ui->RB_2->setChecked(true);
        esquina = DER_ARR;
    }
    if(esquina_ == IZQ_ABA) {
        ui->RB_3->setChecked(true);
        esquina = IZQ_ABA;
    }
    if(esquina_ == DER_ABA) {
        ui->RB_4->setChecked(true);
        esquina = DER_ABA;
    }

    // cargo config comunicacion busca aca

    ComInfo a = datos.getComInfo();
    t_com = a.tipo_com;
    n_com = a.nombre_com;

    if( t_com == WIFI){
        ui->Boton_comunicacion->setText("WIFI");
        ui->Com_in->setVisible(0);
    }

    if(t_com == SERIE){
        ui->Boton_comunicacion->setText("Serie");
        ui->Com_in->setVisible(1);
        QList<QSerialPortInfo> puertos = QSerialPortInfo::availablePorts();
        if(puertos.length()!=0){
            for(int i=0;i<puertos.length();i++){
               ui->Com_in->addItem(puertos.at(i).portName(),puertos.at(i).portName());
            }
        }
        QString buff = datos.getComInfo().nombre_com.c_str();
        buff.remove(buff.length()-1,1);
        ui->Com_in->setCurrentText(buff);
    }

    //Valido
    if(f_cant&&f_esquina&&f_sentido)
        ui->Guardar->setDisabled(false);
    else
        ui->Guardar->setDisabled(true);
}

ConfigHw::~ConfigHw()
{
    delete ui;
}

//Esquina
void ConfigHw::on_RB_1_clicked() {
    f_esquina = true;
    if( ui->RB_1->isChecked() ) {
        ui->RB_2->setChecked(false);
        ui->RB_3->setChecked(false);
        ui->RB_4->setChecked(false);
        esquina = 0;
        if(f_cant)
            ui->Guardar->setDisabled(false);
        else
            ui->Guardar->setDisabled(true);
    } else
        ui->RB_1->setChecked(true);
}

void ConfigHw::on_RB_2_clicked() {
    f_esquina = true;
    if( ui->RB_2->isChecked() ) {
        ui->RB_1->setChecked(false);
        ui->RB_3->setChecked(false);
        ui->RB_4->setChecked(false);
        esquina = 1;
        if(f_cant&&f_esquina&&f_sentido)
            ui->Guardar->setDisabled(false);
        else
            ui->Guardar->setDisabled(true);
    } else
        ui->RB_2->setChecked(true);
}

void ConfigHw::on_RB_3_clicked() {
    f_esquina = true;
    if( ui->RB_3->isChecked() ) {
        ui->RB_1->setChecked(false);
        ui->RB_2->setChecked(false);
        ui->RB_4->setChecked(false);
        esquina = 2;
        if(f_cant&&f_esquina&&f_sentido)
            ui->Guardar->setDisabled(false);
        else
            ui->Guardar->setDisabled(true);
    } else
        ui->RB_3->setChecked(true);
}

void ConfigHw::on_RB_4_clicked() {
    f_esquina = true;
    if( ui->RB_4->isChecked() ) {
        ui->RB_1->setChecked(false);
        ui->RB_2->setChecked(false);
        ui->RB_3->setChecked(false);
        esquina = 3;
        if(f_cant&&f_esquina&&f_sentido)
            ui->Guardar->setDisabled(false);
        else
            ui->Guardar->setDisabled(true);
    } else
        ui->RB_4->setChecked(true);
}

//Sentido
void ConfigHw::on_RB_Horario_clicked() {
    f_sentido = true;
    if( ui->RB_Horario->isChecked() ) {
        ui->RB_Antihorario->setChecked(false);
        sentido = HORARIO;
        if(f_cant&&f_esquina&&f_sentido)
            ui->Guardar->setDisabled(false);
        else
            ui->Guardar->setDisabled(true);
    } else
        ui->RB_Horario->setChecked(true);
}

void ConfigHw::on_RB_Antihorario_clicked() {
    f_sentido = true;
    if( ui->RB_Antihorario->isChecked() ) {
        ui->RB_Horario->setChecked(false);
        sentido = ANTIHORARIO;
        if(f_cant&&f_esquina&&f_sentido)
            ui->Guardar->setDisabled(false);
        else
            ui->Guardar->setDisabled(true);
    } else
        ui->RB_Antihorario->setChecked(true);
}

//Cantidad
void ConfigHw::on_Izquierda_TE_textChanged() {
    f_cant = checkCantModulos();
    if(f_cant&&f_esquina&&f_sentido)
        ui->Guardar->setDisabled(false);
    else
        ui->Guardar->setDisabled(true);
}

void ConfigHw::on_Arriba_TE_textChanged() {
    f_cant = checkCantModulos();
    if(f_cant&&f_esquina&&f_sentido)
        ui->Guardar->setDisabled(false);
    else
        ui->Guardar->setDisabled(true);
}

void ConfigHw::on_Derecha_TE_textChanged() {
    f_cant = checkCantModulos();
    if(f_cant)
        ui->Guardar->setDisabled(false);
    else
        ui->Guardar->setDisabled(true);
}

void ConfigHw::on_Abajo_TE_textChanged() {
    f_cant = checkCantModulos();
    if(f_cant)
        ui->Guardar->setDisabled(false);
    else
        ui->Guardar->setDisabled(true);
}

//Metodos auxiliares
bool ConfigHw::checkCantModulos() {
    QString str_izq = ui->Izquierda_TE->toPlainText();
    QString str_der = ui->Derecha_TE->toPlainText();
    QString str_arr = ui->Arriba_TE->toPlainText();
    QString str_aba = ui->Abajo_TE->toPlainText();

    if( !checkNum(str_izq) || !checkNum(str_der) || !checkNum(str_arr) || !checkNum(str_aba) )
        return false;

    izq = str_izq.toInt();
    der = str_der.toInt();
    arr = str_arr.toInt();
    aba = str_aba.toInt();

    if( (izq==0)&&(der==0)&&(arr==0)&&(aba==0) )
        return false;

    return true;
}

bool ConfigHw::checkNum(QString) {
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

void ConfigHw::on_Descartar_clicked() {
    close();
}

void ConfigHw::on_Guardar_clicked() {
    HwConfig hw;
    ComInfo ci;
    hw.izq= izq;
    hw.der= der;
    hw.arr= arr;
    hw.aba= aba;
    hw.sentido = sentido;
    hw.esquina=esquina;

    ci.nombre_com = n_com;
    ci.tipo_com = t_com;

    datos.setHwConfig(hw);
    datos.SetComInfo(ci);

    datos.escribirConfig();
    this->close();
}

void ConfigHw::on_Boton_comunicacion_clicked()
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

void ConfigHw::on_Com_in_currentIndexChanged(const QString &arg1)
{
    t_com = SERIE;
    n_com = arg1.toLatin1().data();
}

