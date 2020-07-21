#include <Wire.h>

#include "EspMQTTClient.h"
#include "ArduinoJson.h"

//#define DEBUG
//#define LINEDEBUG_ON
#define MQTT_ON
#define TIMER_ON

//---------------------------------------------------------------
//Firmware version
#define MAJOR_VERSION 2
#define MINOR_VERSION 3
#define BUILD_VERSION 3

//---------------------------------------------------------------
//WiFi and MQTT parameters

#define wifiSSID "i-robot"
#define wifiPassword "1n1t1al0"
//#define brokerIP "128.46.109.133"
#define brokerIP "192.168.1.191"
#define brokerPort 1883

#ifdef MQTT_ON
// create MQTT client using pointer
EspMQTTClient *mqttClient;
#endif

//---------------------------------------------------------------
//Global variables

uint8_t robotID;
char robotName[10], ipAdress[16];
uint8_t leftSpeed, rightSpeed, leftSpeed_255, rightSpeed_255;
int8_t feedbackErr;
char payload[50], commandTopic[30];
enum State {STOP, RUN, MOVE, LEFT, RIGHT, UTURN, NONE};
State state = NONE;
enum Message {READY, I2C_FAULT};
Message msg = READY;
enum Feedback {ALL, SPEED, ONB_TEMP, BATT, OFFB_TEMP, LIGHT};

//---------------------------------------------------------------
//Timers

#ifdef TIMER_ON
bool flagTimer0, flagTimer1;
hw_timer_t* timer0 = NULL;
hw_timer_t* timer1 = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer0() {
  portENTER_CRITICAL_ISR(&timerMux);
#ifdef DEBUG
  Serial.println("Timer0 is overflow");
#endif
  flagTimer0 = true;
  portEXIT_CRITICAL_ISR(&timerMux);
}

void IRAM_ATTR onTimer1() {
  portENTER_CRITICAL_ISR(&timerMux);
#ifdef DEBUG
  Serial.println("Timer1 is overflow");
#endif
  flagTimer1 = true;
  portEXIT_CRITICAL_ISR(&timerMux);
}
#endif

//---------------------------------------------------------------
//EEPROM

#define EEPROM_ADDR 0x50
#define EEPROM_WC 23
#define ROBOTID_ADDR 0x00

//---------------------------------------------------------------
//Line follower parameters

#define LINE_THESHORE 150
#define LINE_ADDR 0x09
unsigned char lineData[16];
char line[8];
unsigned char linePos;
String result;

//--------------------------------------------------------------
//LED Array

#define LEDARRAY_ADDR 0x23
const char* linePtr;

//---------------------------------------------------------------
//On-board Temperature Sensor

#define ONTEMP_ADDR 0x49
int16_t temperature;

//---------------------------------------------------------------
// Battery Checker

#define BATT_PIN 34
int16_t battLevel;

//---------------------------------------------------------------
//Movement parameters

const int8_t baseSpeed = 70;
const int8_t KP = 5;

enum MoveStep {INITIAL, DEPART_CROSS, FIND_CROSS, DEPART_CENTER, FIND_CENTER};
MoveStep moveStep = INITIAL;

//---------------------------------------------------------------
//Motor pins

#define RIGHT_SPEED_PIN 25
#define RIGHT_DIR_PIN 26
#define LEFT_SPPED_PIN 32
#define LEFT_DIR_PIN 33

const int16_t freq = 3000;
const char LeftPWM = 1;
const char RightPWM = 0;
const char resolution = 8;

enum Direction {BACKWARD, FORWARD};


//---------------------------------------------------------------
//IO Extender
#define MOTORIO_ADDR 0x21

//---------------------------------------------------------------

void setup() {

  Wire.begin();
  Serial.begin(115200);

  initEeprom();
  initLedArray();
  initMotorIO();
  motorSleepOff();
  blueLedOff();
  greenLedOff();
  checki2c();

#ifdef MQTT_ON
  mqttClient = new EspMQTTClient(
    wifiSSID,
    wifiPassword,
    brokerIP,
    robotName,
    brokerPort
  );

  mqttClient->enableDebuggingMessages();
  mqttClient->enableHTTPWebUpdater();
#endif

#ifdef TIMER_ON
  //timer0 setup 1 second
  timer0 = timerBegin(0, 80, true);
  timerAttachInterrupt(timer0, &onTimer0, true);
  timerAlarmWrite(timer0, 1000000, true);
  timerAlarmEnable(timer0);

  //timer1 setup 20 milliseconds
  timer1 = timerBegin(1, 80, true);
  timerAttachInterrupt(timer1, &onTimer1, true);
  timerAlarmWrite(timer1, 20000, true);
  timerAlarmEnable(timer1);
#endif
}

//---------------------------------------------------------------

void loop() {

#ifdef MQTT_ON
  mqttClient->loop();
#endif

#ifdef TIMER_ON
  if (flagTimer0) {
    timer0Service();
  }

  if (flagTimer1) {
    timer1Service();
  }
#endif

  stateHandler();
  motorExecute();

}
