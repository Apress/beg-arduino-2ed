Project 44 – Simple RFID Reader

char val = 0; // value read for serial port

void setup() {
    Serial.begin(9600); 
}

void loop () {

  if(Serial.available() > 0) {
  val = Serial.read(); // read from the serial port
  Serial.write(val); // and print it to the monitor
  }
} 

