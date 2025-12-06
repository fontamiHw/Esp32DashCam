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
SemaphoreHandle_t interruptSemaphore1;  // Create semaphore handle
SemaphoreHandle_t interruptSemaphore2;

void keyISR1() {  // ISR definition
  xSemaphoreGiveFromISR(interruptSemaphore1, NULL);
}

void keyISR2() {
  xSemaphoreGiveFromISR(interruptSemaphore2, NULL);
}

static void blinkAndDelay(char* nBlink, char ledStatus) {
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
  xTaskCreate(&accelerometerTask, "AccelerometerTask", ACC_STACK_SIZE, NULL, ACC_PRI, &accHandle);
  LOG_INF("AccelerometerTask Task started");
}

static void IRAM_ATTR accISR() {
  // joystick button pressed - toggle state
}

void blink1(void* pvParameters) {
  LOG_INF("       Starting blink1 wait");
  while (1) {
    // wait forever until semaphore is given by ISR
    if (xSemaphoreTake(interruptSemaphore1, portMAX_DELAY)) {
      for (int i = 0; i < 10; i++) {
        LOG_INF("blink1 ON");
        delay(50);
        LOG_INF("blink1 OFF");
        delay(50);
      }
    }
  }
}

void blink2(void* pvParameters) {
  LOG_INF("       Starting blink2 wait");
  while (1) {
    // wait forever until semaphore is given by ISR
    if (xSemaphoreTake(interruptSemaphore2, portMAX_DELAY)) {
      for (int i = 0; i < 10; i++) {
        LOG_INF("blink2 ON");
        delay(50);
        LOG_INF("blink2 OFF");
        delay(50);
      }
    }
  }
}
#define INT_PIN_1 12
#define INT_PIN_2 13
static int attachInterrupts(int interruptMode) {
  LOG_INF("       Attaching interrupts and its tasks");
  interruptSemaphore1 = xSemaphoreCreateBinary();  // Create semaphore
  interruptSemaphore2 = xSemaphoreCreateBinary();
  LOG_INF("       Attaching interrupt 1");
  if (interruptSemaphore1 != NULL) {
    pinMode(INT_PIN_1, INPUT_PULLUP);
    int pin = digitalPinToInterrupt(INT_PIN_1);
    LOG_INF("       Attached interrupt on pin %d", pin);
    attachInterrupt(pin, keyISR1, FALLING);
  }

  LOG_INF("       Attaching interrupt 2");
  if (interruptSemaphore2 != NULL) {
    pinMode(INT_PIN_2, INPUT_PULLUP);
    int pin = digitalPinToInterrupt(INT_PIN_2);
    LOG_INF("       Attached interrupt on pin %d", pin);
    attachInterrupt(pin, keyISR2, FALLING);
  }
  LOG_INF("       create  task 1");
  xTaskCreate(blink1,      // Function name of the task
              "Blink 1",   // Name of the task (e.g. for debugging)
              (1024 * 2),  // Stack size (bytes)
              NULL,        // Parameter to pass
              3,           // Task priority
              NULL         // Task handle
  );

  LOG_INF("       create  task 2");
  xTaskCreate(blink2,      // Function name of the task
              "Blink 2",   // Name of the task (e.g. for debugging)
              (1024 * 2),  // Stack size (bytes)
              NULL,        // Parameter to pass
              3,           // Task priority
              NULL         // Task handle
  );
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
  int intPin = getAccVal();
  LOG_INF("     DashCam Interrut is active in %d", attachInterrupts(intPin));
  pinMode(led, OUTPUT);
  digitalWrite(led, LED_OFF);

  // finally start accelerometer task
  startAccelerometerTask();
  LOG_INF("--------------- Setup Completed");
}

bool getAccVal() {
  // get accelerometer status
  // MIK da studiare come ritornare che si e' attivi
  bool ret = accCS > 0;
  return ret;
}

void printAccelerometerConfig() {
  LOG_INF("- activate accelerometer detection");
  LOG_INF("- attach CS to pin %s", getSelectionOption("accCS", accCS));
  LOG_INF("- Interrupt used %s", getSelectionOption("accINT", accINT));
}

TaskHandle_t getAccelerometerTaskId() {
  return accHandle;
}
#endif
