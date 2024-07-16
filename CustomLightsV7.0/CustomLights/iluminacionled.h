#ifndef ILUMINACIONLED_H
#define ILUMINACIONLED_H

#include <QWidget>
#include <QOpenGLWidget>
#include <GL/glu.h>
#include <GL/gl.h>
#include <QDebug>

class IluminacionLed : public QOpenGLWidget
{
private:
    int led_w, led_h;
    QColor *leds;

public:
    IluminacionLed(QWidget *parent);
    void loadLeds(int w, int h, QColor *l);

protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

    void setColor(int x, int y);

};

#endif // ILUMINACIONLED_H
