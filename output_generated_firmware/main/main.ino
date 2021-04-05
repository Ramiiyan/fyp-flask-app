#include "ServoMotor.h" // custom library header for Servo motors
#include "GSM_Setting.h" // custom library header for GSM_Module.
#include "mainTemplate.h"

/*ServoMotor  *myServo1, *myServo2; */
ServoMotor *servo_list[SERVO_COUNT];
boolean hasInitControl = false;
boolean hasInitMonitor = false;
int servoPos[SERVO_COUNT]; //using in publish positions (monitor)

int getPos[SERVO_COUNT]; //using in control
char *posName[] ={"b","j1","j2","fe","j3","j4"};
String recentCmsg = "hello";
void setup() {
  Serial.begin(BAUD_RATE);
  delay(10);
  Serial.println("setup begin...");
  
  SerialSIM.begin(SIM_BAUD_RATE, SERIAL_8N1, (int8_t) RXD, (int8_t) TXD);
  modem.setNetworkMode(NETWORK_MODE); // 38-nbiot 13-gsm
//  modem.setPreferredMode(2);  // only for nbiot
  modem.getModemName();
  Serial.println(modem.getModemInfo());
  Serial.println(modem.getIMEI());
  modem.restart();
  delay(2000);
  Serial.println(F("Finding Network..."));
  if (!modem.waitForNetwork()) {
    Serial.println(F("Network Fail"));
    while (true);
  }else {
    Serial.println(F("Network identified."));
    Serial.print(F("Signal Strength : "));
    Serial.println(modem.getSignalQuality());
    
    if (!modem.gprsConnect(GSM_APN, "", "")) {
      Serial.println(F("GPRS Fail"));
    }else {
      Serial.println(F("GPRS Connected."));
      Serial.println(getCPSI());
      mqtt.begin(MQTT_HOST, MQTT_PORT,client);
      mqtt.onMessage(messageReceived);
    }
  }

  /*Servo Intializing */ 
  for(int i=0; i<SERVO_COUNT; i++ ){
    Serial.print(F("Servo Motor - "));
    Serial.println(i+1);
    servo_list[i] = new ServoMotor(servo_DPins[i],servo_APins[i]);
    
//    servo_list[i]->calibrate_Range(0,180);
    servo_list[i]->calibrate_Range(servo_range[i][0],servo_range[i][1]); //[0]- min, [1]- max
    delay(100);
  }
}

void loop() {
  if (mqtt.connected()) {
    mqtt.loop();
  }else{
    Serial.println(F("Connecting MQTT..."));
    ConnectToMQTT();
    delay(100);
  }
  delay(1000);
  
  if(action == 0){  // publish mode (monitor) (transmit current position to Platform)
    hasInitControl = false;
    if(!hasInitMonitor){
      for(int i=0; i<SERVO_COUNT; i++ ){
        Serial.print(F("Servo Motor Monitor mode- "));
        Serial.println(i+1);
        servo_list[i]->switch_To_monitor();
        delay(100);
      }
      hasInitMonitor = true;
    }
    for(int i=0; i<SERVO_COUNT; i++){
      Serial.print("myServo ");
      Serial.print(i+1);
      Serial.print(" Pos : ");
      servoPos[i] = (int)servo_list[i]->cur_Pos();
      Serial.println(servoPos[i]);
    }
    
    if(SERVO_COUNT == 1){
      sendRAPosition(servoPos[0],0,0,0,0,0);
    }else if(SERVO_COUNT == 4){
      sendRAPosition(servoPos[0],servoPos[1],servoPos[2],0,0,servoPos[3]); //b,j1,j2,fe
    }else if (SERVO_COUNT == 6){
      sendRAPosition(servoPos[0],servoPos[1],servoPos[2],servoPos[3],servoPos[4],servoPos[5]); //b,j1,j2,j3,j4,fe
    }
    delay(400);
    
  }
  if (action == 1){  // subscribe mode (control)(receive positions from platform)
    hasInitMonitor = true;
    if(!hasInitControl){
      for(int i=0; i<SERVO_COUNT; i++ ){
        Serial.print(F("Servo Motor Control mode- "));
        Serial.println(i+1);
        servo_list[i]->switch_To_control();
      }
      hasInitControl = true;
    }
    if(getmsg != recentCmsg){
      
      Serial.print(F("Recieved event :"));
      Serial.println(getmsg);
  
      deserializeJson(doc, getmsg);
      JsonObject obj = doc.as<JsonObject>();
      
      for(int i=0; i<SERVO_COUNT; i++ ){
        getPos[i] = obj[String(posName[i])];
        Serial.println(getPos[i]);
        servo_list[i]->control(getPos[i]);
      }
      Serial.println("*************");
      recentCmsg = getmsg;
    }
         
//    if(prePos != curPos){
//    Serial.print(curPos);
      
//    }
  }

  
 
}
