#include "comunicacion.h"

using namespace std;



Comunicacion::Comunicacion(WiFiUDP * socket)//:strip(110, 3)
{
  //udp socket
  udp_socket = socket;
  port = 10800;

  // variable global a la clase
  configurado = 0;

  //flags
  s_updIpget = 0;
  s_udpAck = 0;
  s_udpCmd01 = 0;
  f_recibialgo = 1;
  
}

int Comunicacion::obtenerComando(unsigned char * cmd, Comando * out) // obtengo un struct Comando de un string UDP
{
    char s[3] = "<>";

    char *token = strtok((char*)cmd, s);

    int i = 0;

    while(token != NULL)
    {
        switch(i)
        {
            case 0:
                strcpy(out->direccion, token);
                break;
            case 1:
                strcpy((char*)out->argumento, token);
                break;
            case 2:
                if(strcmp(out->direccion, token+1) == 0)
                    return 1;
                else
                    return 0;

        }

        token = strtok(NULL, s);
        i++;
    }

    return 0;
}


void Comunicacion::maquina_configuracion()
{
  static uint8_t estado = RESET;
  
  switch(estado)
  {
    case RESET:
      configurado = 0;
      estado = ESPERANDO;
      break;


    case ESPERANDO:
      if(s_updIpget)
      {
        s_updIpget = 0;
        pc_ip = udp_socket->remoteIP();
        m_transmitirIp();
        estado = ESPERO_ACK_PC;
      }
      break;

    case ESPERO_ACK_PC:
      if(s_udpAck)
      {
        estado = CONFIGURACION_LISTA;
        s_udpCmd01 =0;
		configurado = 1;
      }
      else if(s_updIpget)
      {
        s_updIpget = 0;
        m_transmitirIp();
        estado = ESPERO_ACK_PC;
      }
      break;

    case CONFIGURACION_LISTA:
      if(s_udpCmd01 && f_recibialgo)
      {
        f_recibialgo =0;
        s_udpCmd01 = 0;
        configurado = 0;
        estado = ESPERANDO;
      }
	  else if(s_udpData)
	  {
		s_udpData = 0;
    f_recibialgo=1;
		m_enviarTramaLpc();
	  }
      break;

    default:
      configurado = 0;
      estado = RESET;
  }
}

void Comunicacion::ejecutarComando(Comando c)
{
  if(c.direccion[0] != 0)
  {
    m_eventoTrama(c); // ejecuto el comando solo si es un comando valido
    //Serial.println("Comando valido");
  }
}

int Comunicacion::hex_to_int(const char * hex)
{
  return strtol(hex, NULL, 16);
}

void Comunicacion::m_eventoTrama(const Comando c)
{
  if(!strcmp(c.direccion,"ipget"))
  {
    s_updIpget = 1;
  }
  else if(!strcmp(c.direccion,"ack"))
  {
	s_udpAck = 1;
  }
  else if(!strcmp(c.direccion,"cmd"))
  {
    switch(hex_to_int((char*)c.argumento))
    {
      case 0:
        s_udpCmd00 = 1;
        break;
      case 1:
        s_udpCmd01 = 1;
        break;
    }
  }
  else if(!strcmp(c.direccion,"data"))
  {
  	if(s_udpData == 0)
  	{
		int aux = strlen((char*)payload); // copio el argumento del comando a memoria
		strncpy((char*)payload,(char*)c.argumento,500);
		
		for(int i = 2;i< aux;i++) // multiplico * 2 todos los valores, ya que solo llegan valores entre 0 y 127
		{
			payload[i]  *= 2; 
       	}
     
  		s_udpData = 1;
  	}
  }
}

void Comunicacion::m_transmitirIp()
{
  udp_socket->beginPacket(pc_ip, port);
  udp_socket->write("<ip>");
  udp_socket->print(local_ip);
  udp_socket->write("</ip>");
  udp_socket->endPacket();
}

void Comunicacion::m_transmitoAck()
{
  udp_socket->beginPacket(pc_ip, port);
  udp_socket->write("<ack>");
  udp_socket->write("00");
  udp_socket->write("</ack>");
  udp_socket->endPacket();
}

void Comunicacion::m_transmitoAckError(char * error_code)
{
  udp_socket->beginPacket(pc_ip, port);
  udp_socket->write("<ack>");
  udp_socket->write(error_code);
  udp_socket->write("</ack>");
  udp_socket->endPacket();
}

void Comunicacion::m_enviarTramaLpc()
{

  payload[strlen((char* )payload)-1] = '#'; // agrego el numeral que al multiplicar por 2 y ser impar se perdio
  Serial.print((char*)payload);
  
  s_udpData = 0;
}
