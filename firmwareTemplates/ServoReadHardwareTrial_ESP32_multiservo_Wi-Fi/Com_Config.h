#include <Arduino.h>
#include <WiFi.h>
#include <MQTT.h>

#include "ServoMotor.h"

#define PUB_TOPIC           {{mqtt_setting.pub_topic}}      // "tester2/tesingdev2/v3/common"
#define SUB_TOPIC           {{mqtt_setting.sub_topic}}      // "sub/1119/tester2/tesingdev2/v3/pub"

#define MQTT_USERNAME       {{mqtt_setting.mqtt_username}}  // "tester2-tesingdev2-v3_3220"
#define MQTT_PASSWORD       {{mqtt_setting.mqtt_password}}  // "1572435365_3220"
#define MQTT_HOST           {{mqtt_setting.mqtt_host}}      // "mqtt.iot.ideamart.io"
#define MQTT_PORT           {{mqtt_setting.mqtt_port}}      // 1883

#define BAUD_RATE           {{serial_setting.baud_rate}}          // 115200

#define WIFI_SSID           {{wifi_setting.username}}    // "SSID"
#define WIFI_PASS           {{wifi_setting.password}}   // "PASSWORD"

WiFiClient espClient;
MQTTClient mqtt;

String clientId = "";
String getmsg;
char setmsg[150];
String control_mode = "{\"action\":\"control\",\"param\":{\"mac\":\"1119\"}}";
String monitor_mode = "{\"action\":\"monitor\",\"param\":{\"mac\":\"1119\"}}";
byte action = 0;

void setup_wifi();
boolean ConnectToMQTT();
void sendRAPosition(int b, int j1, int j2, int j3, int j4, int fe);
void messageReceived(String &topic, String &payload);


void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.println(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
}
boolean ConnectToMQTT(){
  randomSeed(analogRead(39)); //analog pin 5 used to genarate random number
  clientId = "TEST-" + String(millis()) + String(random(0, 100000000), HEX);
    Serial.println(clientId);
  if (!mqtt.connect(clientId.c_str(), MQTT_USERNAME, MQTT_PASSWORD)){
    Serial.println(F("MQTT fail"));
    return false;
  }
  Serial.println(SUB_TOPIC);
  mqtt.subscribe(SUB_TOPIC);
  return mqtt.connected();
}
void messageReceived(String &topic, String &payload) {
  Serial.println("incoming: " + topic + " - " + payload);
  if (control_mode == payload){
    action = 1;
    Serial.println(F("Control_Mode Activated"));
  }else if (monitor_mode == payload){
    action = 0;
    Serial.println(F("Monitor_Mode Activated"));
  }else{
    getmsg = payload;
  }
}

void sendRAPosition(int b, int j1, int j2, int j3, int j4, int fe){
  sprintf(setmsg,"{\"b\":\"%d\",\"j1\":\"%d\",\"j2\":\"%d\",\"j3\":\"%d\",\"j4\":\"%d\",\"fe\":\"%d\",\"mac\":\"1119\"}",b,j1,j2,j3,j4,fe);
  if (!mqtt.publish(PUB_TOPIC, setmsg)){
    Serial.println(F("Failed."));
  }else{
    Serial.print(setmsg);
    Serial.println(F("SEND:OK"));
  }
}
