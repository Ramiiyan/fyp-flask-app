#define TINY_GSM_MODEM_SIM7000
#include <Arduino.h>
#include <PubSubClient.h>
#include <TinyGsmClient.h>

#define GSM_APN             "dialogbb"
#define NB_APN              "nbiot"

#define PUB_TOPIC           "tester2/tesingdev2/v3/common" 
#define SUB_TOPIC           "sub/1119/tester2/tesingdev2/v3/pub"

#define MQTT_USERNAME       "tester2-tesingdev2-v3_3220"
#define MQTT_PASSWORD       "1572435365_3220"
#define MQTT_HOST           "mqtt.iot.ideamart.io"
#define MQTT_PORT           1883

//#define ideaBoard_PWRKEY    13
//#define ideaBoard_RX        8
//#define ideaBoard_TX        7
//#define ideaBoard_RST       11

HardwareSerial SerialSIM(2);
TinyGsm modem(SerialSIM);
TinyGsmClient client(modem);
PubSubClient mqtt(client);

String clientId = "";
char getmsg[100];
char setmsg[100];
//{
//  "action": "receive",
//  "param": {
//    "mac": "1119"
//  }
//}
String receive_mode = "{\"action\":\"receive\",\"param\":{\"mac\":\"1119\"}}";
String transmit_mode = "{\"action\":\"transmit\",\"param\":{\"mac\":\"1119\"}}";
String cpsi;
boolean ConnectToMQTT();
void sendRAPosition(double sp1, double sp2, double sp3);
void CallBack(char *t, byte *payload,unsigned int l);
String getCPSI();
byte action = 0;
//byte action_value(byte action_val); // publish mode = 0 (default) , subcribe mode = 1 return 0/1

boolean ConnectToMQTT(){
  randomSeed(analogRead(5)); //analog pin 5 used to genarate random number
  clientId = "TEST-" + String(millis()) + String(random(0, 100000000), HEX);
    Serial.println(clientId);
  if (!mqtt.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)){
    Serial.println("MQTT fail");
    return false;
  }
  Serial.println(SUB_TOPIC);
  mqtt.subscribe(SUB_TOPIC);
  return mqtt.connected();
}

//byte action(byte action_val){
//  return 0; // tramsit_mode by default.  
//}

void CallBack(char *t, byte *payload, unsigned int l){
  Serial.println(F("******************"));
  for (unsigned int i = 0; i < l; i++){
    getmsg[i] = (char)payload[i];
  }
  if (receive_mode == getmsg){
    action = 1;
//    action_value(){ // for subscribe.
//     return 1;  
//    };  
    Serial.println(F("Receive_Mode Activated"));

  }else if (transmit_mode == getmsg){
    action = 0;
    Serial.println(F("Transmit_Mode Activated"));
//    byte action_value(){ // publish parameters.
//     return 0;  
//    }; 
  }
  delay(1000);
  memset(getmsg, 0, sizeof(getmsg));
}
  

void sendRAPosition(int sp1, int sp2, int sp3){
  // {"eventName":"Test12","status":"<none>","a":"20.0","b":"60.0","c":"40.0","param":{"mac":"1119"}}
  sprintf(setmsg,"{\"eventname\":\"Test12\",\"status\":\"none\",\"a\":\"%d\",\"b\":\"%d\",\"c\":\"%d\",\"param\":{\"mac\":\"1119\"}}",sp1,sp2,sp3);
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
