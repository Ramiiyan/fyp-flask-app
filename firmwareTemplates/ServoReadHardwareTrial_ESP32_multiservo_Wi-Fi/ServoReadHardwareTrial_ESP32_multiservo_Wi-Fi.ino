#include "ServoMotor.h" // custom library header for Servo motors
#include "Com_Config.h" // custom library header for GSM_Module.
#include "mainTemplate.h"

/*ServoMotor  *myServo1, *myServo2; */
ServoMotor *servo_list[SERVO_COUNT];
boolean hasInitControl = false;
boolean hasInitMonitor = false;
int servoPos[SERVO_COUNT]; //using in publish positions (monitor)

int getPos[SERVO_COUNT]; //using in control
char *posName[] ={"b","j1","j2","fe","j3","j4"};
String recentCmsg = "c";

void setup() {
  Serial.begin(BAUD_RATE);
  Serial.println(F("setup begin..."));
  setup_wifi();
  delay(1000);
  mqtt.begin(MQTT_HOST, MQTT_PORT,espClient);
  mqtt.onMessage(messageReceived);
  delay(200);
  /*Servo Intializing */ 
  for(int i=0; i<SERVO_COUNT; i++ ){
    Serial.print(F("Servo Motor - "));
    Serial.println(i+1);
    servo_list[i] = new ServoMotor(servo_DPins[i],servo_APins[i]);
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
      servoPos[i] = (int)servo_list[i]->cur_Pos(servo_range[i][0],servo_range[i][1]);
      Serial.println(servoPos[i]);
    }
    
    if(SERVO_COUNT == 2){
      sendRAPosition(servoPos[0],servoPos[1],0,0,0,0);
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
      Serial.println(F("*************"));
      recentCmsg = getmsg;
    }
  }

  
 
}
