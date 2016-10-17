// Project 5 - LED Chase Effect
byte ledPin[] = {4, 5, 6, 7, 8, 9, 10, 11, 12, 13};   // Create array for LED pins
int ledDelay = 65;  // delay between changes
int direction = 1;
int currentLED = 0;
unsigned long changeTime;

void setup() {
  	for (int x=0; x<10; x++) {     // set all pins to output
    		pinMode(ledPin[x], OUTPUT); }
    	changeTime = millis();
}

void loop() {
  	if ((millis() - changeTime) > ledDelay) {      // if it has been ledDelay ms since last change
    		changeLED();
    		changeTime = millis();
  	}
}

void changeLED() {
  	for (int x=0; x<10; x++) {        	// turn off all LED's
   		 digitalWrite(ledPin[x], LOW);
  	}
  	digitalWrite(ledPin[currentLED], HIGH); 	// turn on the current LED
	currentLED += direction; 	// increment by the direction value
  	// change direction if we reach the end
  	if (currentLED == 9) {direction = -1;}
  	if (currentLED == 0) {direction = 1;}
}

