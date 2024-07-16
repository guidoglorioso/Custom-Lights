#include "mainwindow.h"
#include "ui_mainwindow.h"

QTimer *timerMainWindow = new QTimer();
QTimer *timerData = new QTimer();


//--------------------------------------------Constructor / Destructor
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow),mc(this) {

    ui->setupUi(this);

    //No se puede minimizar
    setFixedSize(width(), height());

    //Seteo Text Edit en lectura
    setTextEditReadOnlyAll();

    //Agrego modos al combo box
    f_aux = false;
    agregarModos();
    f_aux = true;

    //Inicializo clase Imprimir
    print.setTeHwEstado( ui->Hw_TE_Estado );
    print.setTeHwInfo( ui->Hw_TE_Info );
    print.setTeRedEstado( ui->Red_TE_Estado );
    print.setTeRedInfo( ui->Red_TE_Info );
    print.setTeDir( ui->Dir_textEdit );
    print.setTeModo( ui->Modo_TE );
    print.setBurbujaColor( ui->Color_LB );

    //Inicio timers
    connect(timerMainWindow, SIGNAL(timeout()),this,SLOT( interruptTimer() ));
    timerMainWindow->start(TIME);
    connect(timerData, SIGNAL(timeout()),this,SLOT( TimerData1ms() ));
    timerData->start(TIME_D);


    //Inicio UDP

    f_ConfigRed =false;
    f_ConfigHW = false;
    habGeneral( f_ConfigHW&&f_ConfigRed );

    //Levanto archivo de configuracion
    cargarConfig();

    //Deshabilito funciones hasta cheuqear que este configurado y conectado
    habGeneral(false);

    //audio
    inicializacion_audio();

}

MainWindow::~MainWindow() {
    //guardo informacion del programa
    datos.setAudioInput(ui->SONIDO_CB->currentText());
    datos.escribirConfig();

    delete ui;
}

//--------------------------------------------Metodos de inicializacion
void MainWindow::setTextEditReadOnlyAll() {
    ui->Modo_TE->setReadOnly(true);
    ui->Red_TE_Info->setReadOnly(true);
    ui->Hw_TE_Info->setReadOnly(true);
    ui->Dir_textEdit->setReadOnly(true);
    ui->Hw_TE_Estado->setReadOnly(true);
    ui->Red_TE_Estado->setReadOnly(true);
}


/*
 * funcion que agrega las opciones de los distintos modos al comboBox
*/
void MainWindow::agregarModos(){
    ui->Modo_CB->addItem("Ninguno",0);
    ui->Modo_CB->addItem("Fade Rojo",0);
    ui->Modo_CB->addItem("Fade Verde",0);
    ui->Modo_CB->addItem("Fade Azul",0);
    ui->Modo_CB->addItem("Fade",0);
    ui->Modo_CB->addItem("Color Fijo",0);
    ui->Modo_CB->addItem("Pantalla",0);
    ui->Modo_CB->addItem("Punchi",0);
    ui->Modo_CB->addItem("Sonido",0);
    ui->Modo_CB->addItem("Movi",0);
    ui->Modo_CB->addItem("Day Light",0);
    ui->Modo_CB->addItem("Rainbow",0);
}

//--------------------------------------------Metodos de lectura y actualizacion de datos

/*
 * funcion que carga la configuracion del archivo de texto y en caso de no existir este archivo lo crea
*/
void MainWindow::cargarConfig() {
    //Si no estaba creado, rutina bienvenida
    //Pide datos y crea archivo
    if( !datos.existeConfig() ) {
        NoConfigWindow ventana(this);
        ventana.setModal(true);
        ventana.exec();
    }
    //Levanto datos
    datos.leerConfig();

    //cargo parametros a la clase que contra los leds
    mc.Set_leds_config(datos.getHwConfig());
    mc.Set_Com(datos.getComInfo());

    //Cargo Custom
    setPersonalizacion();

    //Cargo y valido dirDir
    QString aux = print.printDir( datos.getRuta() , existeDir(datos.getRuta()) );
    datos.setRuta(aux);

    //Cargo Hw Config
    f_ConfigHW = print.printHwInfo(datos.getHwConfig());
    habGeneral( f_ConfigHW&&f_ConfigRed );

    print.printHwEstado(f_ConfigHW);

    if( !f_ConfigHW )
        mensajeError();
// aca tenes que meter eso
    ui->Modo_CB->setCurrentIndex(datos.getCustom().modo);
    on_Modo_CB_currentTextChanged(ui->Modo_CB->currentText());

}

/*
 * mensaje que aparece en caso de que no se haya podido configurar correctamente el archivo
*/
void MainWindow::mensajeError() {
    QMessageBox msgBox;
    msgBox.setText("El archivo de configuracion estaba dañado!\n"
    "Debe volver a configurar los parámetros erróneos desde la ventana configuración.\n"
    "Recuerde que para poder usar las funcionalidades los parámetros de red y hardware"
    "deben estar necesariamente configurados");
    msgBox.exec();
}

/*
 * funcion que establece los parametros levantados de una personalizacion en la interfaz grafica
*/
void MainWindow::setPersonalizacion() {
    //Cargo Modo
    ui->Modo_CB->setCurrentIndex( datos.getCustom().modo );

    //Cargo Config de Borde
    ui->Borde_Ancho->setValue( datos.getCustom().ancho_borde );
    ui->Borde_Ancho_Aux->setText( QString::number(datos.getCustom().ancho_borde)+"%" );

    //Cargo Colores
    ui->Color_R_SLI->setValue( datos.getCustom().R );
    ui->Color_R_Aux->setText( QString::number(datos.getCustom().R) );
    ui->Color_G_SLI->setValue( datos.getCustom().G );
    ui->Color_G_Aux->setText( QString::number(datos.getCustom().G) );
    ui->Color_B_SLI->setValue( datos.getCustom().B );
    ui->Color_B_Aux->setText( QString::number(datos.getCustom().B) );
    setHab();

    //Cargo Brillo
    ui->Brillo_SLI->setValue( datos.getCustom().brillo );
    ui->Brillo_Aux->setText(QString::number( datos.getCustom().brillo)+"%");
    ui->Color_B_SLI->setValue( datos.getCustom().B );
}

//--------------------------------------------Validaciones

/*
 * timer que comprueba la conexion al wifi
*/
void MainWindow::interruptTimer() {

    if(datos.getComInfo().tipo_com == WIFI){
        mc.actualizarUDP(); // llama al metodo ME_configuracion cada 1 seg
    }
    if(!mc.Get_Estado_wifi())
        f_ConfigRed = false;
    else{
        f_ConfigRed = true;
        print.printRedInfo("Red Local", mc.Get_IP());
    }
    print.printRedEstado(f_ConfigRed);

    habGeneral( f_ConfigHW );

    //Valido ruta por si la borraron
    QString aux = print.printDir( datos.getRuta() , existeDir(datos.getRuta()) );
    datos.setRuta(aux);

}

void MainWindow::habGeneral(bool i) {
    if (i) {
        ui->tab->setEnabled(true);
        ui->Test_PB->setEnabled(true);
        setHab();
        int mem = datos.getCustom().modo;
        ui->Modo_CB->setCurrentIndex(mem);
        setPersonalizacion();
    } else {
        ui->tab->setDisabled(true);
        ui->Test_PB->setDisabled(true);
        print.printModo(f_ConfigHW,f_ConfigRed);
        habBorde(0);
        habBrillo(0);
        habColor(0);
    }
}


/*
 * funcion que dependiendo del modo habilita o no los diversos recursos de la app
*/
void MainWindow::setHab(){
    switch(datos.getCustom().modo) {
        default:
        case INDEX_NINGUNO:
            habBorde(0);
            habBrillo(0);
            habColor(0);
            break;
        case INDEX_FADE_R:
        case INDEX_FADE_V:
        case INDEX_FADE_A:
            habBorde(0);
            habBrillo(0);
            habColor(0);
            break;
        case INDEX_FADE:
            habBorde(0);
            habBrillo(0);
            habColor(1);
            break;
        case INDEX_FIJO:
            habBorde(0);
            habBrillo(1);
            habColor(1);
            break;
        case INDEX_PANTALLA:
            habBorde(1);
            habBrillo(1);
            habColor(0);
            break;
        case INDEX_PUNCHI:
            habBorde(0);
            habBrillo(1);
            habColor(0);
            break;
        case INDEX_SONIDO:
            habBorde(0);
            habBrillo(1);
            habColor(1);
            break;
        case INDEX_RAINBOW:
            habBorde(0);
            habBrillo(1);
            habColor(0);
            break;
        case INDEX_MOVI:
            habBorde(0);
            habBrillo(1);
            habColor(1);
        break;
        case INDEX_DAY_LIGHT:
            habColor(1);
            habBrillo(0);
            habBorde(0);
        break;

    }
}



//--------------------------------------------Modo

/*
 * seleccion del modo que se desea utilizar
*/
void MainWindow::on_Modo_CB_currentTextChanged(const QString &arg1) {

    if(!f_aux) // Evita que cuando se cargan los modos se seleccione alguna opcion
        return;

    if(arg1== "Ninguno"){
        print.printModo("No hay ningún modo seleccionado!","Elige uno para encender las luces",QColor(140,40,40));
        habColor(0);
        habBorde(0);
        habBrillo(0);
        mc.Set_colores(datos.getCustom().R,datos.getCustom().G,datos.getCustom().B);
        mc.Set_modo(NINGUNO);
    }

    if(arg1=="Fade"){
        print.printModo(arg1+":",
                        "En este modo se pasa progresivamente por diferentes tonos del color seleccionado.",
                        QColor(255,50,255));
        habColor(1);
        habBorde(0);
        habBrillo(0);
        mc.Set_colores(datos.getCustom().R,datos.getCustom().G,datos.getCustom().B);
        mc.Set_modo(FADE);
    }
    if(arg1=="Fade Rojo"){
        print.printModo(arg1+":",
                        "En este modo se pasa progresivamente por diferentes tonos de rojo.",
                        QColor(255,0,0));
        habColor(0);
        habBorde(0);
        habBrillo(0);
        mc.Set_colores(255,0,0);
        mc.Set_modo(FADE);

    }
    if(arg1=="Fade Verde"){
        print.printModo(arg1+":",
                        "En este modo se pasa progresivamente por diferentes tonos de verde.",
                        QColor(0,255,0));
        habColor(0);
        habBorde(0);
        habBrillo(0);
        mc.Set_colores(0,255,0);
        mc.Set_modo(FADE);

    }

    if(arg1=="Fade Azul"){
        print.printModo(arg1+":",
                        "En este modo se pasa progresivamente por diferentes tonos de azul.",
                        QColor(0,0,255));
        habColor(0);
        habBorde(0);
        habBrillo(0);
        mc.Set_colores(0,0,255);
        mc.Set_modo(FADE);

    }

    if(arg1=="Pantalla") { // falta desarrollar
        print.printModo(arg1+":",
                        "En este modo las luces van a cambiar en función de tu pantalla.\n"
                        "Podés elegir el ancho del borde de lectura.\n"
                        "Además podés variar el brillo como más te guste!",
                        QColor(200,135,25));
        habColor(0);
        habBorde(1);
        habBrillo(1);
        mc.Set_modo(PANTALLA);
    }
    if(arg1=="Color Fijo") {
        print.printModo(arg1+":",
                        "En este modo las luces van a quedar fijas en el color que vos quieras.\n"
                        "Podés elegir el color que más te guste y variar el brillo como desees!",
                        QColor(15,140,100));

        habColor(1);
        habBorde(0);
        habBrillo(1);
        mc.Set_brillo(ui->Brillo_SLI->value());
        mc.Set_colores(datos.getCustom().R,datos.getCustom().G,datos.getCustom().B);
        mc.Set_modo(FIJO);
    }

    if(arg1=="Punchi") { // falta desarrollar
        print.printModo(arg1+":",
                        "En este modo se van a encender múltiples luces de colores\n"
                        "Ideal para tu fiesta."
                        "Podes variar el brillo a gusto",
                        QColor(255,50,255));
        habColor(0);
        habBorde(0);
        habBrillo(1);
        mc.Set_modo(PUNCHI);
    }
    if(arg1=="Day Light") { // falta desarrollar
        print.printModo(arg1+":",
                        "En este modo las luces van a ir variando segun el horario del dia\n"
                        "Toda la luz del dia en tu cuarto",
                        QColor(255,50,255));
        habColor(1);
        habBorde(0);
        habBrillo(0);
        mc.Set_modo(DAY_lIGHT);
    }
    if(arg1=="Sonido") {
        print.printModo(arg1+":",
                        "En este modo el color de las luces varía con la intecidad del sonido.\n"
                        "Podés variar el brillo como más te guste",
                        QColor(100,15,150));
        habColor(1);
        habBorde(0);
        habBrillo(1);
        mc.Set_modo(AUDIO);
        mc.Set_colores(datos.getCustom().R,datos.getCustom().G,datos.getCustom().B);
    }
    if(arg1=="Rainbow") {
        print.printModo(arg1+":",
                        "En este modo las luces van a ir cambiando su color en forma progresiva.\n",
                        QColor(90,120,250));

        habColor(0);
        habBorde(0);
        habBrillo(1);
        mc.Set_brillo(ui->Brillo_SLI->value());
        mc.Set_modo(RAINBOW);
    }
    if(arg1=="Movi") {
        print.printModo(arg1+":",
                        "En este modo las luces van a ir moviendose con su color en forma progresiva.\n",
                        QColor(90,120,250));


        mc.Set_colores(datos.getCustom().R,datos.getCustom().G,datos.getCustom().B);
        mc.Set_brillo(ui->Brillo_SLI->value());
        mc.Set_modo(MOVI);

        habColor(1);
        habBorde(0);
        habBrillo(1);
    }

    Custom aux = datos.getCustom();
    aux.modo = ui->Modo_CB->currentIndex();
    datos.setCustom(aux);
}

//--------------------------------------------Borde

/*
 * habilitacion del slider para la seleccion del borde
*/
void MainWindow::habBorde(bool a) {
    if(a) {
        ui->Borde_Ancho->setEnabled(1);
        ui->Borde_Titulo->setStyleSheet("font: bold;color: rgb(0,0,0);");
    } else {
        ui->Borde_Ancho->setDisabled(1);
        ui->Borde_Titulo->setStyleSheet("font: bold;color: rgb(127,127,127);");
    }
}

/*
 * lectura del slider del borde
*/
void MainWindow::on_Borde_Ancho_valueChanged(int value) {
    QString str = QString::number(value,10);
    Custom aux = datos.getCustom();
    aux.ancho_borde = value;
    datos.setCustom(aux);
    ui->Borde_Ancho_Aux->setText(str+"%");
    mc.Set_borde(value);
}


//--------------------------------------------Color
void MainWindow::habColor(bool a) {
    if (a) {
        ui->Color_R_SLI->setEnabled(1);
        ui->Color_G_SLI->setEnabled(1);
        ui->Color_B_SLI->setEnabled(1);
        ui->Color_titulo->setStyleSheet("font: bold;color: rgb(0,0,0);");
        ui->Color_R_titulo->setStyleSheet("font: bold;qproperty-alignment: AlignCenter;background-color: rgb(244,0,0);border-radius: 5;color: rgb(255,255,255);");
        ui->Color_G_titulo->setStyleSheet("font: bold;qproperty-alignment: AlignCenter;background-color: rgb(0,244,0);border-radius: 5;color: rgb(255,255,255);");
        ui->Color_B_titulo->setStyleSheet("font: bold;qproperty-alignment: AlignCenter;background-color: rgb(0,0,244);border-radius: 5;color: rgb(255,255,255);");
        print.printBurbujaColor( datos.getCustom() );
    } else {
        ui->Color_R_SLI->setDisabled(1);
        ui->Color_G_SLI->setDisabled(1);
        ui->Color_B_SLI->setDisabled(1);
        ui->Color_titulo->setStyleSheet("font: bold;color: rgb(127,127,127);");
        ui->Color_R_titulo->setStyleSheet("font: bold;qproperty-alignment: AlignCenter;background-color: rgb(127,127,127);border-radius: 5;color: rgb(255,255,255);");
        ui->Color_G_titulo->setStyleSheet("font: bold;qproperty-alignment: AlignCenter;background-color: rgb(127,127,127);border-radius: 5;color: rgb(255,255,255);");
        ui->Color_B_titulo->setStyleSheet("font: bold;qproperty-alignment: AlignCenter;background-color: rgb(127,127,127);border-radius: 5;color: rgb(255,255,255);");
        ui->Color_LB->setStyleSheet("border-radius: 10;background-color: rgb(127,127,127);");
    }
}

void MainWindow::on_Color_R_SLI_valueChanged(int value) {
    Custom aux = datos.getCustom();
    aux.R = value;
    datos.setCustom(aux);
    ui->Color_R_Aux->setText( QString::number(value,10) );
    print.printBurbujaColor( datos.getCustom() );

    mc.Set_color_rojo(value);
}

void MainWindow::on_Color_G_SLI_valueChanged(int value) {
    Custom aux = datos.getCustom();
    aux.G = value;
    datos.setCustom(aux);
    ui->Color_G_Aux->setText( QString::number(value,10) );
    print.printBurbujaColor( datos.getCustom() );

    mc.Set_color_verde(value);

}

void MainWindow::on_Color_B_SLI_valueChanged(int value) {
    Custom aux = datos.getCustom();
    aux.B = value;
    datos.setCustom(aux);
    ui->Color_B_Aux->setText( QString::number(value,10) );
    print.printBurbujaColor( datos.getCustom() );

    mc.Set_color_azul(value);
}

//--------------------------------------------Brillo
void MainWindow::habBrillo(bool a) {
    if(a) {
        ui->Brillo_CHB->setEnabled(1);
        if(!ui->Brillo_CHB->isChecked()) // No habilito si esta en modo sonido
            ui->Brillo_SLI->setEnabled(1);
        if(mc.Get_modo()==AUDIO || mc.Get_modo()==MOVI){ // Para el modo AUDIO no se habilita la variacion con sonido
            ui->Brillo_CHB->setEnabled(0);
            ui->Brillo_CHB->setChecked(false);
            mc.Set_brillo(ui->Brillo_SLI->value());
         }
        ui->Brillo_titulo->setStyleSheet("font: bold;color: rgb(0,0,0);");
    } else {
        ui->Brillo_SLI->setDisabled(1);
        ui->Brillo_CHB->setDisabled(1);
        ui->Brillo_titulo->setStyleSheet("font: bold;color: rgb(127,127,127);");
    }
}

void MainWindow::on_Brillo_SLI_valueChanged(int value) {
    QString str = QString::number(value,10);
    Custom aux = datos.getCustom();
    aux.brillo = value;
    datos.setCustom(aux);
    ui->Brillo_Aux->setText(str+"%");
    mc.Set_brillo(value);
}

void MainWindow::on_Brillo_CHB_stateChanged(int arg1) {
    if(arg1){
        ui->Brillo_SLI->setDisabled(true);
        ui->SONIDO_CB->setDisabled(0);
        ui->Brillo_SLI->setDisabled(1);
        mc.Set_brillo_volumen(true);
        initializeAudio(QAudioDeviceInfo::defaultInputDevice());
    }
    else{
        ui->Brillo_SLI->setDisabled(false);
        ui->SONIDO_CB->setDisabled(1);
        ui->Brillo_SLI->setDisabled(0);
        mc.Set_brillo_volumen(false);
        m_audioInput->disconnect(this); // Dejo de leer el audio
        mc.Set_brillo(ui->Brillo_SLI->value());
    }

    Custom aux = datos.getCustom();
    aux.flag_brillo = arg1;
    datos.setCustom(aux);
}

//--------------------------------------------Configuracion
void MainWindow::on_Red_Ayuda_clicked() {
    AyudaRed ventana;
    ventana.setModal(true);
    ventana.exec();
}

void MainWindow::on_Dir_Cambiar_clicked() {
    QString dir = QFileDialog::getExistingDirectory(this,tr("Elegir Directorio"),"..");
    if(!dir.isEmpty()) {
        ui->Dir_textEdit->setStyleSheet(nullptr);
        datos.setRuta(dir);
        ui->Dir_textEdit->setText( dir );
    }
}

void MainWindow::on_Conf_PB_Abrir_clicked() {
    QString dir = ui->Dir_textEdit->toPlainText();
    if( !existeDir(dir) ) {
        dir = "";
        ui->Dir_textEdit->setStyleSheet("color: rgb(244,0,0);font: bold;");
        ui->Dir_textEdit->setText("No configurado!");
    }

    QString file = QFileDialog::getOpenFileName(this,tr("Abrir archivo"),dir,"Todos (*.*);;CustomLights (*.cl)");

    if(file.isEmpty())
        return;

    datos.leerCustom(file);
    setPersonalizacion();
}

void MainWindow::on_Conf_PB_Cerrar_clicked() {
    bool f_cargar = false;

    //Verifico si existe ruta, si no existia le pongo la del programa
    QString dir = ui->Dir_textEdit->toPlainText();
    if( !existeDir(dir) ) {
        ui->Dir_textEdit->setStyleSheet("color: rgb(244,0,0);font: bold;");
        ui->Dir_textEdit->setText("No configurado!");
        f_cargar = true;
        dir = "";
    } else
        dir+="/";
    dir+="CustomLights.cl";
    QString file = QFileDialog::getSaveFileName(this,
                                                tr("Guardar archivo"),
                                                dir,
                                                "CustomLights (*.cl)");
    if(f_cargar) {
        int size = file.size();
        char *dir_aux = new char[size];
        for(int i=0;i<size;i++) {
            dir_aux[i] = 0;
        }
        strncpy( dir_aux , file.toLatin1().data() , size );
        for(int i=1;i<size;i++) {
            if( file.at(size-i) == '/') {
                dir_aux[size-i] = 0;
                break;
            }
        }
        ui->Dir_textEdit->setStyleSheet("");
        ui->Dir_textEdit->setText( dir_aux );
        datos.setRuta(dir_aux);
    }

    datos.escribirConfig();
    datos.escribirCustom(file);

}

bool MainWindow::existeDir(QString dir) {
    QDir aux;
    return aux.exists(dir);
}

void MainWindow::on_Test_PB_clicked() {
    Test aux(&mc);
    timerData->stop();
    aux.setModal(true);
    aux.exec();
    timerData->start(TIME_D);
    cargarConfig();
}

void MainWindow::on_HW_Config_clicked() {
    datos.escribirConfig();
    ConfigHw aux;
    aux.setModal(true);
    aux.exec();
    cargarConfig();
     // aca estoy configurando la comunicacion que se eligio !!! hay que agregar el wifi
    mc.Set_Com(datos.getComInfo());

}

// -------------------------------------Funciones de sonido


/*
 * como se requieren hacer connects para el sonido (saber cuando se termino de hacer un muestreo) es necesario realizar dicha accion en el mainwindow
*/
void MainWindow::on_SONIDO_CB_currentIndexChanged(int index)
{
    if(inicializacion_audio_ok==1){
        m_audioInfo->stop();
        m_audioInput->stop();
    }
    m_audioInput->disconnect(this);
    initializeAudio(ui->SONIDO_CB->itemData(index).value<QAudioDeviceInfo>());

}

/*
 * inicializacion del audio
*/
void MainWindow::inicializacion_audio(){

    inicializacion_audio_ok =0;
    QList<QAudioDeviceInfo> lista;
    QAudioDeviceInfo audio_info;

    lista = QAudioDeviceInfo::availableDevices(QAudio::AudioInput);
    while(!lista.isEmpty()){ // imprimo todos los dispositivos de audio disponibles
        audio_info = lista.takeFirst();
        ui->SONIDO_CB->addItem(audio_info.deviceName(),QVariant::fromValue(audio_info));
    }

    if(ui->SONIDO_CB->findText(datos.getAudioInput())!=-1){ // establezco el que tenemos seleccionado por defecto
        ui->SONIDO_CB->setCurrentText(datos.getAudioInput());
        initializeAudio(ui->SONIDO_CB->itemData(ui->SONIDO_CB->currentIndex()).value<QAudioDeviceInfo>());
    }
    else
        initializeAudio(QAudioDeviceInfo::defaultInputDevice()); // si no habia ninguno seleccionado inicializo el que esta por defecto

    inicializacion_audio_ok = 1; // flag de que se configuro la inicializacion del audio

}

void MainWindow::initializeAudio(const QAudioDeviceInfo &deviceInfo) // configuro el audio de entrada
{
    QAudioFormat format;
    format.setSampleRate(44100); // muestreo del auido
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setSampleType(QAudioFormat::SignedInt);
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setCodec("audio/pcm");

    m_audioInfo.reset(new AudioInfo(format));
    connect(m_audioInfo.data(), &AudioInfo::update,[this](){
        mc.actualizar_volumen(m_audioInfo->Get_maxAudio(),m_audioInfo->Get_levelAudio());
    }); // conecto cuando hay un paquete de audio disponible con la funcion que trata estos paquetes
    m_audioInput.reset(new QAudioInput(deviceInfo, format));
    m_audioInfo->start();
    m_audioInput->stop();
    m_audioInput->start(m_audioInfo.data());
}

// -----------------------------Timmers

void MainWindow::TimerData1ms(){ // timer que envia y actualiza los paquetes
    mc.actualizar_luces();
}

