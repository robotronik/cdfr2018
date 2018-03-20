#include <SoftwareSerial.h>
#include "robotronik_protocol.h"
#include "remote_call.h"

//Déclaration des pins
byte const pin_pwm = 3;
byte const pin_cmd1 = 2;
byte const pin_cmd2 = 4;
byte const pin_led = 5;

SoftwareSerial led_serial(8, 9);

RP_Interface motor_interface, led_interface;
RC_Server motor_server, led_server;

uint8_t send_motor(uint8_t *data, uint16_t size, uint32_t timeout){
  (void)timeout;
  return (Serial.write(data, size) == size);
}

uint8_t send_led(uint8_t *data, uint16_t size, uint32_t timeout){
  (void)timeout;
  return (led_serial.write(data, size) == size);
}

void set_led(RC_Server *server){
  int state;
  RC_Server_Get_Args(server, &state);
  
  digitalWrite(pin_led, (state==1)?HIGH:LOW);

  RC_Server_Return(server, ((state==1)?"LED ON":"LED OFF"));
}

void commande_moteur(unsigned char pwm, bool reverse){
  //Arrêt libre
  if(pwm <= 50){
    digitalWrite(pin_pwm, LOW);
    return;
  }

  //Commande vitesse
  analogWrite(pin_pwm, min(abs(pwm),255));
  if(!reverse){
    digitalWrite(pin_cmd1, LOW);
    digitalWrite(pin_cmd2, HIGH);
  }
  else{
    digitalWrite(pin_cmd1, HIGH);
    digitalWrite(pin_cmd2, LOW);
  }
}

void set_motor(RC_Server *server){
  int pwm;
  RC_Server_Get_Args(server, &pwm);

  if(pwm < 0){
      pwm = map(pwm, 0, -32768, 0, 255);
      commande_moteur(pwm, false);
  }else{
    pwm = map(pwm, 0, 32767, 0, 255);
      commande_moteur(pwm, true);
  } 

  RC_Server_Return(server, pwm);
}

void RP_Packet_Received(RP_Interface *iface, RP_Packet *packet){
  if(iface == &motor_interface){
    RC_Server_Get_Request(&motor_server, packet);
  }else if(iface == &led_interface){
    RC_Server_Get_Request(&led_server, packet);
  }
}

void setup(){
	//pinModes
	pinMode(pin_pwm, OUTPUT);
	pinMode(pin_cmd1, OUTPUT);
	pinMode(pin_cmd2, OUTPUT);
  pinMode(pin_led, OUTPUT);

	//Communication série
	Serial.begin(9600);
  led_serial.begin(9600);

  //Interfaces
  RP_Init_Interface(&motor_interface, send_motor, millis);
  RP_Init_Interface(&led_interface, send_led, millis);

  //Serveurs
  RC_Server_Init(&motor_server, &motor_interface);
  RC_Server_Init(&led_server, &led_interface);

  RC_Server_Add_Function(&motor_server, 0, set_motor, "i", "i", RC_IMMEDIATE);
  RC_Server_Add_Function(&led_server, 0, set_led, "i", "s", RC_IMMEDIATE);
}



void loop(){
  if(Serial.available() > 0){
    byte received = Serial.read();
    RP_Process_Data(&motor_interface, &received, 1);
  }
  if(led_serial.available() > 0){
    byte received = led_serial.read();
    RP_Process_Data(&led_interface, &received, 1);
  }
}
