//Project 48 — Email Alert System

#include <Ethernet.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define time 1000
#define emailInterval 60
#define HighThreshold 28
#define LowThreshold 10


// Data wire is plugged into pin 3 on the Arduino
#define ONE_WIRE_BUS 3
#define TEMPERATURE_PRECISION 12

float tempC, tempF;
char message1[35], message2[35];
char subject[] = "ARDUINO: TEMPERATURE ALERT!!\0";
unsigned long lastMessage;


// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

// arrays to hold device addresses
DeviceAddress insideThermometer = { 0x28, 0x44, 0x12, 0xC2, 0x03, 0x00, 0x00, 0x92 };

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xF7, 0x38 };
byte ip[] = { 192,168,0, 104 };
byte server[] = { 10, 254, 30, 60 };  // Mail server address. Change this to your own mail servers IP.

EthernetClient client;

void sendEmail(char subject[], char message1[], char message2[], float temp) {
  Serial.println("connecting...");
 
 if (client.connect(server, 25)) {
   Serial.println("connected");
   client.println("EHLO MYSERVER");  delay(time);
   client.println("AUTH LOGIN"); delay(time);
   client.println("lcbWNybbWl2JttnRzLmNvrZSbQ==");  delay(time); 
   client.println("GV0yVGbjLlnZ2VEw");  delay(time);
   client.println("MAIL FROM:<sales@earthshineelectronics.com>");      delay(time);
   client.println("RCPT TO:<fred@crunchytoad.com>");      delay(time);
   client.println("DATA");       delay(time);
   client.println("From: < sales@earthshineelectronics.com >");       delay(time);
   client.println("To: < fred@crunchytoad.com >");       delay(time);
   client.print("SUBJECT: ");
   client.println(subject);       delay(time);
   client.println();      delay(time);
   client.println(message1);      delay(time);
   client.println(message2);      delay(time);
   client.print("Temperature: ");
   client.println(temp);   delay(time);
   client.println(".");      delay(time);
   client.println("QUIT");      delay(time);
   Serial.println("Email sent.");
   lastMessage=millis();
 } else {
   Serial.println("connection failed");
 }

}

void checkEmail() {
  while (client.available()) {
   char c = client.read();
   Serial.print(c);
 }
 
 if (!client.connected()) {
   Serial.println();
   Serial.println("disconnecting.");
   client.stop();
 } 
}

// function to get the temperature for a device
void getTemperature(DeviceAddress deviceAddress)
{
  tempC = sensors.getTempC(deviceAddress);
  tempF = DallasTemperature::toFahrenheit(tempC);
} 

void setup()
{
 lastMessage = 0;
 Ethernet.begin(mac, ip);
 Serial.begin(9600);
 
     // Start up the sensors library
  sensors.begin();
    // set the resolution
  sensors.setResolution(insideThermometer, TEMPERATURE_PRECISION);

 delay(1000);
}

void loop()
{
  sensors.requestTemperatures();
  getTemperature(insideThermometer);
  Serial.println(tempC);
  if (tempC >= HighThreshold && (millis()>(lastMessage+(emailInterval*1000)))) {
    Serial.println("High Threshhold Exceeded");
    char message1[] = "Temperature Sensor\0";
    char message2[] = "High Threshold Exceeded\0";
    sendEmail(subject, message1, message2, tempC);
  }
  else if (tempC<= LowThreshold && (millis()>(lastMessage+(emailInterval*1000)))) {
    Serial.println("Low Threshhold Exceeded");
    char message1[] = "Temperature Sensor\0";
    char message2[] = "Low Threshold Exceeded\0";
    sendEmail(subject, message1, message2, tempC);
  }
       
    if (client.available()) {checkEmail();}
}

