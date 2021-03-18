#ifndef SERVO_MOTOR_H 
#define SERVO_MOTOR_H

#include <ESP32Servo.h>
//#include <Servo.h>  //servo library
#include <Arduino.h>


class ServoMotor {
  //private variables. 
  // Soon will include full servo specifications. 
  private:
    byte servo_DIOpin;
    int servo_AIpin;
    Servo servo_Obj;
    unsigned int servo_Range_Min;
    unsigned int servo_Range_Max;
    
  public:
    ServoMotor();
    ServoMotor(byte servo_DIOpin, int servo_AIpin);
    void init();
    void calibrate_Range();
    long cur_Pos();
    
};

#endif
