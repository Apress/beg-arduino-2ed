Project 38 – Simple Ultrasonic Rangefinder

#define sensorPin 9

long pwmRange, inch, cm;

void setup() { 
  // Start serial communications
  Serial.begin(115200);
  pinMode(sensorPin, INPUT);
}

void loop() {
pwmRange = pulseIn(sensorPin, HIGH);
  
  // 147uS per inch according to datasheet
  inch = pwmRange / 147;
  // convert inch to cm
  cm = inch * 2.54;
  
  Serial.print(inch);
  Serial.print(" inches    ");
  Serial.print(cm);
  Serial.println(" cm");
}

