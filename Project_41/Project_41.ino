Project 41 – Ultrasonic Theremin

#define sensorPin 9

#define lowerFreq 123 // C3
#define upperFreq 2093 // C7
#define playHeight 36

float pwmRange, inch, cm, note;

void setup() {
        pinMode(sensorPin, INPUT);
}

void loop() {
        pwmRange = pulseIn(sensorPin, HIGH);

        inch = pwmRange / 147;
        // convert inch to cm
        cm = inch * 2.54;
  
        // map the playHeight range to the upper and lower frequencies
        note = map(inch, 0, playHeight, lowerFreq, upperFreq);
        if (inch<playHeight) {tone(8, note); }
        else {noTone(8);}
}

