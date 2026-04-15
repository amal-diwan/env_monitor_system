#include "alert.h"
#include "config.h"
#include "state.h"

#include <Arduino.h>

static void updateAlarms();
static void checkAlertStatus();
static void triggerAlerts();



void alertInit() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzPin, OUTPUT);

  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
  noTone(buzzPin);
}

void updateAlerts() {
  checkAlertStatus();
  triggerAlerts();
}

static void updateAlarms() {
  if (!systemAlerts.tempAlarm) {
    if (systemSensors.tempC >= systemThresholds.tempLimit) {
      systemAlerts.tempAlarm = true;
    }
  } else {
    if (systemSensors.tempC <= systemThresholds.tempLimit - tempHyst) {
      systemAlerts.tempAlarm = false;
    }
  }

  if (!systemAlerts.humAlarm) {
    if (systemSensors.humidity >= systemThresholds.humLimit) {
      systemAlerts.humAlarm = true;
    }
  } else {
    if (systemSensors.humidity <= systemThresholds.humLimit - humHyst) {
      systemAlerts.humAlarm = false;
    }
  }

  if (!systemAlerts.lightAlarm) {
    if (systemSensors.lightVal <= systemThresholds.lightLimit) {
      systemAlerts.lightAlarm = true;
    }
  } else {
    if (systemSensors.lightVal >= systemThresholds.lightLimit + lightHyst) {
      systemAlerts.lightAlarm = false;
    }
  }
}

static void checkAlertStatus() {
  if (systemSensors.hasData) {
    if (systemSensors.validSensor) {
      updateAlarms();

      bool anyAlarm = systemAlerts.tempAlarm || systemAlerts.humAlarm || systemAlerts.lightAlarm;

      if (!anyAlarm) {
        currentAlert = ALERT_NORMAL;
      }
      else {
        currentAlert = ALERT_WARNING;
      }
    }
    else {
      systemAlerts.tempAlarm = false;
      systemAlerts.humAlarm = false;
      systemAlerts.lightAlarm = false;
      currentAlert = ALERT_SENSOR_FAULT;
    }
  }
  else {
    systemAlerts.tempAlarm = false;
    systemAlerts.humAlarm = false;
    systemAlerts.lightAlarm = false;
    currentAlert = ALERT_NO_DATA;
  }
}

static void triggerAlerts() {
  switch (currentAlert) {
    case ALERT_NORMAL: 
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, HIGH);
      digitalWrite(bluePin, LOW);
      systemAlerts.ledOn = false;
      systemAlerts.currentBlink = now;

      noTone(buzzPin);
      systemAlerts.buzzerOn= false;
      systemAlerts.currentBuzz = now;

      break;
    case ALERT_WARNING:
      digitalWrite(redPin, HIGH);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
      systemAlerts.ledOn = false;
      systemAlerts.currentBlink = now;

      if (!systemAlerts.buzzerOn) {
        tone(buzzPin, 1000);
        systemAlerts.currentBuzz = now;
        systemAlerts.buzzerOn = true;
      }

      if (now - systemAlerts.currentBuzz >= buzzInterval) {
        noTone(buzzPin);
        systemAlerts.buzzerOn = false;
      }

      break;
    case ALERT_SENSOR_FAULT:
      if (now - systemAlerts.currentBlink >= blinkInterval) {
        systemAlerts.ledOn = !systemAlerts.ledOn;
        systemAlerts.currentBlink = now;
      }

      if (systemAlerts.ledOn) {
        digitalWrite(redPin, HIGH);
        digitalWrite(greenPin, HIGH);
        digitalWrite(bluePin, LOW);
      }
      else {
        digitalWrite(redPin, LOW);
        digitalWrite(greenPin, LOW);
        digitalWrite(bluePin, LOW);
      }
      
      noTone(buzzPin);
      systemAlerts.buzzerOn = false;
      systemAlerts.currentBuzz = now;

      break;
    case ALERT_NO_DATA:
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
      systemAlerts.ledOn = false;
      systemAlerts.currentBlink = now;

      noTone(buzzPin);
      systemAlerts.buzzerOn = false;
      systemAlerts.currentBuzz = now;

      break;
    default:
      digitalWrite(redPin, LOW);
      digitalWrite(greenPin, LOW);
      digitalWrite(bluePin, LOW);
      systemAlerts.ledOn = false;
      systemAlerts.currentBlink = now;

      noTone(buzzPin);
      systemAlerts.buzzerOn = false;
      systemAlerts.currentBuzz = now;

      break;
  }
}