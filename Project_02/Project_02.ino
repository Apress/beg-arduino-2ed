// Project 2 - SOS Morse Signaller
// LED connected to digital pin 10 
int ledPin = 10;                

// run once, when the sketch starts
void setup()                    
{
// sets the digital pin as output
pinMode(ledPin, OUTPUT);     
}

// run over and over again 
void loop()                     
{
// 3 dits
for (int x=0; x<3; x++) {
digitalWrite(ledPin, HIGH);   // sets the LED on
delay(150);                  // waits for 150ms
digitalWrite(ledPin, LOW);    // sets the LED off
delay(100);               // waits for 100ms
  	}
  
  // 100ms delay to cause slight gap between letters
  	delay(100);     
  // 3 dahs
  	for (int x=0; x<3; x++) {
  		digitalWrite(ledPin, HIGH);   // sets the LED on
  		delay(400);                  // waits for 400ms
  		digitalWrite(ledPin, LOW);    // sets the LED off
  		delay(100);               // waits for 100ms
  	}
  
  // 100ms delay to cause slight gap between letters 
  delay(100); 
  
  // 3 dits again
  	for (int x=0; x<3; x++) {
  		digitalWrite(ledPin, HIGH);   // sets the LED on
  		delay(150);                  // waits for 150ms
  		digitalWrite(ledPin, LOW);    // sets the LED off
  		delay(100);               // waits for 100ms
  	}
  
// wait 5 seconds before repeating the SOS signal 
delay(5000); 
}

