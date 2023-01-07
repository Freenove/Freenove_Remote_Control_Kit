/*
 * Sketch     Remote
 * Platform   Freenove Remote Control (Compatible with Arduino/Genuino Uno)
 * Brief      This sketch is used to show how to use Freenove Remote Control.
 *            This sketch needs to be uploaded to the board of the remote.
 * Author     Ethan Pan @ Freenove (support@freenove.com)
 * Date       2023/01/07
 * Copyright  Copyright © Freenove (http://www.freenove.com)
 * License    Creative Commons Attribution ShareAlike 3.0
 *            (http://creativecommons.org/licenses/by-sa/3.0/legalcode)
 * -----------------------------------------------------------------------------------------------*/

#ifndef ARDUINO_AVR_UNO
#error Wrong board. Please choose "Arduino/Genuino Uno"
#endif

// wireless module
#include <SPI.h>
#include "RF24.h"
RF24 radio(9, 10);                // define the object to control wireless module
const byte addresses[6] = "Free1";// define communication address which should correspond to remote control
// wireless communication
int dataWrite[8];                 // define array used to save the write data
// pin
const int pot1Pin = A0,           // define POT1
          pot2Pin = A1,           // define POT2
          joystickXPin = A2,      // define pin for direction X of joystick
          joystickYPin = A3,      // define pin for direction Y of joystick
          joystickZPin = 7,       // define pin for direction Z of joystick
          s1Pin = 4,              // define pin for S1
          s2Pin = 3,              // define pin for S2
          s3Pin = 2,              // define pin for S3
          led1Pin = 6,            // define pin for LED1 which is close to POT1 and used to indicate the state of POT1
          led2Pin = 5,            // define pin for LED2 which is close to POT2 and used to indicate the state of POT2
          led3Pin = 8;            // define pin for LED3 which is close to wireless module and used to indicate the state of it

void setup() {
  // wireless module
  pinMode(12, INPUT_PULLUP);
  radio.begin();                        // initialize
  radio.setRetries(3, 15);              // set the number and delay of retries
  radio.setPALevel(RF24_PA_LOW, false); // set power amplifier (PA) level
  radio.setDataRate(RF24_1MBPS);        // set data rate through the air
  radio.openWritingPipe(addresses);     // open a pipe for writing
  radio.openReadingPipe(1, addresses);  // open a pipe for reading
  radio.stopListening();                // stop listening for incoming messages
  // pin
  pinMode(joystickZPin, INPUT);         // set led1Pin to input mode
  pinMode(s1Pin, INPUT);                // set s1Pin to input mode
  pinMode(s2Pin, INPUT);                // set s2Pin to input mode
  pinMode(s2Pin, INPUT);                // set s3Pin to input mode
  pinMode(led1Pin, OUTPUT);             // set led1Pin to output mode
  pinMode(led2Pin, OUTPUT);             // set led2Pin to output mode
  pinMode(led3Pin, OUTPUT);             // set led3Pin to output mode
}

void loop() {
  // put the values of rocker, switch and potentiometer into the array
  dataWrite[0] = analogRead(pot1Pin);
  dataWrite[1] = analogRead(pot2Pin);
  dataWrite[2] = analogRead(joystickXPin);
  dataWrite[3] = analogRead(joystickYPin);
  dataWrite[4] = digitalRead(joystickZPin);
  dataWrite[5] = digitalRead(s1Pin);
  dataWrite[6] = digitalRead(s2Pin);
  dataWrite[7] = digitalRead(s3Pin);

  // control time to write every 20ms
  static unsigned long lastRf24WriteMillis = 0;
  unsigned long millisNow = lastRf24WriteMillis;
  while (millisNow - lastRf24WriteMillis < 20)
    millisNow = millis();
  lastRf24WriteMillis = millisNow;
  digitalWrite(led3Pin, LOW);

  // write radio data
  if (radio.write(dataWrite, sizeof(dataWrite)))
    digitalWrite(led3Pin, HIGH);

  // make LED emit different brightness of light according to analog of potentiometer
  analogWrite(led1Pin, map(dataWrite[0], 0, 1023, 0, 255));
  analogWrite(led2Pin, map(dataWrite[1], 0, 1023, 0, 255));
}
