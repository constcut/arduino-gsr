#include <Wire.h>
#include "DS1307.h"

#include <SPI.h>
#include <SD.h>

const int GSR1 = A0;
const int GSR2 = A3;

const int BUTTON1 = 3;
const int BUTTON2 = 2;
const int VIBRO = 8;
const int SDCARD = 4;

DS1307 clock;

void loop(void) {

  unsigned long startEpoch = millis();
 
  int sensorValue1 = analogRead(GSR1);
  int sensorValue2 = analogRead(GSR2);
    
  int ECG = 0; //Hope for a better future, not to change CSV format
  
  Serial.println(sensorValue1);
  Serial.print(",");
  Serial.println(sensorValue2);

  int button1State = digitalRead(BUTTON1);
  int button2State = digitalRead(BUTTON2);
  int buttonState = 0;

  if (button1State == HIGH)
    buttonState |= 1;
  if (button2State == HIGH)
    buttonState |= 2;
  
  File logFile1 = SD.open("gsr.csv", FILE_WRITE);
  if (logFile1) {
    String cvs = String(startEpoch) + "," + String(sensorValue1) + "," + String(sensorValue2) + "," + String(ECG) + "," + String(buttonState);
    logFile1.println(cvs); 
    logFile1.close();
  }
  else { 
    Serial.println("Failed to open file");
  }
  
}


void setup()
{
  Serial.begin(9600);
  while (!Serial) {;
  }
  clock.begin(); 
  
  pinMode(VIBRO, OUTPUT); 
  pinMode(BUTTON1, INPUT); 
  pinMode(BUTTON2, INPUT); 

  pinMode(53, OUTPUT);    

  Serial.println("SD:");
  if (!SD.begin(SDCARD)) { 
    Serial.println("initialization on 4 failed!");
    return;
  }
  
  Sd2Card card;
  if (!card.init(SPI_HALF_SPEED, SDCARD)) {
    Serial.println("SD_CARD_INIT_FAILED!");
    return;
  } else {
    Serial.println("SD_CARD_INITED.");
  }

  char currentTime[20];
  clock.getTime();
  File logFile = SD.open("gsr.csv", FILE_WRITE);
  if (logFile) { 
    sprintf(currentTime, "%02d.%02d.%02d-%02d_%02d_%02d",  clock.dayOfMonth, clock.month, clock.year, clock.hour, clock.minute, clock.second); 
    String ct = currentTime;
    logFile.println("Session started:");
    logFile.println(ct); 
    logFile.close();
  }
  else { 
    Serial.println("Failed_open_on_start!");
  }
}
