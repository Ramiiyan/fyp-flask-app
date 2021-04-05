#define SERVO_COUNT   {{servo_count_dof}} // based on robotic arm type
#include <ArduinoJson.h> //json decoder
/* Wroom ESP32 Servo-motor Control Digital Pins */
const byte servo_DPins[SERVO_COUNT] = {{Dpin_list}}; //  12,26,32,18,22,14

/* Wroom ESP32 Servo-motor Feedback Analog Pins */
const int servo_APins[SERVO_COUNT] = {{Apin_list}}; //  13,27,33,19,23,15

/*int array1[ rows-2 ][ columns-3 ] = { { 1, 2, 3 }, { 4, 5, 6 } }; EXAMPLE. */
const int servo_range [SERVO_COUNT] [2] = {{servo_range_list}};

DynamicJsonDocument doc(512);
void myDeserialize(String getmsg);
//void controller(String getVal);
