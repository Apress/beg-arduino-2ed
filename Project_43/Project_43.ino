Project 43 – Temperature SD Datalogger

// DS1307 library by Henning Karlsen

#include <SD.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <DS1307.h> // written by Henningh Karlsen

#define ONE_WIRE_BUS 3
#define TEMPERATURE_PRECISION 12

File File1;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
DeviceAddress insideThermometer = { 0x28, 0x44, 0x12, 0xC2, 0x03, 0x00, 0x00, 0x92 };
DeviceAddress outsideThermometer = { 0x28, 0xA5, 0x02, 0xC2, 0x03, 0x00, 0x00, 0xF0 };
  
  float tempC, tempF;
  
  // Init the DS1307
DS1307 rtc(4, 5);

void setup() {
 Serial.println("Initializing the SD Card...");

  if (!SD.begin()) {
    Serial.println("Initialization Failed!");
    return;
  }
  Serial.println("Initialization Complete.\n");
  
  // Set the clock to run-mode
  rtc.halt(false);
  
  Serial.begin(9600);
  Serial.println("Type any character to start");
  while (!Serial.available());
  Serial.println();
  
  // Start up the sensors library
  sensors.begin(); 
  Serial.println("Initialising Sensors.\n");

  // set the resolution
  sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
  sensors.setResolution(outsideThermometer, TEMPERATURE_PRECISION);
  delay(100);
  
  // Set the time on the RTC.
  // Comment out this section if you have already set the time and have a battery backup
  // The following lines can be commented out to use the values already stored in the DS1307
  rtc.setDOW(TUESDAY);        // Set Day-of-Week to TUESDAY
  rtc.setTime(9, 27, 00);     // Set the time HH,MM,SS
  rtc.setDate(30, 04, 2013);   // Set the date DD,MM,YYYY 
}

void getTemperature(DeviceAddress deviceAddress)
{
  sensors.requestTemperatures();
  tempC = sensors.getTempC(deviceAddress);
  tempF = DallasTemperature::toFahrenheit(tempC);
}

void loop() {
    File1 = SD.open("TEMPLOG.txt", FILE_WRITE);
    Serial.println("File Opened.");
    if (File1) {
          File1.print(rtc.getDateStr());
          Serial.print(rtc.getDateStr());
          File1.print(", ");
          Serial.print(", ");
          File1.print(rtc.getTimeStr());
          Serial.print(rtc.getTimeStr());
          File1.print(":  Inside: "); 
          Serial.print(":  Inside: ");
          getTemperature(insideThermometer);
          File1.print(tempC);
          Serial.print(tempC);   
          File1.print("C  Outside: "); 
          Serial.print("C  Outside: ");
          getTemperature(outsideThermometer);
          File1.print(tempC); 
          Serial.print(tempC);
          File1.println(" C");  
          Serial.println(" C");  
          Serial.println("Data written to file.");
    }
    File1.close();
    Serial.println("File Closed.\n");
    Serial.println(“Safe to disconnect card”);
    delay(10000);
    Serial.println(“Card in use, do not disconnect!!”);
}

