// Project 27 - Joystick Servo Control

#include <Servo.h> 

Servo servo1;  // Create a servo object 
Servo servo2;  // Create a second servo object  
int pot1, pot2;

void setup() 
{ 
  servo1.attach(5);  // Attaches the servo on pin 5 to the servo1 object 
  servo2.attach(6);  // Attaches the servo on pin 6 to the servo2 object 
 
  servo1.write(90);  // Put servo1 at home position
  servo2.write(90);  // Put servo2 at home postion

} 
 
void loop() 
{ 
  pot1 = analogRead(3); // Read the X-Axis
  pot2 = analogRead(4); // Read the Y-Axis
  pot1 = map(pot1,0,1023,0,180);
  pot2=map(pot2,0,1023,0,180);
  servo1.write(pot1);
  servo2.write(pot2);
  delay(15);
}

