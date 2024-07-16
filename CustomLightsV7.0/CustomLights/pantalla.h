#ifndef PANTALLA_H
#define PANTALLA_H

#include <windows.h>
#include <QDebug>
#include <QPixmap>
#include <QtWinExtras/QtWinExtras>

class Pantalla
{
private:
    int width, height;

    HDC hdcTemp, hdc;

    BITMAPINFO bitmap;
    HBITMAP hBitmap;

    QImage screen;


public:
    Pantalla();
    ~Pantalla();

    void actualizarPantalla();

    QPixmap devolverPixmap();

    QImage devolverImagen();

};

#endif // PANTALLA_H
