// Project 20 - LED Dot Matrix Display - Scrolling Sprite

#include <TimerOne.h>

int latchPin = 8; //Pin connected to Pin 12 of 74HC595 (Latch)
int clockPin = 12; //Pin connected to Pin 11 of 74HC595 (Clock)
int dataPin = 11; //Pin connected to Pin 14 of 74HC595 (Data)
byte frame = 0;  // variable to store the current frame being displayed

byte led[8][8] = { {0, 56, 92, 158, 158, 130, 68, 56}, // 8 frames of an animation
                      {0, 56, 124, 186, 146, 130, 68, 56},
                      {0, 56, 116, 242, 242, 130, 68, 56},
                      {0, 56, 68, 226, 242, 226, 68, 56},
                      {0, 56, 68, 130, 242, 242, 116, 56},
                      {0, 56, 68, 130, 146, 186, 124, 56},
                      {0, 56, 68, 130, 158, 158, 92, 56},
                      {0, 56, 68, 142, 158, 142, 68, 56} };

void setup() {
        pinMode(latchPin, OUTPUT);  // set the 3 digital pins to outputs
        pinMode(clockPin, OUTPUT);
        pinMode(dataPin, OUTPUT);

        Timer1.initialize(10000); // set a timer of length 10000 microseconds 
        Timer1.attachInterrupt(screenUpdate); // attach the screenUpdate function
}

void loop() {
        for (int i=0; i<8; i++) { // loop through all 8 frames of the animation
                for (int j=0; j<8; j++) { // loop through the 8 rows per frame
                        led[i][j]= led[i][j] << 1 | led[i][j] >> 7; // bitwise rotation
                }
        }
        frame++; // go to the next frame in the animation
        if (frame>7) { frame =0;} // make sure we go back to frame 0 once past 7
        delay(100); // wait a bit between frames
}

void screenUpdate() { // function to display image
   byte row = B10000000; // row 1
   for (byte k = 0; k < 8; k++) {

      shiftOut(dataPin, clockPin, LSBFIRST, led[frame][k] ); // LED array
      shiftOut(dataPin, clockPin, LSBFIRST, ~row); // row select (active low)

      // create a low to high transition on latchPin to transfer output to display
      digitalWrite(latchPin, LOW); 
      digitalWrite(latchPin, HIGH);
      row = row >> 1; // bitshift right
   }
   // turn all rows off until next timer interrupt so last row isn't on longer than others
   shiftOut(dataPin, clockPin, LSBFIRST, 0 ); // column doesn't matter w/ all rows off
   shiftOut(dataPin, clockPin, LSBFIRST, ~0); // select no row

   // create a low to high transition on latchPin to transfer output to display
   digitalWrite(latchPin, LOW); 
   digitalWrite(latchPin, HIGH);
}


