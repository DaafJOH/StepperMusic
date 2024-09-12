#include <L293D.h>

#define MOTOR_A      5   // motor pin a
#define MOTOR_B      6   // motor pin b
#define MOTOR_ENABLE 3   // Enable (also PWM pin)

// Create motor object using given pins
L293D motor(MOTOR_A, MOTOR_B, MOTOR_ENABLE);

void setup() {  
    // begin --> true false, enables disables PWM
    motor.begin(true);
    pinMode(7, INPUT_PULLUP);
    pinMode(8, INPUT_PULLUP);
    pinMode(9, INPUT_PULLUP);
    // Speed -100%...0..100%
    motor.SetMotorSpeed(0);
}

void loop() {
  if ((!digitalRead(7) or !digitalRead(9)) and digitalRead(8)) {
    for(int j = 0; j < 3; j++) {
      if(!digitalRead(8)) {break;}
      motor.SetMotorSpeed(1);
      delay(500);
      for(int i = 0; i < 100; i++) {
        motor.SetMotorSpeed(i);
        delay(1);}
      motor.SetMotorSpeed(100);
      delay(1000);
      for(int i = 100; i > 0; i--) {
        motor.SetMotorSpeed(i);
        delay(40);}
      motor.SetMotorSpeed(1);
      if(!digitalRead(8)) {break;}
      delay(500);
      for(int i = 0; i > -100; i--) {
        motor.SetMotorSpeed(i);
        delay(1);}
      motor.SetMotorSpeed(-100);
      delay(1000);
      for(int i = -100; i < 0; i++) {
        motor.SetMotorSpeed(i);
        delay(40);}}
    motor.SetMotorSpeed(0);}
}