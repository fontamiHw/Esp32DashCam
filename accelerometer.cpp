#include "appGlobals.h"

#if INCLUDE_ACCELEROMETER
#include "accelerometer.h"
#include "appDefaultConfig.h"

#define LED_ON 0
#define LED_OFF 1

TaskHandle_t accHandle = NULL;
static uint32_t blink_delay_milli = 1000;  // Delay between changing state on LED pin
static int led = 33;
static int accTimeVideo;  // time in minutes to record video on accelerometer activation

bool dashChangeVideo = false;


void blinkAndDelay(char* nBlink, char ledStatus) {
  digitalWrite(led, ledStatus);  // turn the LED on (HIGH is the voltage level)
  delay(blink_delay_milli);
  (*nBlink)++;
}

static void accelerometerTask(void* parameter) {
  bool accVal = false;
  long changeVideo = 0;
  int blinkSeconds = blink_delay_milli / 1000;

  for (;;) {
    char nBlink = 0;
    accVal = getAccVal();

    if (accVal) {
      accTimeVideo = dashCamOn;
      blinkAndDelay(&nBlink, LED_ON);  // first blink
    } else {
      accTimeVideo = 0;
    }

    blinkAndDelay(&nBlink, LED_OFF);  // second blink

    changeVideo += blinkSeconds * nBlink;

    bool sensor = (accTimeVideo && accVal);
    if (sensor && (changeVideo >= (accTimeVideo * 60))) {
      changeVideo = 0;
      LOG_INF("\n Time to change video. \nDashCamOn %d, accTimeVideo %d", dashCamOn, accTimeVideo);
      dashChangeVideo = true;
    }
  }
}

static void startAccelerometerTask() {
  xTaskCreate(&accelerometerTask, "accelerometerTask", ACC_STACK_SIZE, NULL, ACC_PRI, &accHandle);

  LOG_INF("Task started");
}

TaskHandle_t getAccelerometerTaskId() {
  return accHandle;
}

void prepAcc() {

  LOG_INF("--------------- Setup accelerometer");
  accTimeVideo = dashCamOn;  // time in minutes to record video on accelerometer activation
  LOG_INF("     DashCam video legth %d minutes", accTimeVideo);
  LOG_INF("     DashCam is activates %d", getAccVal());
  pinMode(led, OUTPUT);
  digitalWrite(led, LED_OFF);
  LOG_INF("--------------- Setup Completed");

  // finally start accelerometer task
  startAccelerometerTask();
}

bool getAccVal() {
  // get accelerometer status
  // MIK da studiare come ritornare che si e' attivi
  bool ret = accCS > 0;
  return ret;
}

void printAccelerometerStatus() {
  LOG_INF("- activate accelerometer detection");
  LOG_INF("- attach CS to pin %s", getSelectionOption("accCS", accCS));
  LOG_INF("- Interrupt used %s", getSelectionOption("accINT", accINT));
}
#endif
