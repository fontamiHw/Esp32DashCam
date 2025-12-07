#include "appGlobals.h"

#if INCLUDE_ACCELEROMETER
#include "accelerometer.h"
#include "appDefaultConfig.h"

#define LED_ON 0
#define LED_OFF 1
#define NO_INTERRUPT 0
#define INTERRUPT_1 1
#define INTERRUPT_2 2
#define INTERRUPT_1_2 3

TaskHandle_t accHandle = NULL;
static uint32_t blink_delay_milli = 1000;  // Delay between changing state on LED pin
static int led = 33;
static int accTimeVideo;  // time in minutes to record video on accelerometer activation

bool dashChangeVideo = false;
bool accDetectContact = false;
bool dashRecord = false;


void keyISR1() {  // ISR definition
  // at the end of debug remove the semaphore and the relative task
  dashRecord = true;
}

void keyISR2() {
  // at the end of debug remove the semaphore and the relative task
  accDetectContact = true;
  dashRecord = false;
}

static void blinkAndDelay(char* nBlink, char ledStatus) {
  digitalWrite(led, ledStatus);  // turn the LED on (HIGH is the voltage level)
  delay(blink_delay_milli);
  (*nBlink)++;
}

static void accelerometerTask(void* parameter) {
  char isRecording = 0;
  long changeVideo = 0;
  int blinkSeconds = blink_delay_milli / 1000;

  for (;;) {
    char nBlink = 0;

    isRecording = dashShallRecord();

    if (isRecording) {
      accTimeVideo = dashCamOn;
      blinkAndDelay(&nBlink, LED_ON);  // first blink
    } else {
      accTimeVideo = 0;
    }

    blinkAndDelay(&nBlink, LED_OFF);  // second blink

    changeVideo += blinkSeconds * nBlink;

    bool sensor = (accTimeVideo && dashRecord);
    if (sensor && (changeVideo >= (accTimeVideo * 60))) {
      changeVideo = 0;
      LOG_INF("\n Time to change video. \nDashCamOn %d, accTimeVideo %d", dashCamOn, accTimeVideo);
      dashChangeVideo = true;
    }
  }
}

static void startAccelerometerTask() {
  xTaskCreate(&accelerometerTask, "AccelerometerTask", ACC_STACK_SIZE, NULL, ACC_PRI, &accHandle);
  LOG_INF("AccelerometerTask Task started");
}

#define INT_PIN_1 12
#define INT_PIN_2 13
static int attachInterrupts(char interruptMode) {
  LOG_INF("       Attaching interrupts and its tasks");
  if (interruptMode == INTERRUPT_1 || interruptMode == INTERRUPT_1_2) {
    LOG_INF("       Attaching interrupt 1");
    pinMode(INT_PIN_1, INPUT_PULLUP);
    int pin = digitalPinToInterrupt(INT_PIN_1);
    LOG_INF("       Attached interrupt on pin %d", pin);
    attachInterrupt(pin, keyISR1, FALLING);
  }

  if (interruptMode == INTERRUPT_2 || interruptMode == INTERRUPT_1_2) {
    LOG_INF("       Attaching interrupt 2");
    pinMode(INT_PIN_2, INPUT_PULLUP);
    int pin = digitalPinToInterrupt(INT_PIN_2);
    LOG_INF("       Attached interrupt on pin %d", pin);
    attachInterrupt(pin, keyISR2, FALLING);
    LOG_INF("       create  task 2");
  }
  LOG_INF("       END create  task");
  return 1;
}


/*
 * public methods
 */
void prepAcc() {
  LOG_INF("--------------- Setup accelerometer");
  accTimeVideo = dashCamOn;  // time in minutes to record video on accelerometer activation
  LOG_INF("     DashCam video legth %d minutes", accTimeVideo);
  LOG_INF("     DashCam Interrupt is active in %s", getSelectionOption("accINT", accINT));
  attachInterrupts(accINT);
  pinMode(led, OUTPUT);
  digitalWrite(led, LED_OFF);

  // finally start accelerometer task
  startAccelerometerTask();
  LOG_INF("--------------- Setup Completed");
}


bool dashShallRecord() {
  // check if dashcam activated recording needed
  return dashRecord;
}

TaskHandle_t getAccelerometerTaskId() {
  return accHandle;
}
#endif
