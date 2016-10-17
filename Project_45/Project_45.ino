//Project 45 — Access Control System

#define lockPin 7
#define speakerPin 9
#define tx 3
#define rx 2
#define unlockLength 2000

#include <SoftwareSerial.h>

SoftwareSerial rfidReader = SoftwareSerial(rx, tx);

int users = 3;

char* cards[] = { // valid cards
  "3D00768B53",
  "3D00251C27",
  "3D0029E6BF", 
};

char* names[] = { // cardholder names
  "Tom Smith",
  "Dick Jones",
  "Harry Roberts"
};

void setup() {
  pinMode (lockPin, OUTPUT); 
  pinMode (speakerPin, OUTPUT);
  digitalWrite(lockPin, LOW);
  Serial.begin(9600);
  rfidReader.begin(9600);
}

void loop() {
  char cardNum[10]; // array to hold card number
  byte cardBytes[6]; // byte version of card number + checksum
  int index=0; // current digit
  byte byteIn=0; // byte read from RFID
  byte lastByte=0; // the last byte read
  byte checksum = 0; // checksum result stored here
  
  if (rfidReader.read()==2) { // read the RFID reader
      while(index<12) { // 12 digits in unique serial number
          byteIn = rfidReader.read(); // store value in byteIn
          if ((byteIn==1) || (byteIn==2) || (byteIn==10) || (byteIn==13)) {return;}
 // if STX, ETX, CR or LF break
          if (index<10) {cardNum[index]=byteIn;} // store first 10 HEX digits only (last 2 are checksum)
          // convert ascii hex to integer hex value
          if ((byteIn>='0') && (byteIn<='9')) {
          byteIn -= '0';
          } 
          else if ((byteIn>='A') && (byteIn<='F')) {
          byteIn = (byteIn+10)-'A';
          }
          if ((index & 1) == 1) { // if odd number merge 2 4 bit digits into 8 bit byte
// move the last digit 4 bits left and add new digit
cardBytes[index/2]= (byteIn | (lastByte<<4)); 
              	if (index<10) {checksum ^= cardBytes[index/2];} // tot up the checksum value 
          }
          lastByte=byteIn; // store the last byte read
          index++; // increment the index
  // if we have reached the end of all digits add a null terminator
          if (index==12) {cardNum[10] = '\0';} 
    }
   
   Serial.println(cardNum); // print the card number
   int cardIndex =checkCard(cardNum); // check if card is valid and return index number
   if(cardIndex>=0 && (cardBytes[5]==checksum)) { // if card number and checksum are valid
        Serial.println("Card Validated");
        Serial.print("User: ");
        Serial.println(names[cardIndex]); // print the relevant name
        unlock(); // unlock the door
        Serial.println();
    }
    else {
        Serial.println("Card INVALID");
        tone(speakerPin, 250, 250);
        delay(250);
        tone(speakerPin, 150, 250);
        Serial.println();
    }
  }
}

// Check the detected card against all known to be valid card numbers
// Return the array index number of a matched card number
// or a negative value to indicate a non matching card number
int checkCard(char cardNum[10]) 
{
   for (int x=0; x<=users; x++) 
   {
      if(strcmp(cardNum, cards[x])==0) 
      {
         return (x);
      }
   }
   return (-1);
}

void unlock() {
    tone(speakerPin, 1000, 500);
    digitalWrite(lockPin, HIGH);
    delay(unlockLength);
    digitalWrite(lockPin, LOW);
}

