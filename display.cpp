#include "display.h"
#include "config.h"
#include "state.h"

#include <Arduino.h>
#include <LiquidCrystal.h>

static LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

static void dataDisplay();
static void editTemp();
static void editHum();
static void editLight();

void displayInit() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Environmental   ");
  lcd.setCursor(0, 1);
  lcd.print("Monitor System  ");
}

void updateLCD() {
  switch (currentMode) {
    case MODE_DISPLAY:
      dataDisplay();
      break;
    case MODE_EDIT_TEMP:
      editTemp();
      break;
    case MODE_EDIT_HUM:
      editHum();
      break;
    case MODE_EDIT_LIGHT:
      editLight();
      break;
    default:
      dataDisplay();
      break;
  }
}

static void dataDisplay() {
  if (!systemSensors.hasData) {
    lcd.setCursor(0, 0);
    lcd.print("Reading Sensors ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
  } else if (!systemSensors.validSensor) {

    lcd.setCursor(0, 0);
    lcd.print("Sensor error    ");
    lcd.setCursor(0, 1);
    lcd.print("Check DHT11     ");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("H:");
    lcd.print(systemSensors.humidity, 1);
    lcd.print("% T:");
    lcd.print(systemSensors.tempC, 1);
    lcd.print("C ");

    lcd.setCursor(0, 1);
    lcd.print("L:");
    lcd.print(systemSensors.lightVal);
    lcd.print("        ");
  }
}

static void editTemp() {
  lcd.setCursor(0, 0);
  lcd.print("Edit Temperature");
  lcd.setCursor(0, 1);
  lcd.print(systemThresholds.tempLimit, 1);
  lcd.print("C               ");
}

static void editHum() {
  lcd.setCursor(0, 0);
  lcd.print("Edit Humidity   ");
  lcd.setCursor(0, 1);
  lcd.print(systemThresholds.humLimit, 1);
  lcd.print("%               ");
}

static void editLight() {
  lcd.setCursor(0, 0);
  lcd.print("Edit Light      ");
  lcd.setCursor(0, 1);
  lcd.print(systemThresholds.lightLimit);
  lcd.print("             ");
}