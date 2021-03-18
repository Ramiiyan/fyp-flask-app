#include "ServoMotor.h" // custom library header for Servo motors
#include "GSM_Setting.h" // custom library header for GSM_Module.

//define DOF
#define SERVO_COUNT   6

// Wroom ESP32 Servo-motor Control Digital Pins 
const byte servo_DPins[SERVO_COUNT] = {32, 34, 12, 18, 22, 26}; //by Default

// Wroom ESP32 Servo-motor Feedback Analog Pins
const int servo_APins[SERVO_COUNT] = {33, 35, 13, 19, 23, 27};

//initiate Servo Motors
ServoMotor *servo_list[SERVO_COUNT];

void setup() {
  
  Serial.begin(115200);
  delay(10);
  Serial.println("setup begin...");

/*  Sim Configuration */
  SerialSIM.begin(9600, SERIAL_8N1, (int8_t) 17, (int8_t) 16); //  Myserial.begin(9600, SERIAL_8N1, RXD, TXD);
  modem.setNetworkMode(13);         // 38-nbiot 13-gsm
//  modem.setPreferredMode(2);      // only for nbiot
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
      mqtt.setServer(MQTT_HOST, MQTT_PORT);
      mqtt.setCallback(CallBack);
    }
  }

  /*Servo Intializing */ 
  for(int i=0; i<SERVO_COUNT; i++ ){
    Serial.print("Servo Motor - ");
    Serial.println(i+1);
    servo_list[i] = new ServoMotor(servo_DPins[i],servo_APins[i]);
    servo_list[i]->calibrate_Range();
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
  
  if(action == 0){  // publish mode (transmit current position to Platform)
    
    for(int i=0; i<SERVO_COUNT; i++){
      Serial.print("myServo ");
      Serial.print(i+1);
      Serial.print(" Pos : ");
      Serial.println(servo_list[i]->cur_Pos());
      
    }
//    sendRAPosition(double sp1, double sp2, double sp3);
    sendRAPosition((int)servo_list[0]->cur_Pos(),(int)servo_list[1]->cur_Pos(),(int)servo_list[2]->cur_Pos());
    delay(500);
    
  }else if (action == 1){  // subsrcibe mode (receive positions from platform)
    // TODO: recive positions 
  }

}