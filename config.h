#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

#define DHTTYPE DHT11

// LCD pins
const int rs = 7;
const int en = 8;
const int d4 = 9;
const int d5 = 10;
const int d6 = 11;
const int d7 = 12;

// DHT
const int DHTPin = 6;

// Light
const int lightPin = A0;

// RGB LED
const int redPin = A1;
const int greenPin = A2;
const int bluePin = A3;

// Buzzer
const int buzzPin = 5;

//Buttons
const int decBut = 2;
const int menuBut = 3;
const int incBut = 4;


// Timing
const unsigned long sensorInterval = 1000;
const unsigned long lcdInterval = 300;
const unsigned long alertInterval = 50;
const unsigned long inputInterval = 20;
const unsigned long serialInterval = 50;

const unsigned long blinkInterval = 100;
const unsigned long buzzInterval = 100;

const unsigned long debounceDelay = 30;

const float tempHyst = 1.0;
const float humHyst = 5.0;
const int lightHyst = 20;

const float defaultTempLimit = 25.0;
const float defaultHumLimit = 70.0;
const int defaultLightLimit = 300;

const float maxTempLimit = 70.0;
const float maxHumLimit = 100.0;
const int maxLightLimit = 1023;

const float minTempLimit = 0.0;
const float minHumLimit = 0.0;
const int minLightLimit = 0;

#endif