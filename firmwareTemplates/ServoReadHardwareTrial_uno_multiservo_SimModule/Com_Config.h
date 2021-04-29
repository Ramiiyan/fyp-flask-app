#define TINY_GSM_MODEM_SIM7000
#include <Arduino.h>
#include <MQTT.h>
#include <TinyGsmClient.h>
#include <SoftwareSerial.h> //apply for Arduino UNO
#include "ServoMotor.h"

#define PUB_TOPIC           {{mqtt_setting.pub_topic}}      // "tester2/tesingdev2/v3/common"
#define SUB_TOPIC           {{mqtt_setting.sub_topic}}      // "sub/1119/tester2/tesingdev2/v3/pub"

#define MQTT_USERNAME       {{mqtt_setting.mqtt_username}}  // "tester2-tesingdev2-v3_3220"
#define MQTT_PASSWORD       {{mqtt_setting.mqtt_password}}  // "1572435365_3220"
#define MQTT_HOST           {{mqtt_setting.mqtt_host}}      // "mqtt.iot.ideamart.io"
#define MQTT_PORT           {{mqtt_setting.mqtt_port}}      // 1883

#define BAUD_RATE           {{serial_setting.baud_rate}}          // 115200
#define SIM_BAUD_RATE       {{serial_setting.sim_module}}         // 9600
//#define TXD                 {{serial_setting.sim_module_Txd}}     // 16    //TXD
//#define RXD                 {{serial_setting.sim_module_Rxd}}     // 17    //RXD

#define GSM_APN             "dialogbb"
#define NB_APN              "nbiot"
#define NETWORK_MODE        13

/* Only applicable for Aruduino Uno */
//#define ideaBoard_PWRKEY    13
#define ideaBoard_RX        8
#define ideaBoard_TX        7
//#define ideaBoard_RST       11

SoftwareSerial SerialSIM(ideaBoard_TX, ideaBoard_RX); // Only for SoftwareSerial
//HardwareSerial SerialSIM(2);    // Only For ESP, NodeMcu
TinyGsm modem(SerialSIM);
TinyGsmClient client(modem);
MQTTClient mqtt;

char clientId = "";
String getmsg;
char setmsg[150];

String control_mode = "{\"action\":\"control\",\"param\":{\"mac\":\"1119\"}}";
String monitor_mode = "{\"action\":\"monitor\",\"param\":{\"mac\":\"1119\"}}";
String cpsi;
byte action = 0;

boolean ConnectToMQTT();
void sendRAPosition(int b, int j1, int j2, int j3, int j4, int fe);
//void CallBack(char *t, byte *payload,unsigned int l);
void messageReceived(String &topic, String &payload);
String getCPSI();

//byte action_value(byte action_val); // publish mode = 0 (default) , subcribe mode = 1 return 0/1

boolean ConnectToMQTT(){
  randomSeed(analogRead(39)); //analog pin 5 used to genarate random number
  clientId = "TEST-" + char(millis()) + char(random(0, 100000000), HEX);
    Serial.println(clientId);
  if (!mqtt.connect(clientId, MQTT_USERNAME, MQTT_PASSWORD)){
    Serial.println(F("MQTT fail"));
    return false;
  }
  Serial.println(SUB_TOPIC);
  mqtt.subscribe(SUB_TOPIC);
  return mqtt.connected();
}

//byte action(byte action_val){
//  return 0; // tramsit_mode by default.  
//}

//void CallBack(char *t, byte *payload, unsigned int l){
//  Serial.println(F("******************"));
//  for (unsigned int i = 0; i < l; i++){
//    getmsg[i] = (char)payload[i];
//  }
//  if (receive_mode == getmsg){
//    action = 1;
//    Serial.println(F("Receive_Mode Activated"));
//    Serial.println(getmsg);
//
//  }else if (transmit_mode == getmsg){
//    action = 0;
//    Serial.println(F("Transmit_Mode Activated"));
//  }
//  delay(500);
//  memset(getmsg, 0, sizeof(getmsg));
//}
void messageReceived(String &topic, String &payload) {
//  Serial.println("incoming: " + topic + " - " + payload);
  
  if (control_mode == payload){
    action = 1;
    Serial.println(F("Control_Mode Activated"));
    
  }else if (monitor_mode == payload){
    action = 0;
    Serial.println(F("Monitor_Mode Activated"));
    
  }else{
    getmsg = payload;
//    Serial.print("header :");
//    Serial.println(getmsg);
  }
}
  

void sendRAPosition(int b, int j1, int j2, int j3, int j4, int fe){
//  {"eventName":"Rovo","status":"<none>","b":"<b>","j1":"<j1>","j2":"<j2>","j3":"<j3>","j4":"<j4>","fe":"<fe>","param":{"mac":"<macAddressOfDevice>"}}
//  sprintf(setmsg,"{\"eventName\":\"Rovo\",\"b\":\"%d\",\"j1\":\"%d\",\"j2\":\"%d\",\"j3\":\"%d\",\"j4\":\"%d\",\"fe\":\"%d\",\"param\":{\"mac\":\"1119\"}}",10,11,12,13,14,15);
  sprintf(setmsg,"{\"b\":\"%d\",\"j1\":\"%d\",\"j2\":\"%d\",\"j3\":\"%d\",\"j4\":\"%d\",\"fe\":\"%d\",\"mac\":\"1119\"}",b,j1,j2,j3,j4,fe);
  if (!mqtt.publish(PUB_TOPIC, setmsg)){
    Serial.println(F("Failed."));
  }else{
    Serial.print(setmsg);
    Serial.println(F("SEND:OK"));
  }
  //delay(1000);
}

String getCPSI(){
  SerialSIM.println("AT+CPSI?");
  delay(500);
  while (SerialSIM.available()){
    cpsi = SerialSIM.readString();
  }
  delay(500);
  return cpsi;
}
