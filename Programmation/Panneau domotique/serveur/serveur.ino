#include <ESP8266WiFi.h>

#define AP_NAME "Robotronik-2A"
#define AP_PASSWD "robotronik"

#define DEBUG 1

//Serveur TCP
WiFiServer server(80);

int score_1a = 0, score_2a = 0;
int score_total;
bool new_score = false;

void setup() {
  Serial.begin(115200);

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
    //On récupère les données
    String data = client.readStringUntil('\0');
    #if DEBUG==1
    Serial.println(data);
    #endif

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
  if(new_score){
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
    unsigned char low_byte = score_total&0xFF, high_byte = score_total>>8;
    Serial.write(low_byte);
    Serial.write(high_byte);
    #endif
    new_score = false;
  }
  
  delay(100);  
}
