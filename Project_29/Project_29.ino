// Project 29 - Using a motor shield
#define QUARTER_SPEED 64
#define HALF_SPEED 128
#define FULL_SPEED 255

// Set the pins for speed and direction of each motor
int speed1 = 3;  
int speed2 = 11;  
int direction1 = 12; 
int direction2 = 13;

void stopMotor() {
  // turn both motors off
  analogWrite(speed1, 0);  
  analogWrite(speed2, 0);
}

void setup()
{
  // set all the pins to outputs
  pinMode(speed1, OUTPUT);  
  pinMode(speed2, OUTPUT);
  pinMode(direction1, OUTPUT);
  pinMode(direction2, OUTPUT);
}

void loop()
{
  // Both motors forward at 50% speed for 2 seconds
  digitalWrite(direction1, HIGH); 
  digitalWrite(direction2, HIGH);  
  analogWrite(speed1, HALF_SPEED);  
  analogWrite(speed2, HALF_SPEED);
  delay(2000);
  
  stopMotor(); delay(1000); // stop
  
  // Left turn for 1 second
  digitalWrite(direction1, LOW); 
  digitalWrite(direction2, HIGH);  
  analogWrite(speed1, HALF_SPEED);  
  analogWrite(speed2, HALF_SPEED);
  delay(1000);
  
  stopMotor(); delay(1000); // stop
  
  // Both motors forward at 50% speed for 2 seconds
  digitalWrite(direction1, HIGH);  
  digitalWrite(direction2, HIGH);  
  analogWrite(speed1, HALF_SPEED);  
  analogWrite(speed2, HALF_SPEED);
  delay(2000);
  
  stopMotor(); delay(1000); // stop
  
  // rotate right at 25% speed
  digitalWrite(direction1, HIGH); 
  digitalWrite(direction2, LOW);  
  analogWrite(speed1, QUARTER_SPEED);  
  analogWrite(speed2, QUARTER_SPEED);
  delay(2000);

  stopMotor(); delay(1000); // stop

}

