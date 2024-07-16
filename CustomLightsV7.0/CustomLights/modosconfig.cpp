#include "modosconfig.h"

#define VALOR_PORCENTAJE_COLORES 90
#define VALOR_NEGROS 30 // THRESHOLD NEGROS

modosConfig::modosConfig(QObject *parent)
{

    //INICIALIZO VARIABLE CON VALORES POR DEFECTO POR LAS DUDAS (SE DEBEN CARGAR EN EL PROGRAMA)
    brillo = 50;
    modo = FIJO ;
    ancho_pantalla = 25;
    color_r = 255;
    color_g = 0;
    color_b = 0;
    brillo_volumen = false;
    cant_led = 0;
    configuracion_leds.aba=1;
    configuracion_leds.arr=1;
    configuracion_leds.der=1;
    configuracion_leds.izq=1;
    configuracion_leds.sentido=HORARIO;
    configuracion_leds.esquina = IZQ_ABA;

    desktop = GetDesktopWindow();
    p = new Pantalla();

    reset_trama();

    com_info.tipo_com = SERIE;
    esp32 = new ConfiguracionEsp32(parent,false);

}
modosConfig::~modosConfig(){
    enviar_apagar();
}
// --------- seteadores

void modosConfig::Set_leds_config(const HwConfig a){
    configuracion_leds = a;
    cant_led= LEDS_MODULO*(configuracion_leds.izq+configuracion_leds.der+configuracion_leds.aba+configuracion_leds.arr);
}

void modosConfig::Set_colores(const unsigned char r,const unsigned char g,const unsigned char b){
    color_r=r;
    color_g=g;
    color_b =b;
}

void modosConfig::Set_modo(const unsigned char m){
    if(m<=AUDIO){
        modo = m;
    }
}


// -------------Actualizadores

/*
Funcion que permite actualizar las luces para los distintos modos.
-Se incluye un escalador para variar las bases de tiempos
-si se desea agregar un modo es necesario incluir su funcion aca.

IMPORTANTE fijar en la variable TIME_D la base de tiempo con la que se llama
a la funcion para que los tiempos concuerden (base minima 10ms)
*/
void modosConfig::actualizar_luces(){
    static int escalador = 0;
    bool flag_change =0;
    static int anterior=-1;
    // en caso de tener activado el cambio de brillo por volumen lo actualizo
    if((brillo_volumen == true)&&(modo!=AUDIO))
        brillo = valor_vumetro;
    if(anterior != modo){
        anterior = modo;
        flag_change = 1;
    }
    switch(modo){
        case PUNCHI:
            if(escalador >= (300/TIME_D)){
                enviar_fiesta();
                escalador = 0;
            }
        break;
        case FIJO:
            if(escalador >= (60/TIME_D)){ // ENVIO CADA 300mS
                enviar_fijo();
                escalador = 0;
            }
        break;
        case FADE:
            if(escalador >= (50/TIME_D)){ // ENVIO CADA 50mS
                enviar_fade();
                escalador = 0;
            }
        break;
        case PANTALLA:
            if(escalador >= (40/TIME_D)){ //10
                p->actualizarPantalla();
                enviar_pantalla();
                escalador = 0;
            }

        break;
        case AUDIO:
            if(escalador >= (100/TIME_D)){ // ENVIO CADA 100mS
                enviar_sonido();
                escalador = 0;
            }
        break;
        case RAINBOW:
            if(escalador >= (60/TIME_D)){ // ENVIO CADA 60mS
                enviar_rainbow();
                escalador = 0;
            }
        break;

        case NINGUNO:
            if((escalador >= (300/TIME_D))||flag_change==1){ // ENVIO CADA 0,5S
                enviar_apagar();
                escalador = 0;
            }

        break;
        case APAGAR:
            enviar_apagar();
        break;
        case MOVI:
            if(escalador >= (40/TIME_D)){ // ENVIO CADA 100mS
                enviar_movimiento();
                escalador = 0;
            }
        break;
        case DAY_lIGHT:
            if((escalador >= (3000/TIME_D))||flag_change==1){ // ENVIO CADA 5s
                enviar_dayLight();
                escalador = 0;
            }

        break;
    }
    escalador++;
}


/*
funcion que se encarga de establecer el valor del sonido que se esta repoduciendo
Se realiza un calculo de la sensibilidad para poder variarla segun el volumen que se reproduce
*/
void modosConfig::actualizar_volumen(unsigned int max, unsigned int prom){
    static double promedio = 0;
    float sensibilidad;

    mi_buff.insert(mi_buff.size(),prom);
    if(mi_buff.size()>=200){ //estoy viendo los ultimos 4Seg de audio
       promedio -= mi_buff.takeFirst();
    }
    promedio += mi_buff.value(mi_buff.size()-1);
    sensibilidad = (((promedio /(double)mi_buff.size()))/(32768.0))*6;// realizo un ajuste de la sensibilidad que se esta tomando en el momento
    if(sensibilidad == 0) sensibilidad = 0.01;
    if(sensibilidad > 1) sensibilidad = 1;

    valor_vumetro=(int)((prom*255)/(sensibilidad*32768.0));// establezco el valor de volumen medido en relacion con la sensibilidad
    if(valor_vumetro>255) valor_vumetro =255;

}


// ------------ operadores tramas para enviar
/*
se setea el tipo de comunicacion que se va a usar
*/
void modosConfig::Set_Com(const ComInfo a){
    com_info = a;
    if(com_info.tipo_com == SERIE){
        serie.in_com_serie(com_info.nombre_com);
    }
    if(com_info.tipo_com == WIFI){
        if(serie.estaConectado())
            serie.cerrar_com();
        esp32->empezarConfig();
    }
}

void modosConfig::enviar_trama(){
    if(!serie.estaConectado()&&!esp32->estaConectado()) // en caso de no estar conectado por ningun medio no se enviar nada
        return;
    if(com_info.tipo_com == SERIE){
        if( cant_cargada >= cant_led){
            ordenar_trama();
            serie.mandar_datos(trama_completa);
            cant_cargada = 0;
        }
    }
    if(com_info.tipo_com == WIFI){
        if( cant_cargada >= cant_led){
            ordenar_trama();
            QString buff;

            for(int i =0 ;i < trama_completa.length();i++){

                if(i>1) trama_completa[i] = (char)((unsigned char) trama_completa[i] >> 1);

                if(trama_completa[i] == '\0' || trama_completa[i]=='<' || trama_completa[i] =='>')
                    buff +=trama_completa[i] + 1;
                else
                    buff +=trama_completa[i];

            }
            cant_cargada = 0;
            esp32->mandar_datos( buff );
        }
    }
}

void modosConfig::agregar_trama(char r, char g, char b, int cant, char brillo_t){
    if( cant_cargada >= cant_led){
        return;
    }
    // reinicio la trama
    if(cant_cargada == 0){
        trama_completa.clear();
        trama_completa += '$';
        trama_completa += cant_led;
        trama_completa += brillo_t;
    }
    //cargo la cantidad de leds que se ingresaron
    for(int i = 0; i<cant && cant_cargada < cant_led ;i++,cant_cargada++){
        trama_completa += r;
        trama_completa += g;
        trama_completa += b;
    }

}


/*
Funcion que permite adaptar la trama segun la configuracion que se haya hecho
El sentido que se toma como base es la esquina izquierda inferior, luego el resto de tramas
se adaptan de forma que se pueda reproducir en las distintas configuraciones.
*/
void modosConfig::ordenar_trama(){
    QByteArray buff_trama = trama_completa;
    int lado=configuracion_leds.esquina;
    if(configuracion_leds.sentido==HORARIO){
        switch(lado){
        case IZQ_ABA:
            //en este caso no toco nada porque esta en orden
            break;
        case IZQ_ARR:
            trama_completa.clear();
            trama_completa += buff_trama.mid(0,3);
            buff_trama.remove(0,3);
            trama_completa += buff_trama.mid((configuracion_leds.izq)*3,configuracion_leds.arr*3);
            buff_trama.remove((configuracion_leds.izq)*3,configuracion_leds.arr*3);
            trama_completa += buff_trama.mid((configuracion_leds.izq)*3,configuracion_leds.der*3);
            buff_trama.remove((configuracion_leds.izq)*3,configuracion_leds.der*3);
            trama_completa += buff_trama.mid((configuracion_leds.izq)*3,configuracion_leds.aba*3);
            buff_trama.remove((configuracion_leds.izq)*3,configuracion_leds.aba*3);
            trama_completa += buff_trama.mid(0,configuracion_leds.izq*3);
            buff_trama.remove(0,configuracion_leds.izq*3);

            break;
        case DER_ARR:
            trama_completa.clear();
            trama_completa += buff_trama.mid(0,3);
            buff_trama.remove(0,3);
            trama_completa += buff_trama.mid((configuracion_leds.izq+configuracion_leds.arr)*3,configuracion_leds.der*3);
            buff_trama.remove((configuracion_leds.izq+configuracion_leds.arr)*3,configuracion_leds.der*3);
            trama_completa += buff_trama.mid((configuracion_leds.izq+configuracion_leds.arr)*3,configuracion_leds.aba*3);
            buff_trama.remove((configuracion_leds.izq+configuracion_leds.arr)*3,configuracion_leds.aba*3);
            trama_completa += buff_trama.mid(0,configuracion_leds.izq*3);
            buff_trama.remove(0,configuracion_leds.izq*3);
            trama_completa += buff_trama.mid(0,configuracion_leds.arr*3);
            buff_trama.remove(0,configuracion_leds.arr*3);

            break;
        case DER_ABA:
            trama_completa.clear();
            trama_completa += buff_trama.mid(0,3);
            buff_trama.remove(0,3);
            trama_completa += buff_trama.mid((configuracion_leds.izq+configuracion_leds.arr+configuracion_leds.der)*3,configuracion_leds.aba*3);
            buff_trama.remove((configuracion_leds.izq+configuracion_leds.arr+configuracion_leds.der)*3,configuracion_leds.aba*3);
            trama_completa += buff_trama.mid(0,configuracion_leds.izq*3);
            buff_trama.remove(0,configuracion_leds.izq*3);
            trama_completa += buff_trama.mid(0,configuracion_leds.arr*3);
            buff_trama.remove(0,configuracion_leds.arr*3);
            trama_completa += buff_trama.mid(0,configuracion_leds.der*3);
            buff_trama.remove(0,configuracion_leds.der*3);
            break;
        }
    }
    else{ // SENTIDO ANTIHORARIO
        switch(lado){
        case IZQ_ABA:
            trama_completa.clear();
            trama_completa += buff_trama.mid(0,3);
            buff_trama.remove(0,3);
            buff_trama = Dar_vuelta_trama(buff_trama);
            trama_completa += buff_trama;
            break;
        case IZQ_ARR:
            trama_completa.clear();
            trama_completa += buff_trama.mid(0,3);
            buff_trama.remove(0,3);
            buff_trama = Dar_vuelta_trama(buff_trama);
            trama_completa += buff_trama.mid((configuracion_leds.aba+configuracion_leds.der+configuracion_leds.arr)*3,configuracion_leds.izq*3);
            buff_trama.remove((configuracion_leds.aba+configuracion_leds.der+configuracion_leds.arr)*3,configuracion_leds.izq*3);
            trama_completa += buff_trama;

            break;
        case DER_ABA:
            trama_completa.clear();
            trama_completa += buff_trama.mid(0,3);
            buff_trama.remove(0,3);
            buff_trama = Dar_vuelta_trama(buff_trama);
            trama_completa += buff_trama.mid((configuracion_leds.aba)*3,configuracion_leds.der*3);
            buff_trama.remove((configuracion_leds.aba)*3,configuracion_leds.der*3);
            trama_completa += buff_trama.mid((configuracion_leds.aba)*3,configuracion_leds.arr*3);
            buff_trama.remove((configuracion_leds.aba)*3,configuracion_leds.arr*3);
            trama_completa += buff_trama.mid((configuracion_leds.aba)*3,configuracion_leds.izq*3);
            buff_trama.remove((configuracion_leds.aba)*3,configuracion_leds.izq*3);
            trama_completa += buff_trama;
            break;
        case DER_ARR:
            trama_completa.clear();
            trama_completa += buff_trama.mid(0,3);
            buff_trama.remove(0,3);
            buff_trama = Dar_vuelta_trama(buff_trama);
            trama_completa += buff_trama.mid((configuracion_leds.aba+configuracion_leds.der)*3,configuracion_leds.arr*3);
            buff_trama.remove((configuracion_leds.aba+configuracion_leds.der)*3,configuracion_leds.arr*3);
            trama_completa += buff_trama.mid((configuracion_leds.aba+configuracion_leds.der)*3,configuracion_leds.izq*3);
            buff_trama.remove((configuracion_leds.aba+configuracion_leds.der)*3,configuracion_leds.izq*3);
            trama_completa += buff_trama;
            break;
        }
    }
    trama_completa += "#"; // agrego final de trama
}

/*
funcion utilizada para ordenar la trama
*/
QByteArray modosConfig::Dar_vuelta_trama(QByteArray a){
    QByteArray buff;
    int contar = a.length()/3;
    for(int i =0;i<=contar;i++){
        buff += a.mid(a.length()-3*i,3);
    }
    return buff;
}

void modosConfig::enviar_apagar(){
    reset_trama();
    agregar_trama_apagar(cant_led);
    enviar_trama();
}

void modosConfig::reset_trama(){
    cant_cargada = 0;
    trama_completa.clear();
}

//--------------- ENVIAR MODOS

void modosConfig::enviar_fijo(){
    agregar_trama(color_r,color_g,color_b,cant_led,brillo);
    enviar_trama();

}

void modosConfig::enviar_fade(){
    static bool sentido = 0;
    static unsigned char brillo_aux=255;
    if(brillo_aux <= 1)
        sentido = 1;
    if(brillo_aux>254)
        sentido = 0;
    if(sentido)
        brillo_aux++;
    else
        brillo_aux--;
    agregar_trama(color_r,color_g,color_b,cant_led,brillo_aux);
    enviar_trama();

}

void modosConfig::enviar_dayLight(){
    //static int prueba=0;
    int horas = time.currentTime().hour();
    int minutos = time.currentTime().minute();
    float calculo=0 ;
    float x = (float)(horas + (minutos/60.0));
    /*static float prueba=0; // para probar rapido
    //prueba += 0.05;
    //x = prueba;
    if (x > 24) prueba =0;*/
    calculo = sqrt(255*255 - (((x - 13.5) * 34)*(x - 13.5) * 34));
    agregar_trama(color_r,color_g,color_b,cant_led,calculo);
    enviar_trama();
}

void modosConfig::enviar_rainbow(){
    static int estado=1;
    static unsigned int r=254,b=126,g=0;

    switch(estado){//variacion luces
    case 1:
        r--;
        if(r<=140) estado ++;
        break;
    case 2:
        g++;
        if(g>=250) estado ++;
        break;
    case 3:
        b++;
        if(b>=250) estado ++;
        break;
    case 4:
        r++;
        b--;
        if(r>=250||b<=0) estado ++;
        break;
    case 5:
        g--;
        if(g<=0) estado++;
        break;
    case 6:
        b++;
        if(b>=146) estado = 1;
        break;
     default:
        estado = 1;
    }
    agregar_trama(r,g,b,cant_led,brillo);
    enviar_trama();
}

void modosConfig::enviar_fiesta(){
    unsigned char r,g,b;
    r = rand()%255;
    g = rand()%255;
    b = rand()%255;
    agregar_trama(r,g,b,cant_led,brillo);
    enviar_trama();
}

void modosConfig::enviar_sonido(){
    int cant_leds_disponibles = cant_led - configuracion_leds.aba-1; // no prendo los de abajo ;
    int leds_prender=(int) ((cant_leds_disponibles/2)*valor_vumetro)/(255.0) ;

   if(leds_prender==0){
       agregar_trama(0,0,0,cant_led+10,0);
       return;
   }
   agregar_trama(color_r,color_g,color_b,leds_prender,brillo);
   agregar_trama(0,0,0, cant_leds_disponibles-(2*leds_prender),0);
   agregar_trama(color_r,color_g,color_b,100,brillo);
   enviar_trama();

}

void modosConfig::enviar_prueba(const int izq,const int aba,const int der,const int arr){
    reset_trama();
    if(izq == ON)
       agregar_trama((char)255,(char)255,(char)255,configuracion_leds.izq,(char)255);
    else
       agregar_trama((char)0,(char)0,(char)0,configuracion_leds.izq,(char)255);
    if(arr == ON)
       agregar_trama((char)255,(char)255,(char)255,configuracion_leds.arr,(char)255);
    else
       agregar_trama((char)0,(char)0,(char)0,configuracion_leds.arr,(char)255);
    if(der == ON)
        agregar_trama((char)255,(char)255,(char)255,configuracion_leds.der,(char)255);
    else
        agregar_trama((char)0,(char)0,(char)0,configuracion_leds.der,(char)255);
    if(aba == ON)
        agregar_trama((char)255,(char)255,(char)255,configuracion_leds.aba,(char)255);
     else
        agregar_trama((char)0,(char)0,(char)0,configuracion_leds.aba,(char)255);

    enviar_trama();
}

void modosConfig::agregar_trama_apagar(int cant){
    agregar_trama(0,0,0,cant,0);

}

void modosConfig::agregar_led(QColor pixel){
    agregar_trama((unsigned char) pixel.red(),(unsigned char)pixel.green(),(unsigned char) pixel.blue(),1,brillo);
}

void modosConfig::enviar_movimiento(){

    static int leds_prender_pos = 0;
    static bool direc=HORARIO;
    if(direc==ANTIHORARIO){
        leds_prender_pos++;
        if(leds_prender_pos >= cant_led-4)
            direc = HORARIO;
    }
    else{
        leds_prender_pos--;
        if(leds_prender_pos <= 0)
            direc = ANTIHORARIO;
    }
    if(leds_prender_pos < 0)
        leds_prender_pos = 0;

    agregar_trama(0,0,0,1,brillo);
    agregar_trama_apagar(leds_prender_pos);
    agregar_trama( color_r, color_g, color_b,3,brillo);

    if((leds_prender_pos+4)<=cant_led)
        agregar_trama_apagar(cant_led+10);
    enviar_trama();
}

//----------------- operadores pantalla

void modosConfig::enviar_pantalla()
{
    float scale =  0.005 + (ancho_pantalla/1500.0);
    int lado=IZQ_ABA;
    QImage img = p->devolverImagen();
    int w =configuracion_leds.arr;
    int h =configuracion_leds.izq;
    int pixelWidth = img.width() / w;
    int pixelHeight = img.height() / h;
    int pixelWidthMax = img.width() * scale;
    int pixelHeightMax = img.height() * scale;
    QColor c;
    int x = 0, y = 0;

    reset_trama();

    for(int cant_lados = 0;cant_lados<4;cant_lados++){
        switch(lado){
            case IZQ_ABA:
                for(y = 1; y < h; y++)
                {
                    c = getAverageRGB(img, pixelWidthMax / 2, y * pixelHeight + (pixelHeight/2), pixelWidthMax, pixelHeight);
                    agregar_led(c);
                }// envio lado izquierdo user
                agregar_led(c);
                lado = IZQ_ARR;
                break;

            case IZQ_ARR:
                for(x = 1; x < w; x++)
                {
                    c = getAverageRGB(img, x * pixelWidth + (pixelWidth/2), y * pixelHeight - (pixelHeightMax / 2), pixelWidth, pixelHeightMax);
                    agregar_led(c);
                } // envio lado superior user
                agregar_led(c);
                lado = DER_ARR;
                break;
            case DER_ARR:
                for(y = h-1,x = w; y > 0; y--)
                {
                    c = getAverageRGB(img, x * pixelWidth - (pixelWidthMax / 2), y * pixelHeight + (pixelHeight/2), pixelWidthMax, pixelHeight);
                    agregar_led(c);
                } // envio lado derecho user
                agregar_led(c);
                lado = DER_ABA;
                break;
            case DER_ABA:
                for(x = w-1; x > 0; x--)
                {
                   c = getAverageRGB(img, x * pixelWidth + (pixelWidth/2), pixelHeightMax / 2, pixelWidth, pixelHeightMax);
                   agregar_led(c);
                } // envio lado abajo user*/
                agregar_led(c);
                lado = IZQ_ABA;
                break;
        }
    }
    enviar_trama();
}

QColor modosConfig::getAverageRGB(QImage img, int x, int y, int w, int h)
{

    long int r = 0;
    long int g = 0;
    long int b = 0;
    int n = 0;
    for(int i = (x - w / 2); i < (x + w / 2); i++)
    {
     for(int j = (y - h / 2); j < (y + h / 2); j++)
     {
         if(i < 0 || i >= img.width() || j < 0 || j >= img.height())
         {
             qDebug() << "Error en getAverageRGB: afuera del rango de la imagen";
             continue;
         }
         QColor c = img.pixelColor(i, j);

         r += c.red() ;//* c.red();
         g += c.green() ;//* c.green();
         b += c.blue() ;//* c.blue();

         n++;
     }
    }
    if(n != 0){
        QColor ret(r/n, g/n, b/n);
      //QColor ret(sqrt(r/n), sqrt(g/n), sqrt(b/n));
      if(ret.blue()<VALOR_NEGROS && ret.red()<VALOR_NEGROS && ret.green()<VALOR_NEGROS)
              return QColor(0, 0, 0);

    return ret;
    }

    else
      return QColor(0, 0, 0);
}


