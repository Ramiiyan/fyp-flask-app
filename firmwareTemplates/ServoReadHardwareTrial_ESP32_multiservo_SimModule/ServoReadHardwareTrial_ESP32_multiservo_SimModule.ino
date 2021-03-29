#include "ServoMotor.h" // custom library header for Servo motors
#include "GSM_Setting.h" // custom library header for GSM_Module.

//define DOF
#define SERVO_COUNT   {{servo_count_dof}}

// Wroom ESP32 Servo-motor Control Digital Pins 
const byte servo_DPins[SERVO_COUNT] = {{Dpin_list}}; //by Default

// Wroom ESP32 Servo-motor Feedback Analog Pins
const int servo_APins[SERVO_COUNT] = {{Apin_list}};

//int array1[ rows-2 ][ columns-3 ] = { { 1, 2, 3 }, { 4, 5, 6 } }; EXAMPLE.
const int servo_range [SERVO_COUNT] [2] = {{0,180}};

//initiate Servo Motors
ServoMotor *servo_list[SERVO_COUNT];

void setup() {
  
  Serial.begin({{serial_setting.baud_rate}});
  delay(10);
  Serial.println("setup begin...");

/*  Sim Configuration */
  SerialSIM.begin({{serial_setting.sim_module}}, SERIAL_8N1, (int8_t) {{serial_setting.sim_module_Rxd}}, (int8_t) {{serial_setting.sim_module_Txd}}); //  Myserial.begin(9600, SERIAL_8N1, RXD, TXD);
  modem.setNetworkMode({{sim_network_setting.network_mode}});         // 38-nbiot 13-gsm
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
