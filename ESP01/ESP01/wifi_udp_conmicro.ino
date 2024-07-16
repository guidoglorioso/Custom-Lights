#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include "comunicacion.h"

const char* ssid = "CustomLights"; // datos para la red provisoria que se crea en caso de no tener guardada una red valida
const char* psw = "1881941220";

unsigned int localPort = 10800;      // puerto UDP definido para la comunicaicon

unsigned char packetBuffer[1024]; // buffer para la entrada de paquetes UDP

WiFiUDP Udp; // instancia de la clase UDP
Comunicacion udp_pc(&Udp); // instancia de la clase Comunicacion

Comando cmd; // variable auxiliar

void setup()
{
  // abro el puerto serie a 300k de baudrate
  Serial.begin(300000);

  WiFiManager wifiManager;
  wifiManager.setSTAStaticIPConfig(IPAddress(192,168,0,42), IPAddress(192,168,0,1), IPAddress(255,255,255,0)); // configuro ip fija (en algunos routers es necesario)
  
  while(!wifiManager.autoConnect(ssid, psw)) // me intento conectar a la red previa guardada o creo una red provisoria
  {
    delay(1000);  
  }

  WiFi.hostname("CustomLights"); // nombre del dispositivo

  Udp.begin(localPort); // abro el puerto udp

  udp_pc.setLocalIp(WiFi.localIP()); // asigno mi ip a la clase comunicacion

}

void loop()
{
  int total_bytes = Udp.parsePacket();
 
  if(total_bytes) 
  {
    
    // Si total_bytes != 0 significa que recibi un paquete
    Udp.read(packetBuffer,total_bytes); // leo el paquete y lo dejo en el buffer

    packetBuffer[total_bytes] = 0; // agrego un \0 al final
    
    cmd.direccion[0] = 0; // seteo el comando provisorio
    cmd.argumento[0] = 0;
	
    if(Comunicacion::obtenerComando(packetBuffer, &cmd)) // obtengo el comando del string
    {
      udp_pc.ejecutarComando(cmd); // ejecuto el comando
    }
  }

  udp_pc.maquina_configuracion(); // maquina de estados de configuracion
}
