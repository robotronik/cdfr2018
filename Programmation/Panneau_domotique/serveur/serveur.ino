#include <ESP8266WiFi.h>
#include "./rpv1.h"

#define AP_NAME "Robotronik"
#define AP_PASSWD "xyz2la8ca4PfmRff5eda"

#define DEBUG 0
#if DEBUG==1
#warning "DEBUG MODE IS ACTIVE"
#endif

//Serveur TCP
WiFiServer server(80);

//Robotronik Protocol
RP_Interface interface;
RP_Packet packet;

int score_1a = 0, score_2a = 0;
int score_total;
bool new_score = false;
unsigned long last_time = 0;

uint8_t send(uint8_t *data, uint16_t len, uint32_t timeout){
  return (Serial.write(data, len) == len);
}

#if DEBUG == 1
void print_mac(){
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC: ");
  Serial.print(mac[5],HEX); Serial.print(":");
  Serial.print(mac[4],HEX); Serial.print(":");
  Serial.print(mac[3],HEX); Serial.print(":");
  Serial.print(mac[2],HEX); Serial.print(":");
  Serial.print(mac[1],HEX); Serial.print(":");
  Serial.println(mac[0],HEX);
}
#endif

void setup() {
  //Initialisation UART
  Serial.begin(115200);
  
  RP_Init_Interface(&interface, send);
  packet.len = 2;

  //Connexion au point d'accès
  WiFi.begin(AP_NAME, AP_PASSWD);

  //Reconnexion auto en cas de perte de connexion
  WiFi.setAutoReconnect(true);

  //Attente de la connexion
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
  }
  
  #if DEBUG==1
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());
  #endif
 
  //Démarrage du serveur
  server.begin();
}

void loop() {
  //On récupère un éventuel client ayant envoyé un paquet
  WiFiClient client = server.available();
  int id, score;

  //Si ce client existe et est connecté
  if(client && client.connected() && client.available()){
    /*
     * Remarque : si un client a fermé la connexion
     * mais que des données n'ont pas été lues,
     * client.connected() vaut true
     */
    //On récupère le dernier paquet reçu
    String data;
    while(client.available()){
      data = client.readStringUntil('\0');
    }

    //On décompose la chaîne de caractères
    if((data.length() >= 3) && (data[0] == '0')){
      id = data.substring(1,2).toInt();
      score = data.substring(2).toInt();
      
      //Si le score semble correct
      if(score >= 0 && score <= 0xFFFF){
        switch(id){
          case 1:
          score_1a = score;
          new_score = true;
          break;
          case 2:
          score_2a = score;
          new_score = true;
          break;
          default:
          break; 
        }
      }
    }
    client.stop();//Aparemment nécessaire pour l'ESP
  }

  //Envoi du score
  unsigned long current_time = millis();
  if(new_score || (current_time - last_time >= 1000)){
    last_time = current_time;
    score_total = (score_1a+score_2a)&0xFFFF;
    #if DEBUG==1
    switch(id){
      case 1:
      Serial.print("Robot_1A : ");
      break;
      case 2:
      Serial.print("Robot_2A : ");
      break;
      default:
      break;
    }
    Serial.println(String(score)+"; total : "+String(score_total));
    #else
    //Envoi des données
    packet.data[0] = score_total & 0xFF; //octet de poids faible
    packet.data[1] = score_total >> 8; //octet de poids fort
    RP_Sync(&interface, 0); //Resynchronise l'automate de réception du F3, au cas où l'ESP ait envoyé des données de debug sur la liaison série entre temps
    RP_Send(&interface, &packet, 0);
    #endif
    new_score = false;
  }

  delay(100);
}
