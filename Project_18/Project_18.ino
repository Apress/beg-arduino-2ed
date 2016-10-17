// Project 18 - Dual Binary Counters

int latchPin = 8;  //Pin connected to Pin 12 of 74HC595 (Latch)
int clockPin = 12; //Pin connected to Pin 11 of 74HC595 (Clock)
int dataPin = 11;  //Pin connected to Pin 14 of 74HC595 (Data)

void setup() {
  	//set pins to output 
pinMode(latchPin, OUTPUT);
  	pinMode(clockPin, OUTPUT);
  	pinMode(dataPin, OUTPUT);
}

void loop() {
for (int i = 0; i < 255; i++) {     //count from 0 to 255
digitalWrite(latchPin, LOW); //set latchPin low to allow data flow
shiftOut(dataPin, clockPin, LSBFIRST, i);  // shift out first 8 bits
  shiftOut(dataPin, clockPin, LSBFIRST, 255-i); // shiftOut second 8 bits//set latchPin to high to lock and send data
digitalWrite(latchPin, HIGH); 
    		delay(250 );
  	}
}

