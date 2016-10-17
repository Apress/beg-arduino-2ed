// Project 32 - Digital Barograph

#include <glcd.h>
#include "fonts/allFonts.h"  
#include <Wire.h> // so we can use I2C communication

// The altitude at your location is needed to calculate the mean sea 
// level pressure in meters
#define MYALTITUDE 262
#define SENSORADDRESS 0x60 // The address of the MPL3115A1
#define SENSOR_CONTROL_REG_1 0x26
#define SENSOR_DR_STATUS 0x00  // Address of DataReady status register
#define SENSOR_OUT_P_MSB 0x01  // Starting address of Pressure Data registers

  // Time interval in seconds (approx.) per graph tic
// Scale to deduct from hPa reading to fit within the 40 pixels graph 990-1030
#define INTERVAL 900

float baroAltitudeCorrectionFactor = 1/(pow(1-MYALTITUDE/44330.77,5.255877));

byte I2Cdata[5] = {0,0,0,0,0}; //buffer for sensor data
float pressure, temperature, buffer[30], baroPressure;
int dots[124], dotCursor = 0, counter = 0, index = 0, numReadingsBuffered = 0;

void setup()
{
  Wire.begin(); // join i2c bus
  I2C_Write(SENSOR_CONTROL_REG_1, 0b00000000); // put in standby mode
  // these upper bits of the control register 
  // can only be changed while in standby

// Set oversampling to 128. Then trigger first reading
  I2C_Write(SENSOR_CONTROL_REG_1, 0b00111010);

  GLCD.Init();   // initialise the library
  GLCD.ClearScreen(); 
  GLCD.SelectFont(System5x7, BLACK); // load the font
  
  GLCD.DrawRect(1,1,125,44); // Draw a rectangle
  for (int x=0; x<46; x+=11) { // Draw vertical scale
    GLCD.SetDot(0,1+x, BLACK);
    GLCD.SetDot(127,1+x, BLACK);
  }
  for (int x=0; x<128; x+=5) { // Draw horizontal scale
    GLCD.SetDot(1+x,0, BLACK);
  }

// Clear the array to standard sea level
  for (byte x=0; x<124; x++) {dots[x]=1013;}

  Read_Sensor_Data();
  drawPoints(dotCursor);
}

void loop()
{  
  Read_Sensor_Data();

  GLCD.CursorToXY(0, 49); // print pressure
  GLCD.print("hPa:"); 
  GLCD.CursorToXY(24,49);
  GLCD.print(baroPressure/100);
  GLCD.print("   ");  // erase any old value longer than new value
  
  float tempF = (temperature*1.8) + 32;
 
  GLCD.CursorToXY(0,57); // print temperature
  GLCD.print("Temp:");
  GLCD.CursorToXY(28, 57);
//  GLCD.print(temperature); // change to temperature for Centigrade
  GLCD.print(tempF); 
  GLCD.print("   ");  // erase any old value longer than new value
    
  delay(1000);
  
  GLCD.CursorToXY(84,49); // print trend
  GLCD.print("TREND:");
  GLCD.CursorToXY(84,57);
  printTrend();
  
  counter++;
  if (counter==INTERVAL) {
    drawPoints(dotCursor);
    counter = 0;
  }
}

void drawPoints(int position) {
  dots[dotCursor] = int(baroPressure/100);
  int midscale = dots[dotCursor];  // center graph scale on current reading 
  GLCD.FillRect(2, 2, 123, 40, WHITE); // clear graph area
  for (int x=0; x<124; x++) {
    // limit to graph boundary
    int y = constrain(22-((dots[position]- midscale)), 0,44);
    GLCD.SetDot(125-x,y, BLACK);
    position--;
    if (position<0) {position=123;}
  }
  dotCursor++;
  if (dotCursor>123) {dotCursor=0;}
}

// calculate trend since last data point and print
void printTrend() { 
  int dotCursor2=dotCursor-1;
  if (dotCursor2<0) {dotCursor2=123;}
  int val1=dots[dotCursor2];
  int dotCursor3=dotCursor2-1;
  if (dotCursor3<0) {dotCursor3=123;}
  int val2=dots[dotCursor3];
  if (val1>val2) {GLCD.print("RISING ");}
  if (val1==val2) {GLCD.print("STEADY ");}
  if (val1<val2) {GLCD.print("FALLING");}
}
 
  // This function triggers a reading, waits for acquisition complete, and 
  //  reads the pressure and temperature readings from the sensor
void Read_Sensor_Data(){
float tempPressure;
 
  // request a single measurement from the sensor
  I2C_Write(SENSOR_CONTROL_REG_1, 0b00111010); //bit 1 is one shot mode 

  // wait for measurement to complete. 
  // One-shot bit will clear when it is done.
  do {
   // read the current (sensor control) register
   // then repeat until sensor clears OST bit
    Wire.requestFrom(SENSORADDRESS,1); 
  } while ((Wire.read() & 0b00000010) != 0);
 
  I2C_ReadData(); //reads registers from the sensor
  temperature = Calc_Temperature();
  tempPressure = Calc_Pressure();
  buffer[index++]=tempPressure;
  if (index > 29)  index = 0; // at end of buffer, wrap around to start.
  numReadingsBuffered++;
  if (numReadingsBuffered > 30)   numReadingsBuffered = 30;
  
  float mean = 0;
  for (int i=0; i<numReadingsBuffered; i++) {
    mean = mean+buffer[i];
  }
  pressure = mean/numReadingsBuffered;
  baroPressure = pressure*baroAltitudeCorrectionFactor;
}
 
// This function takes values from the read buffer 
// and converts them to pressure units
float Calc_Pressure(){
  unsigned long m_pressure = I2Cdata[0];
  unsigned long c_pressure = I2Cdata[1];
  float l_pressure = (float)(I2Cdata[2]>>6)/4;
  return((float)(m_pressure<<10 | c_pressure<<2)+l_pressure);
}
 
// This function assembles the temperature reading 
// from the values in the read buffer
float Calc_Temperature(){
  int m_temp;
  float l_temp;
  m_temp = I2Cdata[3]; //temperature in whole degrees C
  l_temp = (float)(I2Cdata[4]>>4)/16.0; //fractional portion of temperature
  return((float)(m_temp + l_temp));
}
void I2C_ReadData(){  //Read Barometer and Temperature data (5 bytes)
  byte readUnsuccessful;
  do {
    byte i=0;
    byte dataStatus = 0;
    
    Wire.beginTransmission(SENSORADDRESS);
    Wire.write(SENSOR_OUT_P_MSB);
    Wire.endTransmission(false);

    //read 5 bytes. 3 for pressure, 2 for temperature.
    Wire.requestFrom(SENSORADDRESS,5);
    while(Wire.available()) I2Cdata[i++] = Wire.read();

    // in some modes it is possible for the sensor to 
    // update the pressure reading while we were in 
    // the middle of reading it, in which case our copy is garbage
    // because it has parts of two different readings.
    // We can check some status bits in the DR (data ready) 
    // register to see if this happened.

    Wire.beginTransmission(SENSORADDRESS);
    Wire.write(SENSOR_DR_STATUS);
    Wire.endTransmission(false);

    //read 1 byte to get status value
    Wire.requestFrom(SENSORADDRESS,1);
    dataStatus = Wire.read();
    readUnsuccessful = (dataStatus & 0x60) != 0;  
    // unsuccessful if overwrite happened while we 
    // were reading the pressure or temp data
   // keep reading until we get a successful clean read
  } while (readUnsuccessful);
}
 
// This function writes one byte over I2C
void I2C_Write(byte regAddr, byte value){
Wire.beginTransmission(SENSORADDRESS);
  Wire.write(regAddr);
  Wire.write(value);
  Wire.endTransmission(true);
}

