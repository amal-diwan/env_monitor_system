#include "serial_cmd.h"
#include "config.h"
#include "state.h"
#include "storage.h"

#include <Arduino.h>

static void processCommand(String c);
static void get();
static void save();
static void defaults();
static void status();
static void printMode(SystemMode s);
static void printAlert(AlertStatus a);
static void editTemp(float t);
static void editHum(float h);
static void editLight(int l);

void serialCmdInit() {
  Serial.println("List of Commands");
  Serial.println("1) get: Displays System Thresholds");
  Serial.println("2) save: Saves System Thresholds to Memory");
  Serial.println("3) defaults: Updates System Thresholds to Default");
  Serial.println("4) status: Displays System Data");
  Serial.println("5) set temp x: Updates Temperature Threshold to x");
  Serial.println("6) set hum x: Updates Humidity Threshold to x");
  Serial.println("7) set light x: Updates Light Threshold to x");
}

void handleSerial() {
  if (Serial.available() <= 0) {
    return;
  }

  String command = Serial.readStringUntil('\n');
  command.trim();

  if (command.length() == 0) {
    Serial.println("Empty command");
  } else {
    processCommand(command);
  }
}

static void processCommand(String c) {
  if (c == "get") {
    get();
  } else if (c == "save") {
    save();
  } else if (c == "defaults") {
    defaults();
  } else if (c == "status") {
    status();
  } else if (c.startsWith("set temp ")) {
    String value = c.substring(9);
    float temp = value.toFloat();
    editTemp(temp);
  } else if (c.startsWith("set hum ")) {
    String value = c.substring(8);
    float hum = value.toFloat();
    editHum(hum);
  } else if (c.startsWith("set light ")) {
    String value = c.substring(10);
    int light = value.toInt();
    editLight(light);
  } else {
    Serial.println("Unknown command");
  }
}

static void get() {
  Serial.println("Thresholds");
  Serial.print("Temperature: ");
  Serial.print(systemThresholds.tempLimit, 1);
  Serial.println("C");
  Serial.print("Humidity: ");
  Serial.print(systemThresholds.humLimit, 1);
  Serial.println("%");
  Serial.print("Light: ");
  Serial.println(systemThresholds.lightLimit);
}

static void save() {
  saveThresholds();
  Serial.println("Thresholds saved.");
}

static void defaults() {
  setDefaultThresholds();
  Serial.println("Thresholds restored to defaults.");
  get();
}

static void status() {
  Serial.print("System Mode: ");
  printMode(currentMode);

  Serial.print("Alert Status: ");
  printAlert(currentAlert);

  Serial.println("Sensor Data");
  Serial.print("Temperature: ");
  Serial.print(systemSensors.tempC, 1);
  Serial.println("C");
  Serial.print("Humidity: ");
  Serial.print(systemSensors.humidity, 1);
  Serial.println("%");
  Serial.print("Light: ");
  Serial.println(systemSensors.lightVal);

  Serial.println();
  Serial.println("Thresholds");
  Serial.print("Temperature: ");
  Serial.print(systemThresholds.tempLimit, 1);
  Serial.println("C");
  Serial.print("Humidity: ");
  Serial.print(systemThresholds.humLimit, 1);
  Serial.println("%");
  Serial.print("Light: ");
  Serial.println(systemThresholds.lightLimit);
}

static void printMode(SystemMode s) {
  switch (s) {
    case MODE_DISPLAY:
      Serial.println("DISPLAY");
      break;
    case MODE_EDIT_TEMP:
      Serial.println("EDIT TEMPERATURE");
      break;
    case MODE_EDIT_HUM:
      Serial.println("EDIT HUMIDITY");
      break;
    case MODE_EDIT_LIGHT:
      Serial.println("EDIT LIGHT");
      break;
    default:
      Serial.println("DISPLAY");
      break;
  }
}

static void printAlert(AlertStatus a) {
  switch (a) {
    case ALERT_NORMAL:
      Serial.println("NORMAL");
      break;
    case ALERT_WARNING:
      Serial.println("WARNING");
      break;
    case ALERT_SENSOR_FAULT:
      Serial.println("SENSOR FAULT");
      break;
    case ALERT_NO_DATA:
      Serial.println("NO DATA");
      break;
    default:
      Serial.println("NORMAL");
      break;
  }
}

static void editTemp(float t) {
  if (t >= minTempLimit && t <= maxTempLimit) {
    systemThresholds.tempLimit = t;
    Serial.println("Temperature threshold updated.");
  } else {
    Serial.println("Temperature threshold exceeds limits.");
  }
  get();
}

static void editHum(float h) {
  if (h >= minHumLimit && h <= maxHumLimit) {
    systemThresholds.humLimit = h;
    Serial.println("Humidity threshold updated.");
  } else {
    Serial.println("Humidity threshold exceeds limits.");
  }
  get();
}

static void editLight(int l) {
  if (l >= minLightLimit && l <= maxLightLimit) {
    systemThresholds.lightLimit = l;
    Serial.println("Light threshold updated.");
  } else {
    Serial.println("Light threshold exceeds limits.");
  }
  get();
}