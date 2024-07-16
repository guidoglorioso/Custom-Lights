#include "ayudared.h"
#include "ui_ayudared.h"

AyudaRed::AyudaRed(QWidget *parent):QDialog(parent),ui(new Ui::AyudaRed) {
    ui->setupUi(this);

    //No se puede minimizar
    setFixedSize(width(), height());

    //Solo lectura
    ui->textEdit->setReadOnly(true);
}

AyudaRed::~AyudaRed() {
    delete ui;
}

void AyudaRed::on_pushButton_clicked() {
    this->close();
}
