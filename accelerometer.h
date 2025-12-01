#pragma once
#include "Arduino.h" // FOR fREErtos

void prepAcc();
TaskHandle_t getTaskId();
bool getAccVal();