Project 37 – One-Wire Digital Temperature Sensor

#include <OneWire.h>
#include <DallasTemperature.h>

// Data line goes to digital pin 3
#define ONE_WIRE_BUS 3

// Setup a oneWire instance to communicate with any 
// OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
DeviceAddress insideThermometer, outsideThermometer;

void setup()
{
  // start serial port
  Serial.begin(9600);

  // Start up the library
  sensors.begin();

  // locate devices on the bus
  Serial.print("Locating devices...");
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  if (!sensors.getAddress(insideThermometer, 0)) 
 {
     Serial.println("Unable to find address for Device 0"); 
  }

  if (!sensors.getAddress(outsideThermometer, 1)) 
 {
     Serial.println("Unable to find address for Device 1"); 
  }

  // print the addresses of both devices
  Serial.print("Device 0 Address: ");
  printAddress(insideThermometer);
  Serial.println();

  Serial.print("Device 1 Address: ");
  printAddress(outsideThermometer);
  Serial.println();
  Serial.println();
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (int i = 0; i < 8; i++)
  {
    // zero pad the address if necessary
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}

// function to print the temperature for a device
void printTemperature(DeviceAddress deviceAddress)
{
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  Serial.print(" Temp F: ");
  Serial.print(DallasTemperature::toFahrenheit(tempC));
}

// main function to print information about a device
void printData(DeviceAddress deviceAddress)
{
  Serial.print("Device Address: ");
  printAddress(deviceAddress);
  Serial.print(" ");
  printTemperature(deviceAddress);
  Serial.println();
}

void loop()
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
   sensors.requestTemperatures();
  Serial.println("DONE");
  delay(750);

  // print the device information
  printData(insideThermometer);
  printData(outsideThermometer);
  Serial.println();
  delay(1000);
}

