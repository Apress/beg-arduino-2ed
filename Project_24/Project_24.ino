// Project 24 - LCD Temperature Display

#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // create an lcd object and assign the pins
int maxC=0, minC=100, maxF=0, minF=212;
int scale = 1;
int buttonPin=8;

void setup() {
  	lcd.begin(16, 2); // Set the display to 16 columns and 2 rows
  	analogReference(INTERNAL);
	// analogReference(INTERNAL1V1); If you have an Arduino Mega
  	pinMode(buttonPin, INPUT);
lcd.clear();
}

void loop() {
  	lcd.setCursor(0,0); // set cursor to home position
  	int sensor = analogRead(0); // read the temp from sensor
  	int buttonState = digitalRead(buttonPin); // check for button press
  	switch (buttonState) { // change scale state if pressed
  		case HIGH:
    			scale=-scale; // invert scale
    			lcd.clear();
  	}

  	switch (scale) { // decide if C or F scale
  		case 1:
    			celsius(sensor);
    			break;
  		case -1:
    			fahrenheit(sensor);
  	}
  	delay(250);
}

void celsius(int sensor) {
  	lcd.setCursor(0,0);
  	int temp = sensor * 0.1074188; // convert to C
  	lcd.print(temp);
  	lcd.write(B11011111); // degree symbol
  	lcd.print("C ");
  	if (temp>maxC) {maxC=temp;}
  	if (temp<minC) {minC=temp;}
  	lcd.setCursor(0,1);
  	lcd.print("H=");
  	lcd.print(maxC);
  	lcd.write(B11011111);
  	lcd.print("C L=");
  	lcd.print(minC);
  	lcd.write(B11011111);
  	lcd.print("C ");
}

void fahrenheit(int sensor) {
  	lcd.setCursor(0,0);
  	float temp = ((sensor * 0.1074188) * 1.8)+32; // convert to F
  	lcd.print(int(temp));
  	lcd.write(B11011111); // print degree symbol
  	lcd.print("F ");
  	if (temp>maxF) {maxF=temp;}
  	if (temp<minF) {minF=temp;}
  	lcd.setCursor(0,1);
  	lcd.print("H=");
  	lcd.print(maxF);
  	lcd.write(B11011111);
  	lcd.print("F L=");
  	lcd.print(minF);
  	lcd.write(B11011111);
  	lcd.print("F ");
}

