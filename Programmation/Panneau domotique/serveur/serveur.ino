#include <ESP8266WiFi.h>

#define DEBUG 1

//Point d'accès
char name[] = "Robotronik-2A";
char password[] = "robotronik";

//Adresses IP des robots
IPAddress robot_2a(192, 168, 0, 1);
IPAddress robot_1a(192, 168, 0, 2);

//Serveur TCP
WiFiServer server(80);

int score_1a = 0, score_2a = 0;
bool new_score = false;

void setup() {
  Serial.begin(115200);

  //Connexion au point d'accès
  WiFi.begin(name, password);

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
  WiFiClient client = server.available();
  if(client && client.connected() && client.available()){
    String data = client.readStringUntil('\0');
    int score = data.toInt();
    if((score >= 0) && ((score&0xFFFF) == score)){
      new_score = true;
      IPAddress remote = client.remoteIP();
      if(remote == robot_1a){
        score_1a = score;
      }
      else if(remote == robot_2a){
        score_2a = score;
      }
      else{
        new_score = false;
      }
    }
    client.stop();//Aparemment nécessaire pour l'ESP
  }
  if(new_score){
    #if DEBUG==1
    Serial.println(score_1a+score_2a);
    #else
    unsigned int score = (score_1a+score_2a)&0xFFFF;
    unsigned char low_byte = score&0xFF, high_byte = score>>8;
    Serial.write(low_byte);
    Serial.write(high_byte);
    #endif
    new_score = false;
  }
  
  delay(100);  
}
