#include "iluminacionled.h"

IluminacionLed::IluminacionLed(QWidget *parent) : QOpenGLWidget(parent)
{
    this->led_h = 0;
    this->led_w = 0;
}

void IluminacionLed::initializeGL()
{
    // Set up the rendering context, load shaders and other resources, etc.:
    glClearColor(0,0,0,1);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_BLEND);
}

void IluminacionLed::resizeGL(int w, int h)
{
    //Update projection matrix and other size related settings:
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, (float)w/h, 0.01, 5000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    //gluLookAt(0,0,50,0,0,0,0,1,0);
    gluLookAt(this->width()/2,this->height()/2,700,this->width()/2,this->height()/2,0,0,1,0);
}

void IluminacionLed::loadLeds(int w, int h, QColor *c)
{
    this->led_w = w;
    this->led_h = h;

    this->leds = c;
}

void IluminacionLed::setColor(int x, int y)
{
    if(x == this->led_w || y == this->led_h || x < 0 || y < 0)
    {
       glColor3f(0, 0, 0);
    }
    else
    {
        QColor c =  this->leds[y * this->led_w + x];

        glColor3f(c.red() / 255.0, c.green() / 255.0, c.blue() / 255.0);
    }



}

void IluminacionLed::paintGL()
{

    if(led_h == 0 || led_w == 0)
        return;

    // Draw the scene:
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBegin(GL_QUADS);

    float quad_height = (float)this->height() / this->led_h;
    float quad_width = (float)this->width() / this->led_w;

    for(int x = -1; x < this->led_w; x++)
    {
        for(int y = -1; y < this->led_h; y++)
        {
            setColor(x, y);
            glVertex2f(0 + x * quad_width, 0 + y * quad_height);
            setColor(x, y + 1);
            glVertex2f(0 + x * quad_width, quad_height + y * quad_height);
            setColor(x + 1, y + 1);
            glVertex2f(quad_width + x * quad_width, quad_height + y * quad_height);
            setColor(x + 1, y);
            glVertex2f(quad_width + x * quad_width, 0 + y * quad_height);
        }
    }
    glEnd();


}
