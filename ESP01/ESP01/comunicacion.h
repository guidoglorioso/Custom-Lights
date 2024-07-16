#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <cstring>

enum estados{RESET, ESPERANDO, ESPERO_ACK_PC, CONFIGURACION_LISTA, TRAMA_RECIBIDA};

struct Comando
{
    char direccion[8];
    unsigned char argumento[500];
};

class Comunicacion
{
private:
  WiFiUDP * udp_socket;
  IPAddress local_ip;
  IPAddress pc_ip;
  uint16_t port;

  uint8_t configurado;
  uint8_t f_recibialgo;
  int hex_to_int(const char * hex);

  // todo se inicializa a 0
  uint8_t s_updIpget, s_udpAck, s_udpData, s_udpCmd00, s_udpCmd01; // flags
  unsigned char payload[500];

  void m_transmitirIp();
  void m_transmitoAck();
  void m_transmitoAckError(char * error_code);

  void m_eventoTrama(const Comando);
  void m_enviarTramaLpc();

public:
  Comunicacion(WiFiUDP *);

  inline void setLocalIp(IPAddress ip) { local_ip = ip; };

  inline uint8_t getConfiguracion() { return configurado; };
  inline IPAddress getPcIp() { return pc_ip; };
  
  /*
  Funcion que pasa de string a struct Comando:
  
  <cmd>01</cmd> -> Comando c = {"cmd", 1]}
  
  Si la trama es invalida devuelve 0, sino 1
  
  */
  static int obtenerComando(unsigned char * cmd, Comando * out);
	
   /*
	Maquina de estados de configuracion del ESP
   */
  void maquina_configuracion();
  
  /*
  Funcion que ejecuta el comando c. Segun sea el comando, la funcionalidad sera diferente.
  */
  void ejecutarComando(Comando c);
};
