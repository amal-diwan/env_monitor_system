#include "config.h"
#include "state.h"
#include "scheduler.h"
#include "sensor.h"
#include "display.h"
#include "alert.h"
#include "input.h"
#include "storage.h"
#include "serial_cmd.h"

SensorData systemSensors = {0.0, 0.0, 0, false, false};
AlertData systemAlerts = {false, 0, false, 0, false, false, false};
Threshold systemThresholds = {defaultTempLimit, defaultHumLimit, defaultLightLimit};
SystemMode currentMode = MODE_DISPLAY;
AlertStatus currentAlert = ALERT_NO_DATA;

unsigned long now = 0;

Task tasks[] = {
  {&readSensors, sensorInterval, 0},
  {&updateLCD,   lcdInterval,    0},
  {&updateAlerts, alertInterval,  0},
  {&handleInput, inputInterval, 0},
  {&handleSerial, serialInterval, 0}
};
const int numOfTasks = sizeof(tasks) / sizeof(tasks[0]);

void setup() {
  Serial.begin(9600);

  loadThresholds();

  serialCmdInit();
  sensorInit();
  displayInit();
  alertInit();
  inputInit();
}

void loop() {
  now = millis();
  scheduler(tasks, numOfTasks, now);
}