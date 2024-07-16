#include "datosusuario.h"


DatosUsuario::DatosUsuario() {
    m_ruta = "-";

    m_HwConfig.izq=0;
    m_HwConfig.arr=0;
    m_HwConfig.der=0;
    m_HwConfig.aba=0;
    m_HwConfig.sentido=0;
    m_HwConfig.esquina=0;

    m_HwInfo.cant_leds = 10 ;
    m_HwInfo.largo = 20;
    m_HwInfo.max_brillo = 255;

    m_Custom.modo=0;
    m_Custom.alto_borde = 25;
    m_Custom.ancho_borde = 25;
    m_Custom.R = 85;
    m_Custom.G = 170;
    m_Custom.B = 255;
    m_Custom.flag_brillo=0;
    m_Custom.brillo=99;

    comunicacion_info.nombre_com = "COM1";
    comunicacion_info.tipo_com = SERIE;

    audio_default = "NOT SET";
}

DatosUsuario::~DatosUsuario(){
}

bool DatosUsuario::existeConfig(){
    FILE *p=NULL;
    if((p=fopen( (char*)NOMBRE ,"r"))==NULL)
       return false;
    fclose(p);
    return true;
}

bool DatosUsuario::escribirConfig(){
    FILE *p=NULL;

    if((p=fopen( (char*)NOMBRE , "w" ))==NULL)
        return 0;

    fprintf(p,"#INFORMACION DE HARDWARE#\n");
    fprintf(p,"CANT_LEDS: %i\n", m_HwInfo.cant_leds);
    fprintf(p,"LARGO_MOD: %i\n", m_HwInfo.largo );
    fprintf(p,"MAX_BRILLO: %i\n", m_HwInfo.max_brillo );

    fprintf(p,"\n#CONFIGURACION DE HARDWARE#\n");
    fprintf(p,"CANT_MOD_IZQ: %i\n", m_HwConfig.izq );
    fprintf(p,"CANT_MOD_ARRIBA: %i\n", m_HwConfig.arr );
    fprintf(p,"CANT_MOD_DER: %i\n", m_HwConfig.der );
    fprintf(p,"CANT_MOD_ABAJO: %i\n", m_HwConfig.aba );
    fprintf(p,"SENT_GIRO: %i\n", m_HwConfig.sentido );
    fprintf(p,"ESQUINA: %i\n", m_HwConfig.esquina);

    fprintf(p,"\n#CONFIGURACION DE LA PERSONALIZACION#\n");
    fprintf(p,"MODO_EN_USO: %i\n", m_Custom.modo);
    fprintf(p,"ANCHO_BORDE: %i\n", m_Custom.ancho_borde);
    fprintf(p,"ALTO_BORDE: %i\n", m_Custom.alto_borde);
    fprintf(p,"R_EN_USO: %i\n", m_Custom.R);
    fprintf(p,"G_EN_USO: %i\n", m_Custom.G);
    fprintf(p,"B_EN_USO: %i\n", m_Custom.B);
    fprintf(p,"FLAG_BRILLO: %i\n", m_Custom.flag_brillo);
    fprintf(p,"BRILLO_EN_USO: %i\n", m_Custom.brillo);

    fprintf(p,"\n#RUTA DEFAULT#\n");
    fprintf(p,"RUTA: %s\n", m_ruta.toLatin1().data() );

    fprintf(p,"\n#DATOS COMUNICACION#\n");
    fprintf(p,"MODO_COM: %i\n", comunicacion_info.tipo_com );
    fprintf(p,"NOMBRE_COM:%s\n", comunicacion_info.nombre_com.c_str() );

    fprintf(p,"\n#AUDIO DEFAULT#\n");
    fprintf(p,"AUDIO_INPUT:%s\n", audio_default.toLatin1().data() );

    fprintf(p,"#FIN#\n");
    fclose(p);
    return 1;
}

bool DatosUsuario::leerConfig() {
    FILE *p=NULL;
    char *punt= NULL;
    char buff[1024];

    if((p=fopen( (char*)NOMBRE , "r" ))==NULL)
        return 0;

    while(!feof(p)){
        fgets(buff,1024,p);
        if((punt = strtok(buff,":"))!=NULL) {
            //------------------------------------------------------------HwInfo
            if(!strcmp(punt,"CANT_LEDS")){
                punt = strtok(NULL,":");
                m_HwInfo.cant_leds = atoi(punt);
             }
            if(!strcmp(punt,"LARGO_MOD")){
                punt = strtok(NULL,":");
                m_HwInfo.largo = atoi(punt);
            }
            if(!strcmp(punt,"MAX_BRILLO")){
                punt = strtok(NULL,":");
                m_HwInfo.max_brillo = atoi(punt);
            }
            //------------------------------------------------------------HwConfig
            if(!strcmp(punt,"CANT_MOD_IZQ")){
                punt = strtok(NULL,":");
                m_HwConfig.izq = atoi(punt);
            }
            if(!strcmp(punt,"CANT_MOD_ARRIBA")){
                punt = strtok(NULL,":");
                m_HwConfig.arr = atoi(punt);
            }
            if(!strcmp(punt,"CANT_MOD_DER")){
                punt = strtok(NULL,":");
                m_HwConfig.der = atoi(punt);
            }
            if(!strcmp(punt,"CANT_MOD_ABAJO")){
                punt = strtok(NULL,":");
                m_HwConfig.aba = atoi(punt);
            }
            if(!strcmp(punt,"SENT_GIRO")){
                punt = strtok(NULL,":");
                m_HwConfig.sentido= atoi(punt);
            }
            if(!strcmp(punt,"ESQUINA")){
                punt = strtok(NULL,":");
                m_HwConfig.esquina = atoi(punt);
            }
            //------------------------------------------------------------Custom
            if(!strcmp(punt,"MODO_EN_USO")){
                punt = strtok(NULL,":");
                m_Custom.modo = atoi(punt);
            }
            if(!strcmp(punt,"ANCHO_BORDE")){
                punt = strtok(NULL,":");
                 m_Custom.ancho_borde= atoi(punt);
            }
            if(!strcmp(punt,"ALTO_BORDE")){
                punt = strtok(NULL,":");
                m_Custom.alto_borde = atoi(punt);
            }
            if(!strcmp(punt,"R_EN_USO")){
                punt = strtok(NULL,":");
                m_Custom.R = atoi(punt);
            }
            if(!strcmp(punt,"G_EN_USO")){
                punt = strtok(NULL,":");
                m_Custom.G = atoi(punt);
            }
            if(!strcmp(punt,"B_EN_USO")){
                punt = strtok(NULL,":");
                m_Custom.B = atoi(punt);
            }
            if(!strcmp(punt,"FLAG_BRILLO")){
                punt = strtok(NULL,":");
                m_Custom.flag_brillo = atoi(punt);
            }
            if(!strcmp(punt,"BRILLO_EN_USO")){
                punt = strtok(NULL,":");
                m_Custom.brillo = atoi(punt);
            }
            //------------------------------------------------------------Ruta
            if(!strcmp(punt,"RUTA")){
                punt = strtok(NULL,":");
                int len = strlen(punt);
                punt[len-1] = 0;
                if(punt[0]==' ')
                    punt+=1;
                QString ruta(punt);
                m_ruta = ruta;
             }
            //---------------------------------------------- Tipo de transmicion
            if(!strcmp(punt,"MODO_COM")){
                punt = strtok(NULL,":");
                comunicacion_info.tipo_com = atoi(punt);
            }
            if(!strcmp(punt,"NOMBRE_COM")){
                punt = strtok(NULL,":");
                comunicacion_info.nombre_com = punt;
                //comunicacion_info.nombre_com.erase(comunicacion_info.nombre_com.length(),1);
            }
            //---------------------------------------------- entrada de audio
            if(!strcmp(punt,"AUDIO_INPUT")){
                punt = strtok(NULL,":");
                audio_default = punt;
                audio_default.remove(audio_default.length()-1,1);
            }
        }
    }
    fclose(p);
    return true;
}

bool DatosUsuario::escribirCustom( QString ruta ) {
    if(ruta.isEmpty())
        return false;

    FILE *p = NULL;
    if((p=fopen( ruta.toLatin1().data() , "w" ))==NULL)
        return false;

    fprintf(p,"#CONFIGURACION DE LA PERSONALIZACION#\n");
    fprintf(p,"MODO_EN_USO: %i\n", m_Custom.modo);
    fprintf(p,"ANCHO_BORDE: %i\n", m_Custom.ancho_borde);
    fprintf(p,"ALTO_BORDE: %i\n", m_Custom.alto_borde);
    fprintf(p,"R_EN_USO: %i\n", m_Custom.R);
    fprintf(p,"G_EN_USO: %i\n", m_Custom.G);
    fprintf(p,"B_EN_USO: %i\n", m_Custom.B);
    fprintf(p,"FLAG_BRILLO: %i\n", m_Custom.flag_brillo);
    fprintf(p,"BRILLO_EN_USO: %i\n", m_Custom.brillo);
    fprintf(p,"#FIN#\n");
    fclose(p);
    return true;
}

bool DatosUsuario::leerCustom( QString ruta ) {
    if(ruta.isEmpty())
        return false;

    FILE *p=NULL;
    char *punt= NULL;
    char buff[1024];

    if( (p=fopen(ruta.toLatin1().data(),"r")) == NULL )
        return false;

    while(!feof(p)){
        fgets(buff,1024,p);
        if((punt = strtok(buff,":"))!=NULL) {
            //------------------------------------------------------------Custom
            if(!strcmp(punt,"MODO_EN_USO")){
                punt = strtok(NULL,":");
                m_Custom.modo = atoi(punt);
            }
            if(!strcmp(punt,"ANCHO_BORDE")){
                punt = strtok(NULL,":");
                 m_Custom.ancho_borde= atoi(punt);
            }
            if(!strcmp(punt,"ALTO_BORDE")){
                punt = strtok(NULL,":");
                m_Custom.alto_borde = atoi(punt);
            }
            if(!strcmp(punt,"R_EN_USO")){
                punt = strtok(NULL,":");
                m_Custom.R = atoi(punt);
            }
            if(!strcmp(punt,"G_EN_USO")){
                punt = strtok(NULL,":");
                m_Custom.G = atoi(punt);
            }
            if(!strcmp(punt,"B_EN_USO")){
                punt = strtok(NULL,":");
                m_Custom.B = atoi(punt);
            }
            if(!strcmp(punt,"FLAG_BRILLO")){
                punt = strtok(NULL,":");
                m_Custom.flag_brillo = atoi(punt);
            }
            if(!strcmp(punt,"BRILLO_EN_USO")){
                punt = strtok(NULL,":");
                m_Custom.brillo = atoi(punt);
            }
        }
    }
    fclose(p);
    return true;
}
