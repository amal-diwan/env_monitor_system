#include "input.h"
#include "config.h"
#include "state.h"
#include "storage.h"

#include <Arduino.h>

struct Button {
  int pin;
  int lastRawState;
  int currentRawState;
  int stableState;
  unsigned long lastStateChange;
};

static bool buttonPressed(Button &b);
static void initButton(Button &b);
static void incThreshold();
static void decThreshold();
static void toggleMenu();

static Button menu;
static Button inc;
static Button dec;

void inputInit() {
  menu.pin = menuBut;
  inc.pin = incBut;
  dec.pin = decBut;

  initButton(menu);
  initButton(inc);
  initButton(dec);
}

void handleInput() {
  if (buttonPressed(menu)) {
    toggleMenu();
  }

  if (currentMode == MODE_EDIT_TEMP || currentMode == MODE_EDIT_HUM || currentMode == MODE_EDIT_LIGHT) {
    if (buttonPressed(inc)) {
      incThreshold();
    }
    if (buttonPressed(dec)) {
      decThreshold();
    }
  }
}

static void initButton(Button &b) {
  pinMode(b.pin, INPUT_PULLUP);
  int reading = digitalRead(b.pin);
  b.lastRawState = reading;
  b.currentRawState = reading;
  b.stableState = reading;
  b.lastStateChange = millis();
}

static bool buttonPressed(Button &b) {
  b.currentRawState = digitalRead(b.pin);

  if (b.currentRawState != b.lastRawState) {
    b.lastStateChange = now;
  }

  if (now - b.lastStateChange >= debounceDelay) {
    if (b.currentRawState != b.stableState) {
      b.stableState = b.currentRawState;
      if (b.stableState == LOW) {
        b.lastRawState = b.currentRawState;
        return true;
      }
    }
  }

  b.lastRawState = b.currentRawState;
  return false;
}

static void toggleMenu() {
  switch (currentMode) {
    case MODE_DISPLAY:
      currentMode = MODE_EDIT_TEMP;
      break;
    case MODE_EDIT_TEMP:
      currentMode = MODE_EDIT_HUM;
      break;
    case MODE_EDIT_HUM:
      currentMode = MODE_EDIT_LIGHT;
      break;
    case MODE_EDIT_LIGHT:
      saveThresholds();
      currentMode = MODE_DISPLAY;
      break;
  }
}

static void incThreshold() {
  switch (currentMode) {
    case MODE_EDIT_TEMP:
      if (systemThresholds.tempLimit < maxTempLimit) {
        systemThresholds.tempLimit = systemThresholds.tempLimit + 0.1;
      }
      break;
    case MODE_EDIT_HUM:
      if (systemThresholds.humLimit < maxHumLimit) {
        systemThresholds.humLimit = systemThresholds.humLimit + 0.1;
      }
      break;
    case MODE_EDIT_LIGHT:
      if (systemThresholds.lightLimit < maxLightLimit) {
        systemThresholds.lightLimit = systemThresholds.lightLimit + 1;
      }
      break;
  }
}

static void decThreshold() {
  switch (currentMode) {
    case MODE_EDIT_TEMP:
      if (systemThresholds.tempLimit > minTempLimit) {
        systemThresholds.tempLimit = systemThresholds.tempLimit - 0.1;
      }
      break;
    case MODE_EDIT_HUM:
      if (systemThresholds.humLimit > minHumLimit) {
        systemThresholds.humLimit = systemThresholds.humLimit - 0.1;
      }
      break;
    case MODE_EDIT_LIGHT:
      if (systemThresholds.lightLimit > minLightLimit) {
        systemThresholds.lightLimit = systemThresholds.lightLimit - 1;
      }
      break;
  }
}