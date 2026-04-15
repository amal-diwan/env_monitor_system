#include "storage.h"
#include "config.h"
#include "state.h"

#include <EEPROM.h>
#include <Arduino.h>

const unsigned long MAGIC = 0xABCD1234;

struct StoredConfig {
  float tempLimit;
  float humLimit;
  int lightLimit;
  unsigned long magic;
};

static bool configIsValid(const StoredConfig &cfg);

void setDefaultThresholds() {
  systemThresholds.tempLimit = defaultTempLimit;
  systemThresholds.humLimit = defaultHumLimit;
  systemThresholds.lightLimit = defaultLightLimit;
}

void saveThresholds() {
  StoredConfig cfg;

  cfg.magic = MAGIC;
  cfg.tempLimit = systemThresholds.tempLimit;
  cfg.humLimit = systemThresholds.humLimit;
  cfg.lightLimit = systemThresholds.lightLimit;

  EEPROM.put(0, cfg);
}

void loadThresholds() {
  StoredConfig cfg;

  EEPROM.get(0, cfg);

  if (configIsValid(cfg)) {
    systemThresholds.tempLimit = cfg.tempLimit;
    systemThresholds.humLimit = cfg.humLimit;
    systemThresholds.lightLimit = cfg.lightLimit;
  } else {
    setDefaultThresholds();
  }
}

static bool configIsValid(const StoredConfig &cfg) {
  bool valid = true;

  if (cfg.magic != MAGIC) {
    valid = false;
  }
  if (cfg.tempLimit < minTempLimit || cfg.tempLimit > maxTempLimit) {
    valid = false;
  }
  if (cfg.humLimit < minHumLimit || cfg.humLimit > maxHumLimit) {
    valid = false;
  }
  if (cfg.lightLimit < minLightLimit || cfg.lightLimit > maxLightLimit) {
    valid = false;
  }

  return valid;
}