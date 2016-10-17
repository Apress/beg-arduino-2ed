Project 40 – Ultrasonic Alarm

#include "LedControl.h"

#define sensorPin 9
#define switchPin 7
#define buttonPin 6
#define potPin 0
#define DataIn 2
#define CLK 4
#define LOAD 3
#define NumChips 1
#define samples 5.0

float pwmRange, averageReading, inch, cm, alarmRange;
LedControl lc=LedControl(DataIn,CLK,LOAD,NumChips);

void setup() {

 // Wakeup the MAX7219
  lc.shutdown(0,false);
  // Set it to medium brightness
  lc.setIntensity(0,8);
  // clear the display
  lc.clearDisplay(0);
  pinMode(sensorPin, INPUT);
  pinMode(switchPin, INPUT);
}


void loop() { 
  readPot();
  averageReading = 0;
  for (int i = 0; i<samples; i++) {
   pwmRange = pulseIn(sensorPin, HIGH);
  averageReading += pwmRange;
  }

  averageReading /= samples;
  // 147uS per inch according to datasheet
  inch = averageReading / 147;
  // convert inch to cm
  cm = inch * 2.54;

  if (digitalRead(switchPin)) {
  displayDigit(inch);
  }
  else {
    displayDigit(cm);
  }
  
        // if current range smaller than alarmRange, set off alarm
        if (inch<=alarmRange) {startAlarm();}
}

void displayDigit(float value) { 
    int number = value*100;
    lc.setDigit(0,4,number/10000,false); // 100s digit
    lc.setDigit(0,3,(number%10000)/1000,false); // 10s digit
    lc.setDigit(0,2,(number%1000)/100,true); // first digit
    lc.setDigit(0,1,(number%100)/10,false); // 10th digit
    lc.setDigit(0,0,number%10,false); // 100th digit
}

// read the potentiometer
float readPot()  {
  float potValue = analogRead(potPin);
  alarmRange = 254 * (potValue/1024);
  return alarmRange;
}

// set off the alarm sound till reset pressed
void startAlarm() {
  while(1) {
    for (int freq=800; freq<2500;freq++) {
      tone(8, freq);
      if (digitalRead(buttonPin)) {
        noTone(8);
        return;
      }
    }
  }
}

