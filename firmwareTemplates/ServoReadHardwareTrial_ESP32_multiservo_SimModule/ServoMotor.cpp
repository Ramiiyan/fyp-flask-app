#include "ServoMotor.h" //header file included.
//#include <Servo.h>

//Default constructor
ServoMotor::ServoMotor(): servo_DIOpin(0),servo_AIpin(0){
// for setup made.
}
//with Argument
ServoMotor::ServoMotor(byte servo_DIOpin, int servo_AIpin){
  // constructor
  this->servo_DIOpin = servo_DIOpin;
  this->servo_AIpin = servo_AIpin;
  init();
  
}

void ServoMotor::init() {
  this->servo_Obj.attach(servo_DIOpin);
    
}

void ServoMotor::calibrate_Range(int d_of_min, int d_of_max ){
  servo_Obj.write(0); //set the servo to 0 position
  delay(2000); //wait for the servo to reach there
  servo_Range_Min = analogRead(servo_AIpin); // Pot value at 0 degrees
  Serial.println("Pot value for 0 deg is " + String(servo_Range_Min)); // Print it!
  delay(500); //fancy delay
  
  servo_Obj.write(180); //go to 180 degrees
  delay(2000); //wait for the servo to reach there
  servo_Range_Max = analogRead(servo_AIpin); //pot value at 180 deg
  Serial.println("Pot value for 180 deg is " + String(servo_Range_Max));
  delay(500); //fancy delay
  
//  Serial.println("Now going to 90 Degrees"); //It does what it says
//  servo_Obj.write(90);// going to 90 degrees
//  delay(2000);// wait for it to reach there
  servo_Obj.detach(); // stop the PWM so that we can freely move the servo
//  delay(1000);

}

long ServoMotor::cur_Pos(){
  
  return map(analogRead(servo_AIpin), servo_Range_Min, servo_Range_Max, 0, 180);
}
