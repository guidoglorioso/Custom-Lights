#ifndef IMPRIMIR_H
#define IMPRIMIR_H

#include <QString>
#include <QTextEdit>
#include <QLabel>
#include <QDir>
#include "datosusuario.h"

class Imprimir
{
private:
    QTextEdit *RedEstado;
    QTextEdit *RedInfo;
    QTextEdit *HwEstado;
    QTextEdit *HwInfo;
    QTextEdit *Modo;
    QTextEdit *Dir;
    QLabel *BurbujaColor;

    bool printCant(int, int, int, int);
    bool printSentido(int);
    bool printEsquina(int);

public:
    Imprimir();
    inline void setTeRedEstado(QTextEdit *a) {RedEstado=a;}
    inline void setTeRedInfo(QTextEdit *a) {RedInfo=a;}
    inline void setTeHwEstado(QTextEdit *a) {HwEstado=a;}
    inline void setTeHwInfo(QTextEdit *a) {HwInfo=a;}
    inline void setTeModo(QTextEdit *a) {Modo=a;}
    inline void setTeDir(QTextEdit *a) {Dir=a;}
    inline void setBurbujaColor(QLabel *a) {BurbujaColor=a;}

    void printRedEstado(bool);
    void printRedInfo(QString,QString);
    void printHwEstado(bool);
    bool printHwInfo(HwConfig);
    void printModo(QString,QString,QColor);
    void printModo(bool,bool);
    QString printDir(QString,bool);
    void printBurbujaColor(Custom);
};

#endif // IMPRIMIR_H
