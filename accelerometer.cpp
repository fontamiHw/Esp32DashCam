#include "accelerometer.h"
#include "appGlobals.h"
#include "appDefaultConfig.h"

#define LED_ON 0
#define LED_OFF 1

TaskHandle_t accHandle = NULL;
static uint32_t blink_delay = 1000;  // Delay between changing state on LED pin
int led=33;


static void accelerometerTask(void* parameter) {
  
  uint32_t blink_delay = *((uint32_t *)parameter);
  bool accVal = false;
  for (;;) {  
    if (accUse) {
      accVal = getAccVal();
    } else {
      accVal = false;
    }
    
    if (accVal) {
      digitalWrite(led, LED_ON);  // turn the LED on (HIGH is the voltage level)
      delay(blink_delay);
    }

    digitalWrite(led, LED_OFF);  // turn the LED off by making the voltage LOW
    delay(blink_delay);
  }
}

static void startAccelerometerTask(){
  pinMode(led, OUTPUT);
  digitalWrite(led, LED_OFF); // set lamp pin fully off as sd_mmc library still initialises pin 4 in 1 line mode
  xTaskCreate(&accelerometerTask, "accelerometerTask", ACC_STACK_SIZE, &blink_delay, ACC_PRI, &accHandle);
  
  LOG_INF("Task started");
}

TaskHandle_t getTaskId() {
  return accHandle;
}

void prepAcc() {
  LOG_INF("Preparing accelerometer");
  startAccelerometerTask();
  if (accUse) {
    LOG_INF("activation CS on %s", getSelectionOption("accCS", accCS));
  } else {
    LOG_INF("accelerometer Not in use");
  }
}

bool getAccVal() {
  // get accelerometer status
  // MIK da studiare come ritornare che si e' attivi
  bool ret = accCS > 0;
  return ret;
}