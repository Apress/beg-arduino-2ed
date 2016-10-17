// Project 28 - Basic Stepper Control

#include <Stepper.h>

// steps value is 360 / degree angle of motor
#define STEPS 200

// create a stepper object on pins 4, 5, 6 and 7
Stepper stepper(STEPS, 4, 5, 6, 7);

void setup()
{
}

void loop()
{
    stepper.setSpeed(60);
    stepper.step(200);
    delay(100);
    stepper.setSpeed(20);
    stepper.step(-50);
    delay(100);
}

