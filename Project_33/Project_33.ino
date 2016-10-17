// Project 33 - Basic Touch Screen

// Power connections
#define Left 8     // Left (X1) to digital pin 8
#define Bottom 9   // Bottom (Y2) to digital pin 9
#define Right 10   // Right (X2) to digital pin 10
#define Top 11     // Top (Y1) to digital pin 11

// Analog connections 
#define topInput 0   // Top (Y1) to analog pin 0
#define rightInput 1 // Right (X2) to analog pin 1 

int coordX = 0, coordY = 0;

void setup()
{
        Serial.begin(38400);
}

void loop()
{    
        if (touch())  // If screen touched, print co-ordinates
        {
                Serial.print(coordX);
                Serial.print("  ");
                Serial.println(coordY);
                delay(250); 
        }
}

// return TRUE if touched, and set coordinates to touchX and touchY
boolean touch()
{
        boolean touch = false;

        // get horizontal co-ordinates

        pinMode(Top, INPUT); // Top and Bottom to high impedance
        pinMode(Bottom, INPUT);

        pinMode(Left, OUTPUT); 
        digitalWrite(Left, LOW); // Set Left to low 

        pinMode(Right, OUTPUT); // Set right to +5v
        digitalWrite(Right, HIGH);


        delay(3); 
        coordX = analogRead(topInput);

        // get vertical co-ordinates

        pinMode(Right, INPUT); // left and right to high impedance
        pinMode(Left, INPUT);

        pinMode(Bottom, OUTPUT); // set Bottom to Gnd
        digitalWrite(Bottom, LOW);

        pinMode(Top, OUTPUT); // set Top to +5v
        digitalWrite(Top, HIGH);


        delay(3); 
        coordY = analogRead(rightInput);

        // if co-ordinates read are less than 1000 and greater than 24 
        // then the screen has been touched
        if(coordX < 1000 && coordX > 24 && coordY < 1000 && coordY > 24) {touch = true;}

          return touch;
}

