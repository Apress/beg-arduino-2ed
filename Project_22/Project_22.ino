// Project 22 - LED Dot Matrix Display - Pong Game

LedControl myMatrix = LedControl(2, 4, 3, 1); // create an instance of a Matrix

int column = 0, row = random(8);
int directionX = 1, directionY = 1;
int paddle1 = 5, paddle1Val;
int movement_interval = 300;
int counter = 0;

void setup()
{ 
  myMatrix.shutdown(0,false);
  /* Set the brightness to a medium values */
  myMatrix.setIntensity(0,8);
  randomSeed(analogRead(0));
  oops();
}


void loop()
{
   paddle1Val = analogRead(paddle1);
   paddle1Val = map(paddle1Val, 0, 1024, 0,6);
   if (column == 6 && (paddle1Val == row || 
      paddle1Val+1 == row || paddle1Val+2 == row)) {directionX = -1;}
  if (column == 0) {directionX = 1;}
  if (row == 7) {directionY = -1;}
  if (row == 0) {directionY = 1;}
  if (column == 7) { oops();}
  column += directionX;
  row += directionY;
 displayDashAndDot();
 counter++; 
}

void oops() {
  for (int x=0; x<3; x++) {
    myMatrix.clearDisplay(0);
    delay(250);
    for (int y=0; y<8; y++) {
      myMatrix.setRow(0, y, 255);
    }
    delay(150);
  }
  counter=0;
  movement_interval=300;
  column=0;
  row = random(8); 
  displayDashAndDot();
}

void displayDashAndDot() {
  myMatrix.clearDisplay(0);
  myMatrix.setLed(0, column, row, HIGH);  
  myMatrix.setLed(0, 7, paddle1Val, HIGH);
  myMatrix.setLed(0, 7, paddle1Val+1, HIGH);
  myMatrix.setLed(0, 7, paddle1Val+2, HIGH);
  if (!(counter % 10)) {movement_interval -= 5;}
  delay(movement_interval);  
}

