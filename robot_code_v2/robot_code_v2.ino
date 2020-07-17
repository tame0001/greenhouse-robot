#include <Wire.h>

#include "EspMQTTClient.h"
#include "ArduinoJson.h"

//#define DEBUG
//#define LINEDEBUG_ON
#define MQTT_ON
#define TIMER_ON

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
uint8_t leftSpeed, rightSpeed, feedbackErr;
char payload[30], commandTopic[30];
enum State {STOP, RUN, FORWARD, LEFT, RIGHT, UTURN, NONE};
State state = NONE;


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
int battLevel;

//---------------------------------------------------------------

void setup() {

  Wire.begin();
  Serial.begin(115200);

  initEeprom();
  initLedArray();

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

  //timer1 setup 50 milliseconds
  timer1 = timerBegin(1, 80, true);
  timerAttachInterrupt(timer1, &onTimer1, true);
  timerAlarmWrite(timer1, 50000, true);
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

}
