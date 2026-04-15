#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <Arduino.h>

struct Task {
  void (*taskptr)();
  unsigned long interval;
  unsigned long lastRun;
};

void scheduler(Task arr[], int num, unsigned long currentTime);

#endif