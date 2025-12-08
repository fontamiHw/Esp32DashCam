#pragma once
#include "Arduino.h"  // FOR fREErtos


#define ACC_STACK_SIZE (1024 * 2)
#define ACC_PRI 2


// accelerometer (ADXL345) pins
extern int accINT;
extern bool dashChangeVideo;
extern bool collisionDetect;

void prepAcc();
TaskHandle_t getAccelerometerTaskId();
bool dashShallRecord();
void accCurrentAviDir(String currentDir);
void saveCurrentDir();
