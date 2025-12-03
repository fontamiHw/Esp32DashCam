#pragma once
#include "Arduino.h" // FOR fREErtos


#define ACC_STACK_SIZE (1024 * 2)
#define ACC_PRI 2

void prepAcc();
TaskHandle_t getAccelerometerTaskId();
bool getAccVal();
void printAccelerometerStatus();