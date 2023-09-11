// citeste tagurile si le salveaza o singura data pe cardul sd

// Date and time functions using a DS1307 RTC connected via I2C and Wire lib
//Pini configurati
//SDA - A4 RTC
//SCL - A5 RTC
// // SCK- pin 13; MOSI - 11; MOSO - 12; CS - 4;  -- Card SD
//RX, TX  --> TX M6e nano pe 2 ( fir galben);

#include "RTClib.h"                    // library for the timmer that we use
#include <SoftwareSerial.h>            // library to handle the comunnication between arduino and the rfid module
#include <SD.h>                        // library for the SD card
#include "SparkFun_UHF_RFID_Reader.h"  //Library for controlling the M6E Nano module


#define PIN_SPI_CS 53               // pins for the SD card reader
SoftwareSerial softSerial(10, 11);  // pins for the RFID module RX and TX

#define DEBUG 0

#define BUZZER1 8
//#define BUZZER1 0 //For testing quietly
#define BUZZER2 9

// define pins for the L298N, H birdge
int ena = 2;
int in1 = 3;
int in2 = 4;
int in3 = 5;
int in4 = 6;
const long irPin = 7;


File myFile;
RFID nano;  //Create instance
RTC_DS1307 rtc;

char store_tags[9];      // vector to store each id of the tag, and after to search in it.
int number_of_tags = 0;  // number of tags
unsigned long previousMillis = 0;
unsigned long pausetime = 3;  // the time in seconds that the rfid reader will wait between 2 scans
bool check_one = 1;           // varibila pentru a scrie o singura data println in fisier
bool check_one_date = 1;      // varibila pentru a scrie o singura data pe linie data si timpul;

void setup() {

  for (byte i = 0; i <= 9; i++) {
    store_tags[i] = 0x0;  // set up the vector that we will use to store the id of the tags
  }

  Serial.begin(115200);

  pinMode(BUZZER1, OUTPUT);
  pinMode(BUZZER2, OUTPUT);

  digitalWrite(BUZZER2, LOW); //Pull half the buzzer to ground and drive the other half.


  if (setupNano(38400) == false)  //Configure nano to run at 38400bps
  {
    Serial.println(F("Module failed to respond. Please check wiring."));
    while (1)
      ;  //Freeze!
  }

  nano.setRegion(REGION_NORTHAMERICA);  //Set to North America

  nano.setReadPower(2000);  //5.00 dBm. Higher values may caues USB port to brown out
  //Max Read TX Power is 27.00 dBm and may cause temperature-limit throttling


  if (!rtc.begin()) {  // star the RTC clock, before the scanning starts
    Serial.println(F("Couldn't find RTC"));
    Serial.flush();
    while (1) delay(10);
  }

  if (!rtc.isrunning()) {
    Serial.println(F("RTC is NOT running, let's set the time!"));
    rtc.adjust(DateTime(F(_DATE), F(__TIME_)));
  }

  nano.startReading();  //Begin scanning for tags
}

void loop() {
  unsigned long currentSeconds = millis() / 1000;
  if (currentSeconds % pausetime == 0) {
    bool flag = 0;  // see if the id is allready in the store vector;

    //tone(BUZZER1, 500, 1); //C

    if (nano.check() == true)  //Check to see if any new data has come in from module
    {
      check_one = 0;
      byte responseType = nano.parseResponse();  //Break response into tag ID, RSSI

      if (responseType == RESPONSE_IS_KEEPALIVE) {
        Serial.println(F("Scanning"));
      } else if (responseType == RESPONSE_IS_TAGFOUND) {
        //If we have a full record we can pull out the fun bits

        byte tagEPCBytes = nano.getTagEPCBytes();  //Get the number of bytes of EPC from response

        //Print EPC bytes, this is a subsection of bytes from the response/msg array
        char unint_to_text[tagEPCBytes - 4];

        for (byte x = 0; x < tagEPCBytes - 4; x++) {
          if (nano.msg[31 + x] < 0x10) Serial.print(F("0"));  //Pretty print

          unint_to_text[x] = char(nano.msg[31 + x]);  // conversie to char form unint_8 pentru fiecare tag citit
          Serial.print(unint_to_text[x]);
        }
        Serial.print(F("]"));
        Serial.println();

        if (!SD.begin(PIN_SPI_CS)) {
          Serial.println(F("SD CARD FAILED, OR NOT PRESENT!"));
          while (1)
            ;  // don't do anything more
        }

        Serial.println(F("SD CARD INITIALIZED."));

        for (uint8_t i = 0; i <= number_of_tags; i++) {
          if (store_tags[i] == unint_to_text[0]) {  //compara fiecare element din stored tags cu primul element, adica id-ul tagului pe care l-am citit
            flag = 1;                               // flag to see if we have allready stored the id value in the vector
          }
        }
        if (flag == 0) {
          store_tags[number_of_tags] = unint_to_text[0];  // vector in each we store the vale of the id that we have read
          number_of_tags++;                               // increase the value of each tag
          tone(BUZZER1, 500, 100); 
           
          // open file for writing
          myFile = SD.open("cos.txt", FILE_WRITE);  // opne file to write only a single time per line the date and time;
          if (check_one_date == 1) {                // writes olny one the on the row the date and hours in the text file like that(date, hour, id, id..)
            check_one_date = 0;
            if (myFile) {
              DateTime now = rtc.now();
              myFile.print(now.year(), DEC);
              myFile.print('/');
              myFile.print(now.month(), DEC);
              myFile.print('/');
              myFile.print(now.day(), DEC);
              myFile.print(",");
              myFile.print(now.hour(), DEC);
              myFile.print(':');
              myFile.print(now.minute(), DEC);
              myFile.print(':');
              myFile.print(now.second(), DEC);
            }
            myFile.close();
          }
          myFile = SD.open("cos.txt", FILE_WRITE);
          if (myFile) {
            myFile.print(",");
            myFile.print((char)unint_to_text[0]);  // print the id of each tag like that(id, id, id) on a text file
          }
          myFile.close();
        }

      } else if (responseType == ERROR_CORRUPT_RESPONSE) {
        Serial.println(F("Bad CRC"));  // the rfid reader transmits a unknown message
      } else {
        //Unknown response
        Serial.print(F("Unknown error"));
      }
    }
  } else {  // golim vectorul in care avem stocate id-urile de la cititrea anterioara
    for (uint8_t i = 0; i <= number_of_tags; i++) {
      store_tags[i] = 0;  // reseteaza vectorul in care am avut stocate id-urile la citirea precedenta
    }

    if (check_one == 0) {  // flag used to write on a new line each list of id
      check_one_date = 1;
      check_one = 1;
      myFile = SD.open("cos.txt", FILE_WRITE);
      if (myFile) {
        myFile.println();
      }
      myFile.close();
    }
  }
  
}


//Gracefully handles a reader that is already configured and already reading continuously
//Because Stream does not have a .begin() we have to do this outside the library
boolean setupNano(long baudRate) {
  nano.begin(softSerial);  //Tell the library to communicate over software serial port

  //Test to see if we are already connected to a module
  //This would be the case if the Arduino has been reprogrammed and the module has stayed powered
  softSerial.begin(baudRate);  //For this test, assume module is already at our desired baud rate
  while (softSerial.isListening() == false)
    ;  //Wait for port to open

  //About 200ms from power on the module will send its firmware version at 115200. We need to ignore this.
  while (softSerial.available()) softSerial.read();

  nano.getVersion();

  if (nano.msg[0] == ERROR_WRONG_OPCODE_RESPONSE) {
    //This happens if the baud rate is correct but the module is doing a ccontinuous read
    nano.stopReading();

    Serial.println(F("Module continuously reading. Asking it to stop..."));

    delay(1500);
  } else {
    //The module did not respond so assume it's just been powered on and communicating at 115200bps
    softSerial.begin(115200);  //Start software serial at 115200

    nano.setBaud(baudRate);  //Tell the module to go to the chosen baud rate. Ignore the response msg

    softSerial.begin(baudRate);  //Start the software serial port, this time at user's chosen baud rate

    delay(250);
  }

  //Test the connection
  nano.getVersion();
  if (nano.msg[0] != ALL_GOOD) return (false);  //Something is not right

  //The M6E has these settings no matter what
  nano.setTagProtocol();  //Set protocol to GEN2

  nano.setAntennaPort();  //Set TX/RX antenna ports to 1

  return (true);  //We are ready
}