//Project 49 — Twitterbot

#include <Ethernet.h>
#include <Twitter.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>

// Data wire is plugged into pin 3 on the Arduino
#define ONE_WIRE_BUS 3
#define TEMPERATURE_PRECISION 12

float itempC, itempF, etempC, etempF;
boolean firstTweet = true;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses

DeviceAddress insideThermometer = { 0x28, 0x44, 0x12, 0xC2, 0x03, 0x00, 0x00, 0x92 };
DeviceAddress outsideThermometer = { 0x28, 0xA5, 0x02, 0xC2, 0x03, 0x00, 0x00, 0xF0 };

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xF7, 0x38 };

// Your Token to Tweet (get it from http://arduino-tweet.appspot.com/)
Twitter twitter("19735326-neITsUBnLTZHgN9UGaRkcGvAe9vYuaRP7E55K26J"); // DuinoBot

unsigned long interval = 600000; // 10 minutes
unsigned long lastTime; // time since last tweet

// Message to post
char message[140], serialString[60];

// function to get the temperature for a device
void getTemperatures()
{
  itempC = sensors.getTempC(insideThermometer);
  itempF = DallasTemperature::toFahrenheit(itempC);
  etempC = sensors.getTempC(outsideThermometer);
  etempF = DallasTemperature::toFahrenheit(etempC);
} 

void tweet(char msg[]) {
   Serial.println("connecting ...");
  if (twitter.post(msg)) {
    int status = twitter.wait();
    if (status == 200) {
      Serial.println("OK. Tweet sent.");
      Serial.println();
      lastTime = millis();
      firstTweet = false;
    } else {
      Serial.print("failed : code ");
      Serial.println(status);
    }
  } else {
    Serial.println("connection failed.");
  }
}

void setup()
{
  Ethernet.begin(mac);
  Serial.begin(9600);
   sensors.begin();
    // set the resolution
  sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);
  sensors.setResolution(outsideThermometer, TEMPERATURE_PRECISION);
  sensors.requestTemperatures();

  getTemperatures();

  while (firstTweet) { 
    sprintf(message, "Int. Temp: %d C (%d F) Ext. Temp: %d C (%d F). Tweeted from Arduino. %ld", int(itempC), int(itempF), int(etempC), int(etempF), millis());
    tweet(message); 
  }
}

void loop()
{
  Ethernet.maintain();
  sensors.requestTemperatures();
  sprintf(serialString, "Internal Temp: %d C  %d F. External Temp: %d C %d F", int(itempC), int(itempF), int(etempC), int(etempF));
  delay(500);
  Serial.println(serialString);
  Serial.println();
  
  if (millis() >= (lastTime + interval)) {
    sprintf(message, "Int. Temp: %d C (%d F) Ext. Temp: %d C (%d F). Tweeted from Arduino. %ld", int(itempC), int(itempF), int(etempC), int(etempF), millis());
       delay(500);
      tweet(message);
    }
    
  delay(10000); // 10 seconds
}

