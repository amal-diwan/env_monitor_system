#ifndef STATE_H
#define STATE_H

#include <Arduino.h>

struct SensorData {
  float humidity;
  float tempC;
  int lightVal;
  bool validSensor;
  bool hasData;
};

struct AlertData {
  bool ledOn;
  unsigned long currentBlink;

  bool buzzerOn;
  unsigned long currentBuzz;

  bool tempAlarm;
  bool humAlarm;
  bool lightAlarm;
};

// Thresholds
struct Threshold {
  float tempLimit;
  float humLimit;
  int lightLimit;
};


enum SystemMode {
  MODE_DISPLAY,
  MODE_EDIT_TEMP,
  MODE_EDIT_HUM,
  MODE_EDIT_LIGHT
};

enum AlertStatus {
  ALERT_NORMAL,
  ALERT_WARNING,
  ALERT_SENSOR_FAULT,
  ALERT_NO_DATA
};

extern SensorData systemSensors;
extern AlertData systemAlerts;
extern Threshold systemThresholds;
extern SystemMode currentMode;
extern AlertStatus currentAlert;
extern unsigned long now;

#endif