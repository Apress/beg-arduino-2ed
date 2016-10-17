// Project 25 - Servo Control

#include <Servo.h> 
 
Servo servo1;  // Create a servo object 
 
void setup() 
{ 
        servo1.attach(5);  // Attaches the servo on Pin 5 to the servo object 
} 
 
void loop() 
{ 
        int angle = analogRead(0); // Read the pot value
        angle=map(angle, 0, 1023, 0, 180); // Map the values from 0 to 180 degrees
        servo1.write(angle); // Write the angle to the servo
        delay(15); // Delay of 15ms to allow servo to reach position 
} 

