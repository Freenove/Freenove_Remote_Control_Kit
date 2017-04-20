/*
 * Sketch     Project
 * Platform   Arduino Based Products/Projects
 * Brief      This sketch is used to show how to use Freenove Remote Control.
 *            This sketch needs to be uploaded to the Arduino based products/projects.
 * Author     Ethan Pan @ Freenove (support@freenove.com)
 * Date       2017/4/8
 * Copyright  Copyright © Freenove (http://www.freenove.com)
 * License    Creative Commons Attribution ShareAlike 3.0
 *            (http://creativecommons.org/licenses/by-sa/3.0/legalcode)
 * -----------------------------------------------------------------------------------------------*/

// NRF24L01
#include <SPI.h>
#include "RF24.h"
RF24 radio(9, 10);                // define the object to control NRF24L01
const byte addresses[6] = "Free1";// define communication address which should correspond to remote control
// wireless communication
int dataRead[8];                  // define array used to save the read data

void setup() {
  // NRF24L01
  radio.begin();                      // initialize RF24
  radio.setPALevel(RF24_PA_LOW);      // set power amplifier (PA) level
  radio.setDataRate(RF24_1MBPS);      // set data rate through the air
  radio.setRetries(0, 15);            // set the number and delay of retries
  radio.openWritingPipe(addresses);   // open a pipe for writing
  radio.openReadingPipe(1, addresses);// open a pipe for reading
  radio.startListening();             // start monitoringtart listening on the pipes opened
  // serial port
  Serial.begin(115200);              // initialize serial
}

void loop()
{
  // read radio data
  if ( radio.available()) {             // if receive the data
    radio.read(dataRead, sizeof(dataRead));   // read data
    showInfo();
  }
}

void showInfo() {
  Serial.print("Pot1: ");   Serial.print(dataRead[0]);
  Serial.print(", Pot2: "); Serial.print(dataRead[1]);

  Serial.print(", X: ");    Serial.print(dataRead[2]);
  Serial.print(", Y: ");    Serial.print(dataRead[3]);
  Serial.print(", Z: ");    Serial.print(dataRead[4]);

  Serial.print(", S1: ");   Serial.print(dataRead[5]);
  Serial.print(", S2: ");   Serial.print(dataRead[6]);
  Serial.print(", S3: ");   Serial.print(dataRead[7]);

  Serial.println("");
}

