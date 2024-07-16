#include "imprimir.h"

Imprimir::Imprimir() {
    RedEstado = nullptr;
    RedInfo = nullptr;
    HwEstado = nullptr;
    HwInfo = nullptr;
    Modo=nullptr;
    Dir = nullptr;
    BurbujaColor = nullptr;
}


void Imprimir::printRedEstado(bool f_ConfigRed) {
    QTextCharFormat format;
    QTextCursor cursor;
    if(RedEstado==nullptr)
        return;
    RedEstado->clear();
    RedEstado->setFontWeight(QFont::Normal);
    RedEstado->setTextColor( QColor::fromRgb(0,0,0) );
    RedEstado->setText("Estado: ");
    cursor = RedEstado->textCursor();
    cursor.movePosition(QTextCursor::End);
    RedEstado->setTextCursor(cursor);
    RedEstado->setFontWeight(QFont::Bold);
    if( f_ConfigRed ) {
        RedEstado->setTextColor( QColor::fromRgb(78,154,6) );
        RedEstado->insertPlainText("Conectado!");
    } else {
        RedEstado->setTextColor( QColor::fromRgb(144,0,0) );
        RedEstado->insertPlainText("No Conectado!");
    }
    return;
}

void Imprimir::printRedInfo(QString red ,QString ip) {

    QTextCharFormat format;
    QTextCursor cursor;

    if(RedInfo==nullptr)
        return;

    //Red
    RedInfo->setFontWeight(QFont::Bold);
    RedInfo->setTextColor( QColor::fromRgb(0,0,0) );
    RedInfo->setText("Red: ");
    cursor = RedInfo->textCursor();
    cursor.movePosition(QTextCursor::End);
    RedInfo->setTextCursor(cursor);
    RedInfo->setFontWeight(QFont::Normal);
    RedInfo->insertPlainText(red);

    //Ip
    RedInfo->setFontWeight(QFont::Bold);
    RedInfo->append("\nIP del CustomLightController: ");
    cursor = RedInfo->textCursor();
    cursor.movePosition(QTextCursor::End);
    RedInfo->setTextCursor(cursor);
    RedInfo->setFontWeight(QFont::Normal);
    RedInfo->insertPlainText(ip);
}

void Imprimir::printHwEstado(bool f_ConfigHw) {
    QTextCharFormat format;
    QTextCursor cursor;

    if(HwEstado==nullptr)
        return;

    HwEstado->clear();
    HwEstado->setFontWeight(QFont::Normal);
    HwEstado->setTextColor( QColor::fromRgb(0,0,0) );
    HwEstado->setText("Estado: ");
    cursor = HwEstado->textCursor();
    cursor.movePosition(QTextCursor::End);
    HwEstado->setTextCursor(cursor);
    HwEstado->setFontWeight(QFont::Bold);
    if(f_ConfigHw) {
        HwEstado->setTextColor( QColor::fromRgb(78,154,6) );
        HwEstado->insertPlainText("Configurado!");
    } else {
        HwEstado->setTextColor( QColor::fromRgb(144,0,0) );
        HwEstado->insertPlainText("No Configurado!");
    }
}

bool Imprimir::printHwInfo(HwConfig datos) {
    if(HwInfo==nullptr)
        return false;

    HwInfo->clear();
    bool f_cant = printCant(datos.izq,datos.arr,datos.der,datos.aba);
    bool f_esquina = printEsquina(datos.esquina);
    bool f_sentido = printSentido(datos.sentido);
    return (f_cant&&f_esquina&&f_sentido);
}

void Imprimir::printBurbujaColor( Custom datos ) {
    if(BurbujaColor==nullptr)
        return;

    QString str = "border-radius: 10;";
    str += "background-color: rgb( ";
    str += QString::number( datos.R );
    str += ", ";
    str += QString::number( datos.G );
    str += ", ";
    str += QString::number( datos.B );
    str += " );";
    BurbujaColor->setStyleSheet(str);
}

bool Imprimir::printCant(int izq, int arr, int der, int aba ) {
    QTextCharFormat format;
    QTextCursor cursor;
    bool f_cantMal = ( (izq<0||der<0||arr<0||aba<0) ||  ((izq==0)&&(der==0)&&(arr==0)&&(aba==0)) );
    HwInfo->setFontWeight(QFont::Bold);
    HwInfo->setTextColor( QColor::fromRgb(0,0,0) );
    HwInfo->append("Cantidad de Módulos: ");
    if(f_cantMal) {
        cursor = HwInfo->textCursor();
        cursor.movePosition(QTextCursor::End);
        HwInfo->setTextCursor(cursor);
        HwInfo->setFontWeight(QFont::Normal);
        HwInfo->setTextColor( QColor::fromRgb(144,0,0) );
        HwInfo->insertPlainText("Error!");
    } else {
        HwInfo->setFontWeight(QFont::Normal);
        HwInfo->append("->Izquierda: ");
        cursor = HwInfo->textCursor();
        cursor.movePosition(QTextCursor::End);
        HwInfo->setTextCursor(cursor);
        HwInfo->insertPlainText( QString::number(izq, 10) );

        HwInfo->append("->Arriba: ");
        cursor = HwInfo->textCursor();
        cursor.movePosition(QTextCursor::End);
        HwInfo->setTextCursor(cursor);
        HwInfo->insertPlainText( QString::number(arr, 10) );

        HwInfo->append("->Derecha: ");
        cursor = HwInfo->textCursor();
        cursor.movePosition(QTextCursor::End);
        HwInfo->setTextCursor(cursor);
        HwInfo->insertPlainText( QString::number(der, 10) );

        HwInfo->append("->Abajo: ");
        cursor = HwInfo->textCursor();
        cursor.movePosition(QTextCursor::End);
        HwInfo->setTextCursor(cursor);
        HwInfo->insertPlainText( QString::number(aba, 10) );
    }
    HwInfo->setFontWeight(QFont::Normal);
    HwInfo->setTextColor( QColor::fromRgb(0,0,0) );
    HwInfo->append(" ");
    return !f_cantMal;
}

bool Imprimir::printEsquina(int nro) {
    //Actualizo Info esquinas
    // 0              1
    //    Pantalla
    // 2              3

    QTextCharFormat format;
    QTextCursor cursor;
    QString espacio = "               ";

    bool f_esquinaMal = (nro<0||nro>3);

    HwInfo->setFontWeight(QFont::Bold);
    HwInfo->setTextColor( QColor::fromRgb(0,0,0) );
    HwInfo->append("Ubicación del primer módulo: ");
    HwInfo->setFontWeight(QFont::Normal);

    if(f_esquinaMal) {
        cursor = HwInfo->textCursor();
        cursor.movePosition(QTextCursor::End);
        HwInfo->setTextCursor(cursor);
        HwInfo->setTextColor( QColor::fromRgb(144,0,0) );
        HwInfo->insertPlainText("Error!");
        cursor = HwInfo->textCursor();
        cursor.movePosition(QTextCursor::End);
        HwInfo->setTextCursor(cursor);
        HwInfo->setTextColor( QColor::fromRgb(0,0,0) );
        return !f_esquinaMal;
    } else {
        //Esquina 0
        HwInfo->append( espacio+espacio );
        cursor = HwInfo->textCursor();
        cursor.movePosition(QTextCursor::End);
        HwInfo->setTextCursor(cursor);
        if(nro == 0)
            HwInfo->setTextColor( QColor::fromRgb(244,0,0) );
        else
            HwInfo->setTextColor( QColor::fromRgb(0,0,0) );
        HwInfo->insertPlainText("O");

        //Esquina 1
        cursor = HwInfo->textCursor();
        cursor.movePosition(QTextCursor::End);
        HwInfo->setTextCursor(cursor);
        HwInfo->insertPlainText( espacio+espacio );
        if(nro == 1)
            HwInfo->setTextColor( QColor::fromRgb(244,0,0) );
        else
            HwInfo->setTextColor( QColor::fromRgb(0,0,0) );
        HwInfo->insertPlainText("O");

        //Pantalla
        HwInfo->setTextColor( QColor::fromRgb(0,0,0) );
        HwInfo->append( espacio+espacio+"           "+"Pantalla");

        //Esquina 2
        HwInfo->append( espacio+espacio );
        cursor = HwInfo->textCursor();
        cursor.movePosition(QTextCursor::End);
        HwInfo->setTextCursor(cursor);
        if(nro == 2)
            HwInfo->setTextColor( QColor::fromRgb(244,0,0) );
        else
            HwInfo->setTextColor( QColor::fromRgb(0,0,0) );
        HwInfo->insertPlainText("O");

        //Esquina 3
        cursor = HwInfo->textCursor();
        cursor.movePosition(QTextCursor::End);
        HwInfo->setTextCursor(cursor);
        HwInfo->insertPlainText( espacio+espacio );
        if(nro == 3)
            HwInfo->setTextColor( QColor::fromRgb(244,0,0) );
        else
            HwInfo->setTextColor( QColor::fromRgb(0,0,0) );
        HwInfo->insertPlainText("O");
    }

    HwInfo->append(" ");
    return !f_esquinaMal;
}

bool Imprimir::printSentido(int sentido) {
    QTextCharFormat format;
    QTextCursor cursor;
    bool f_sentidoMal = (sentido<0 || sentido>1);
    HwInfo->setFontWeight(QFont::Bold);
    HwInfo->setTextColor( QColor::fromRgb(0,0,0) );
    HwInfo->append("Sentido de recorrido: ");
    cursor = HwInfo->textCursor();
    cursor.movePosition(QTextCursor::End);
    HwInfo->setTextCursor(cursor);
    HwInfo->setFontWeight(QFont::Normal);
    if(f_sentidoMal) {
        HwInfo->setTextColor( QColor::fromRgb(144,0,0) );
        HwInfo->insertPlainText("Error!");
    } else {
        if(sentido == HORARIO)
            HwInfo->insertPlainText("Horario");
        if(sentido == ANTIHORARIO){
            HwInfo->insertPlainText("Antihorario");
        }

    }
    HwInfo->setFontWeight(QFont::Normal);
    HwInfo->setTextColor( QColor::fromRgb(0,0,0) );
    HwInfo->append(" ");
    return !f_sentidoMal;
}

void Imprimir::printModo(QString nombre,QString descripcion,QColor color) {
    if(Modo==nullptr)
        return;
    Modo->setFontWeight(QFont::Bold);
    Modo->setTextColor( color );
    Modo->setText(nombre);
    Modo->setFontWeight(QFont::Normal);
    Modo->setTextColor( QColor::fromRgb(0,0,0) );
    Modo->append(descripcion);
}

void Imprimir::printModo(bool f_ConfigHW, bool f_ConfigRed) {
    if(Modo==nullptr)
        return;
    Modo->setTextColor(QColor(0,0,0));
    Modo->setFontWeight(QFont::Normal);
    Modo->clear();
    if(f_ConfigHW)
        Modo->append("Configurar la seccion de HW para que se habiliten las funcionalidades");
    if(f_ConfigRed)
        Modo->append("Buscando al CustomLightController...");


}

QString Imprimir::printDir(QString dir, bool i) {
    if(Dir==nullptr)
        return "Puntero no seteado";
    if(!i) {
        Dir->setStyleSheet("color: rgb(244,0,0);font: bold;");
        Dir->setText("No configurado!");
        return "-";
    }
    Dir->setStyleSheet("color: rgb(0,0,0);");
    Dir->setText(dir);
    return dir;

}
