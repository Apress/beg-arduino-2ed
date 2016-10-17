//Project 47 — Internet Weather Display

// Project 47 - Based on the Xively Arduino examples

#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <HttpClient.h>
#include <Xively.h>

#define SHARE_FEED_ID              128497     // this is your xively feed ID that you want to share to
#define xivelyKey  "_WlvrG2Nw0RxPYKGGDMUNLqHDtl8sDcyXGSAKxvYU1ZtYz0g" // fill in your API key  

char sensorId1[] = "IntTempC";
char sensorId2[] = "IntTempF";
char sensorId3[] = "ExtTempC";
char sensorId4[] = "ExtTempF";
XivelyDatastream datastreams[] = {
  XivelyDatastream(sensorId1, strlen(sensorId1), DATASTREAM_FLOAT),
  XivelyDatastream(sensorId2, strlen(sensorId2), DATASTREAM_FLOAT),
  XivelyDatastream(sensorId3, strlen(sensorId3), DATASTREAM_FLOAT),
  XivelyDatastream(sensorId4, strlen(sensorId4), DATASTREAM_FLOAT)
};
// Finally, wrap the datastreams into a feed
XivelyFeed feed(SHARE_FEED_ID, datastreams, 4 /* number of datastreams */);

// Data wire is plugged into pin 3 on the Arduino
#define ONE_WIRE_BUS 3
#define TEMPERATURE_PRECISION 12

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
DeviceAddress insideThermometer = { 0x28, 0x44, 0x12, 0xC2, 0x03, 0x00, 0x00, 0x92 };
DeviceAddress outsideThermometer = { 0x28, 0xA5, 0x02, 0xC2, 0x03, 0x00, 0x00, 0xF0 };

unsigned int interval;
float itempC, itempF, etempC, etempF;

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xF7, 0x38 }; // make sure this is unique on your network
                       
EthernetClient localClient;
XivelyClient xivelyclient(localClient);

void setup()
{
  Serial.begin(9600); 
  Serial.println("Starting 4 stream data upload to Xively...");
  Serial.println();
  while (Ethernet.begin(mac) != 1)
  {
    Serial.println("Error getting IP address via DHCP, trying again...");
    delay(15000);
  }
      // Start up the sensors library
  sensors.begin();
    // set the resolution
  sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
  sensors.setResolution(outsideThermometer, TEMPERATURE_PRECISION);
  delay(100);
}

void xively_in_out() {
  getTemperatures();
  datastreams[0].setFloat(itempC);
  datastreams[1].setFloat(itempF);
  datastreams[2].setFloat(etempC);
  datastreams[3].setFloat(etempF);
  xivelyclient.put(feed, xivelyKey);
  Serial.println("Read sensor value ");
  Serial.println(datastreams[0].getFloat());
  Serial.println(datastreams[1].getFloat());
  Serial.println(datastreams[2].getFloat());
  Serial.println(datastreams[3].getFloat());
  delay(10000);
}

// function to get the temperature for a device
void getTemperatures()
{
  sensors.requestTemperatures();
  itempC = sensors.getTempC(insideThermometer);
  itempF = DallasTemperature::toFahrenheit(itempC);
  etempC = sensors.getTempC(outsideThermometer);
  etempF = DallasTemperature::toFahrenheit(etempC);
}

void loop()
{
  xively_in_out();
}

