// Project 10 - Serial controlled mood lamp

char buffer[18];
int red, green, blue;

int RedPin = 11;
int GreenPin = 10;
int BluePin = 9;

void setup() 
{ 
  	Serial.begin(9600);
while(Serial.available())
 		Serial.read();  	
pinMode(RedPin, OUTPUT);
  	pinMode(GreenPin, OUTPUT);
  	pinMode(BluePin, OUTPUT);
} 

void loop() 
{ 
  	if (Serial.available() > 0) {
    		int index=0;
    		delay(100); // let the buffer fill up
    		int numChar = Serial.available();
    		if (numChar>15) {
      			numChar=15;
   		 }
    		while (numChar--) {
      			buffer[index++] = Serial.read();
    		}
    		splitString(buffer);
  	}
}

void splitString(char* data) {
  	Serial.print("Data entered: ");
  	Serial.println(data);
  	char* parameter; 
  	parameter = strtok (data, " ,");   // Note that this is a space before the comma in " , "
  	while (parameter != NULL) {
    		setLED(parameter);
    		parameter = strtok (NULL, " ,");  // space before the comma in " , "
}

 	 // Clear the text and serial buffers
  	for (int x=0; x<16; x++) {
    		buffer[x]='\0';
 	 }
  	while(Serial.available())
Serial.read();}

void setLED(char* data) {
  	if ((data[0] == 'r') || (data[0] == 'R')) {
    		int Ans = strtol(data+1, NULL, 10);
    		Ans = constrain(Ans,0,255);
    		analogWrite(RedPin, Ans);
    		Serial.print("Red is set to: ");
    		Serial.println(Ans);
  	}
  	if ((data[0] == 'g') || (data[0] == 'G')) {
    		int Ans = strtol(data+1, NULL, 10);
    		Ans = constrain(Ans,0,255);
    		analogWrite(GreenPin, Ans);
    		Serial.print("Green is set to: ");
    		Serial.println(Ans);
  	}
  	if ((data[0] == 'b') || (data[0] == 'B')) {
    		int Ans = strtol(data+1, NULL, 10);
    		Ans = constrain(Ans,0,255);
    		analogWrite(BluePin, Ans);
    		Serial.print("Blue is set to: ");
    		Serial.println(Ans);
  	}
}

