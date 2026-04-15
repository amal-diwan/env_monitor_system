#include "sensor.h"
#include "config.h"
#include "state.h"

#include <Arduino.h>
#include <DHT.h>

static DHT dht(DHTPin, DHTTYPE);

void sensorInit() {
  pinMode(lightPin, INPUT);
  dht.begin();
}

void readSensors() {
  systemSensors.humidity = dht.readHumidity();
  systemSensors.tempC = dht.readTemperature();
  systemSensors.lightVal = analogRead(lightPin);

  if (isnan(systemSensors.humidity) || isnan(systemSensors.tempC)) {
    systemSensors.validSensor = false;
    systemSensors.hasData = true;
  } else {
    systemSensors.validSensor = true;
    systemSensors.hasData = true;
  }
}