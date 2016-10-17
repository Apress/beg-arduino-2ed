Project 42 – Simple SD Card/Read Write

#include <SD.h>

File File1;

void setup()
{
  Serial.begin(9600);
   while (!Serial) { } // wait for serial port to connect. 
                       // Needed for Leonardo only

  Serial.println("Initializing the SD Card...");

  if (!SD.begin()) {
    Serial.println("Initialization Failed!");
    return;
  }
  Serial.println("Initialization Complete.\n");

  Serial.println("Looking for file 'testfile.txt'...\n");

  if (SD.exists("testfile.txt")) {
    Serial.println("testfile.txt already exists.\n");
  }
  else {
    Serial.println("testfile.txt doesn't exist.");
    Serial.println("Creating file testfile.txt...\n");
  }

  File1 = SD.open("testfile.txt", FILE_WRITE);
  File1.close();

  
  Serial.println("Writing text to file.....");
  String dataString;
  File1 = SD.open("testfile.txt", FILE_WRITE);
  if (File1) {
      for (int i=1; i<11; i++) {
        dataString = "Test Line ";
        dataString += i;
        File1.println(dataString);
      }
      Serial.println("Text written to file.....\n");
  }
  File1.close();
  
  Serial.println("Reading contents of textfile.txt...");
  File1 = SD.open("testfile.txt");
  
   if (File1) {
    while (File1.available()) {
      Serial.write(File1.read());
    }
    File1.close();
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening testfile.txt");
  } 

  // delete the file:
  Serial.println("\nDeleting testfile.txt...\n");
  SD.remove("testfile.txt");

  if (SD.exists("testfile.txt")){ 
    Serial.println("testfile.txt still exists.");
  }
  else {
    Serial.println("testfile.txt has been deleted.");  
  }
}

void loop()
{
  // Nothing to see here
}

