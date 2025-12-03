#include "appGlobals.h"
#if INCLUDE_ACCELEROMETER

#include "accelerometer.h"
#include "appDefaultConfig.h"

#define LED_ON 0
#define LED_OFF 1

TaskHandle_t accHandle = NULL;
static uint32_t blink_delay = 1000;  // Delay between changing state on LED pin
int led = 33;

void blink(char* nBlink, char ledStatus) {
  digitalWrite(led, ledStatus);  // turn the LED on (HIGH is the voltage level)
  delay(blink_delay);
  (*nBlink)++;
}

static void accelerometerTask(void* parameter) {
  uint32_t blink_delay = *((uint32_t*)parameter);
  bool accVal = false;
  long changeVideo = 0;
  int blinkSeconds = blink_delay / 1000;
  for (;;) {
    char nBlink = 0;
    if (accUse) {
      accVal = getAccVal();
    } else {
      accVal = false;
    }

    if (accVal) {
      blink(&nBlink, LED_ON);
    }

    blink(&nBlink, LED_OFF);

    changeVideo += blinkSeconds * 2;
    if (changeVideo >= (dashCamOn * 60)) {
      changeVideo = 0;
      LOG_INF("Time to change video. and call static bool closeAvi() {in mjpeg2sd.cpp}");
    }
  }
}

static void startAccelerometerTask() {
  pinMode(led, OUTPUT);
  digitalWrite(led, LED_OFF);  // set lamp pin fully off as sd_mmc library still
                               // initialises pin 4 in 1 line mode
  xTaskCreate(&accelerometerTask, "accelerometerTask", ACC_STACK_SIZE, &blink_delay, ACC_PRI,
              &accHandle);

  LOG_INF("Task started");
}

TaskHandle_t getAccelerometerTaskId() {
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

void printAccelerometerStatus() {
  if (accUse) {
    LOG_INF("- activate accelerometer detection");
    LOG_INF("- attach CS to pin %s", getSelectionOption("accCS", accCS));
    LOG_INF("- Interrupt used %s", getSelectionOption("accINT", accINT));
  }
}
#endif
