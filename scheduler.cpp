#include "scheduler.h"

void scheduler(Task arr[], int num, unsigned long currentTime) {
  for (int i = 0; i < num; i++) {
    if (currentTime - arr[i].lastRun >= arr[i].interval) {
      arr[i].taskptr();
      arr[i].lastRun = currentTime;
    }
  }
}