//Project 50 – RSS Weather Reader

// Thanks to  Bob S. for original code
// Get current weather observation for Edwards AFB from weather.gov in XML format

// Include description files for other libraries used (if any)
//#include <string.h>
#include <Ethernet.h>
#include <SPI.h>

// Max string length may have to be adjusted depending on data to be extracted
#define MAX_STRING_LEN  20

// Setup vars
char tagStr[MAX_STRING_LEN] = "";
char dataStr[MAX_STRING_LEN] = "";
char tmpStr[MAX_STRING_LEN] = "";
char endTag[3] = {'<', '/', '\0'};
int len;

// Flags to differentiate XML tags from document elements (ie. data)
boolean tagFlag = false;
boolean dataFlag = false;

// Ethernet vars
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0xF7, 0x38 };
byte ip[] = {192, 168, 0, 35};
byte server[] = { 140, 90, 113, 200 }; // www.weather.gov

// Start ethernet client
EthernetClient client;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting Weather RSS Reader");
  Serial.println("connecting...");
  Ethernet.begin(mac, ip);
  delay(1000);

  if (client.connect(server, 80)) {
    Serial.println("connected");
    Serial.println("Current weather at Edwards AFB:"); 
    client.println("GET /xml/current_obs/KEDW.xml HTTP/1.0");    
    client.println();
    delay(2000);
  } else {
    Serial.println("connection failed");
  }  
}

void loop() {

  // Read serial data in from web:
  while (client.available()) {
    serialEvent();
  }

  if (!client.connected()) {

    client.stop();

      for (int t=0; t<15; t++) {
      delay(60000); // 1 minute
      }
 

    if (client.connect(server, 80)) {   
      client.println("GET /xml/current_obs/KEDW.xml HTTP/1.0");    
      client.println();
      delay(2000);
    } else {
      Serial.println("Reconnection failed");
    }      
  }
}

// Process each char from web
void serialEvent() {
   // Read a char
	char inChar = client.read();
  
   if (inChar == '<') {
      addChar(inChar, tmpStr);
      tagFlag = true;
      dataFlag = false;

   } else if (inChar == '>') {
      addChar(inChar, tmpStr);

      if (tagFlag) {      
         strncpy(tagStr, tmpStr, strlen(tmpStr)+1);
      }

      // Clear tmp
      clearStr(tmpStr);

      tagFlag = false;
      dataFlag = true;      
      
   } else if (inChar != 10) {
      if (tagFlag) {
         // Add tag char to string
         addChar(inChar, tmpStr);

         // Check for </XML> end tag, ignore it
         if ( tagFlag && strcmp(tmpStr, endTag) == 0 ) {
            clearStr(tmpStr);
            tagFlag = false;
            dataFlag = false;
         }
      }
      
      if (dataFlag) {
         // Add data char to string
         addChar(inChar, dataStr);
      }
   }  
  
   // If a LF, process the line
   if (inChar == 10 ) {

      // Find specific tags and print data
      if (matchTag("<temp_f>")) {
	      Serial.print("TempF: ");
         Serial.print(dataStr);
      }
          if (matchTag("<temp_c>")) {
	      Serial.print(", TempC: ");
         Serial.print(dataStr);
      }
      if (matchTag("<relative_humidity>")) {
	      Serial.print(", Humidity: ");
         Serial.print(dataStr);
      }
      if (matchTag("<pressure_in>")) {
	      Serial.print(", Pressure: ");
         Serial.print(dataStr);
         Serial.println("");
      }

      // Clear all strings
      clearStr(tmpStr);
      clearStr(tagStr);
      clearStr(dataStr);

      // Clear Flags
      tagFlag = false;
      dataFlag = false;
   }
}

// Function to clear a string
void clearStr (char* str) {
   int len = strlen(str);
   for (int c = 0; c < len; c++) {
      str[c] = 0;
   }
}

//Function to add a char to a string and check its length
void addChar (char ch, char* str) {
   char *tagMsg  = "<TRUNCATED_TAG>";
   char *dataMsg = "-TRUNCATED_DATA-";

   // Check the max size of the string to make sure it doesn't grow too
   // big.  If string is beyond MAX_STRING_LEN assume it is unimportant
   // and replace it with a warning message.
   if (strlen(str) > MAX_STRING_LEN - 2) {
      if (tagFlag) {
         clearStr(tagStr);
         strcpy(tagStr,tagMsg);
      }
      if (dataFlag) {
         clearStr(dataStr);
         strcpy(dataStr,dataMsg);
      }

      // Clear the temp buffer and flags to stop current processing
      clearStr(tmpStr);
      tagFlag = false;
      dataFlag = false;

   } else {
      // Add char to string
      str[strlen(str)] = ch;
   }
}

// Function to check the current tag for a specific string
boolean matchTag (char* searchTag) {
   if ( strcmp(tagStr, searchTag) == 0 ) {
      return true;
   } else {
      return false;
   }
}

