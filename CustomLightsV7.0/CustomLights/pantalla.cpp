#include "pantalla.h"

Pantalla::Pantalla()
{

}

Pantalla::~Pantalla()
{


}

void Pantalla::actualizarPantalla()
{

    // datos de la pantalla
    width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    height = GetSystemMetrics(SM_CYVIRTUALSCREEN);

    // bitmap header
    bitmap.bmiHeader.biSize = sizeof(bitmap.bmiHeader);
    bitmap.bmiHeader.biWidth = width;
    bitmap.bmiHeader.biHeight = -height;
    bitmap.bmiHeader.biPlanes = 1;
    bitmap.bmiHeader.biBitCount = 24;
    bitmap.bmiHeader.biCompression = BI_RGB;
    bitmap.bmiHeader.biSizeImage = 0;
    bitmap.bmiHeader.biClrUsed = 0;
    bitmap.bmiHeader.biClrImportant = 0;

    hdc = GetDC(HWND_DESKTOP); // contexto del escritorio


    hBitmap = CreateDIBSection(hdcTemp, &bitmap, DIB_RGB_COLORS, NULL, NULL, (DWORD)NULL); // creo el bitmap

    hdcTemp = CreateCompatibleDC(hdc); // creo un contexto temporal

    HGDIOBJ hPrevBitmap = SelectObject(hdcTemp, hBitmap); // guardo el contexto anterior y asigno el temporal

    BitBlt(hdcTemp, 0, 0, width, height, hdc, 0, 0, SRCCOPY); // copio la pantalla en el bitmap

    screen = QtWin::fromHBITMAP(hBitmap).toImage(); // guardo el bitmap en el QImage

    SelectObject(hdcTemp, hPrevBitmap); // vuelvo al contexto anterior

    DeleteDC(hdcTemp); // libero la memoria del contexto temporal


    // libero la memoria pedida
    DeleteObject(hBitmap);
    ReleaseDC(HWND_DESKTOP, hdc);
}

QImage Pantalla::devolverImagen()
{
    return screen.mirrored(false,true);
}

QPixmap Pantalla::devolverPixmap()
{
    return QPixmap::fromImage(screen.mirrored(false,true));
}
