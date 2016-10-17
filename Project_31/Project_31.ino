// Project 31 - Digital Pressure Sensor
// Based on the One Shot example by Henry Lahr

#include <Wire.h> // so we can use I2C communication
#define MYALTITUDE 262 //define altitude at your location to calculate mean sea level pressure in meters

// Register addresses
const int SENSORADDRESS = 0x60; // MPL3115A1 address from the datasheet
#define SENSOR_CONTROL_REG_1 0x26
#define SENSOR_DR_STATUS 0x00  // Address of DataReady status register
#define SENSOR_OUT_P_MSB 0x01  // Starting address of Pressure Data registers

float baroAltitudeCorrectionFactor = 1/(pow(1-MYALTITUDE/44330.77,5.255877));

byte I2Cdata[5] = {0,0,0,0,0}; //buffer for sensor data
 
void setup(){
  Wire.begin(); // join i2c bus
  Serial.begin(9600); // start serial for output at 9600 baud
  Serial.println("Setup");
  I2C_Write(SENSOR_CONTROL_REG_1, 0b00000000); // put in standby mode
  // these upper bits of the control register 
  // can only be changed while in standby
  I2C_Write(SENSOR_CONTROL_REG_1, 0b00111000); // set oversampling to 128
  Serial.println("Done.");
}
 
void loop(){
  float temperature, pressure, baroPressure;
  
  Read_Sensor_Data();
  temperature = Calc_Temperature();
  pressure = Calc_Pressure();
  baroPressure = pressure * baroAltitudeCorrectionFactor;
  Serial.print("Absolute pressure: ");
  Serial.print(pressure); // in Pascal
  Serial.print(" Pa,    Barometer: ");
  Serial.print(baroPressure); // in Pascal
  Serial.print(" Pa,    Temperature: ");
  
  Serial.print(temperature); // in degrees C
  Serial.println(" C");
  delay(1000);
}
 
  // Read the pressure and temperature readings from the sensor
void Read_Sensor_Data(){

  // request a single measurement from the sensor
  I2C_Write(SENSOR_CONTROL_REG_1, 0b00111010); //bit 1 is one shot mode 

  // Wait for measurement to complete. 
  // One-shot bit will clear when it is done.
  // Rread the current (sensor control) register
  // repeat until sensor clears OST bit
  do {
    Wire.requestFrom(SENSORADDRESS,1);
  } while ((Wire.read() & 0b00000010) != 0);
 
  I2C_ReadData(); //reads registers from the sensor
}
 
  // This function assembles the pressure reading 
  // from the values in the read buffer
  // The two lowest bits are fractional so divide by 4
float Calc_Pressure(){
  unsigned long m_pressure = I2Cdata[0];
  unsigned long c_pressure = I2Cdata[1];
  float l_pressure = (float)(I2Cdata[2]>>4)/4; 
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

// Read Barometer and Temperature data (5 bytes)
void I2C_ReadData(){
  byte readUnsuccessful;
  do {
    byte i=0;
    byte dataStatus = 0;
    
    Wire.beginTransmission(SENSORADDRESS);
    Wire.write(SENSOR_OUT_P_MSB);
    Wire.endTransmission(false);

    // read 5 bytes. 3 for pressure, 2 for temperature.
    Wire.requestFrom(SENSORADDRESS,5);
    while(Wire.available()) I2Cdata[i++] = Wire.read();

    // in some modes it is possible for the sensor 
    // to update the pressure reading
    // while we were in the middle of reading it, 
    // in which case our copy is garbage
    // (parts of two different readings)
    // We can check bits in the DR (data ready) 
    // register to see if this happened.

    Wire.beginTransmission(SENSORADDRESS);
    Wire.write(SENSOR_DR_STATUS);
    Wire.endTransmission(false);

    Wire.requestFrom(SENSORADDRESS,1); //read 5 bytes. 3 for pressure, 2 for temperature.
    dataStatus = Wire.read();
    readUnsuccessful = (dataStatus & 0x60) != 0;  
    // This will be unsuccessful if overwrite happened 
    // while we were reading the pressure or temp data.
    // So keep reading until we get a successful clean read
  } while (readUnsuccessful);
}
 
// This function writes one byte over I2C
void I2C_Write(byte regAddr, byte value){
  Wire.beginTransmission(SENSORADDRESS);
  Wire.write(regAddr);
  Wire.write(value);
  Wire.endTransmission(true);
}

