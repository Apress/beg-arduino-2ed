// Project 34 - Touch Screen Light Controller

// Power connections
#define Left 8     // Left (X1) to digital pin 8
#define Bottom 9   // Bottom (Y2) to digital pin 9
#define Right 10   // Right (X2) to digital pin 10
#define Top 11     // Top (Y1) to digital pin 11

// Analog connections 
#define topInput 0   // Top (Y1) to analog pin 0
#define rightInput 1 // Right (X2) to analog pin 1 

// RGB pins
#define pinR 3
#define pinG 5
#define pinB 6

int coordX = 0, coordY = 0;
boolean ledState = true;
int red = 100, green = 100, blue = 100;

void setup()
{
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
}

void loop()
{    
        if (touch()) {
                if ((coordX>0 && coordX<270) && (coordY>0 && coordY<460)) 
                {
                    ledState = true; delay(50);
                }

                if ((coordX>0 && coordX<270) && (coordY>510 && coordY< 880))
                {
                    ledState = false; delay(50);
                }

                if ((coordX>380 && coordX<930) && (coordY>0 && coordY<300)) 
               {
                  red=map(coordX, 380, 930, 0, 255);
               }

                if ((coordX>380 && coordX<930) && (coordY>350 && coordY<590)) 
                {
                   green=map(coordX, 380, 930, 0, 255);
                } 

                if ((coordX>380 && coordX<930) && (coordY>640 && coordY<880)) 
                {
                   blue=map(coordX, 380, 930, 0, 255);
                }

                delay(10);
}

        if (ledState) {
                analogWrite(pinR, red);
                analogWrite(pinG, green);
                analogWrite(pinB, blue);
        }
        else {
                analogWrite(pinR, 0);
                analogWrite(pinG, 0);
                analogWrite(pinB, 0);
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



