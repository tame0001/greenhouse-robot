#include <Wire.h>
#include <HTTPClient.h>

#include "EspMQTTClient.h"
#include "ArduinoJson.h"

//#define DEBUG
#define MQTT_ON
#define TIMER_ON

//---------------------------------------------------------------

//WiFi and MQTT parameters
#define robotID "1"
#define robot "i-robot"
#define robotName robot robotID
#define wifiSSID "i-robot"
#define wifiPassword "1n1t1al0"
#define brokerIP "128.46.109.133"
#define brokerPort 1883

#ifdef MQTT_ON
// create MQTT client
EspMQTTClient client(
  wifiSSID,
  wifiPassword,
  brokerIP,
  robotName,
  brokerPort
);
#endif

//---------------------------------------------------------------
//HTTP Client
#define HTTPURL "http://128.46.109.133:5000"
HTTPClient http;
char requestURL[60];
const int capacity = JSON_OBJECT_SIZE(4) * 2;

//---------------------------------------------------------------
//Global variables
int leftSpeed, rightSpeed, feedbackErr, drivingTime, turningTime;
char payload[30];
char commandTopic[30];
enum State {STOP, RUN, FORWARD, LEFT, RIGHT};
State state = STOP;

//---------------------------------------------------------------
//Timers
#ifdef TIMER_ON
bool flagTimer0, flagTimer1, lineFlag;
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
  lineFlag = true;
  portEXIT_CRITICAL_ISR(&timerMux);
}
#endif

//---------------------------------------------------------------
//Right Motor Pins
const char motor1Pin1 = 18;
const char motor1Pin2 = 19;
const char enA = 5;

//Left Motor Pins
const char motor2Pin1 = 16;
const char motor2Pin2 = 17;
const char enB = 4;

//Setting PWM properties
const unsigned int freq = 30000;
const char pwmChannelB = 1;
const char pwmChannelA = 0;
const char resolution = 8;

//---------------------------------------------------------------
//Movement parameters
const int baseSpeed = 80;
const int KP = 10;

//Line follower parameters
unsigned char lineData[16];
const int theshore = 200;
char line[8];
//---------------------------------------------------------------
void setup()
{
  Wire.begin();
  Serial.begin(115200);

#ifdef MQTT_ON
  client.enableDebuggingMessages();
#endif

  //  ---------------------------------------------------------------
  //  Initiate timers
#ifdef TIMER_ON

  //    timer0 setup 1 second
  timer0 = timerBegin(0, 80, true);
  timerAttachInterrupt(timer0, &onTimer0, true);
  timerAlarmWrite(timer0, 1000000, true);
  timerAlarmEnable(timer0);

  //    timer1 setup 2 milliseconds
  timer1 = timerBegin(1, 80, true);
  timerAttachInterrupt(timer1, &onTimer1, true);
  timerAlarmWrite(timer1, 2000, true);
  timerAlarmEnable(timer1);
#endif

  //  ---------------------------------------------------------------
  //  Pin setup
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  ledcSetup(pwmChannelA, freq, resolution);
  ledcSetup(pwmChannelB, freq, resolution);
  ledcAttachPin(enA, pwmChannelA);
  ledcAttachPin(enB, pwmChannelB);
}

//---------------------------------------------------------------

void loop()
{

#ifdef MQTT_ON
  client.loop();
#endif

  if (state == STOP) {
    leftSpeed = 0;
    rightSpeed = 0;
  }
  else if (state == RUN) {
    speedAdjust();
  }

  else if (state == FORWARD) {

    if (line[0] == '0' && line[7] == '0') {
      forwardHandler(drivingTime);
      state = FORWARD;
      readIRData();
#ifdef DEBUG
      sprintf(payload, "%s:%s exit cross", robotID, line);
      client.publish("irobot/debug", payload);
#endif
    }
    else {
      checkCrossHandler();
    }
  }

  else if (state == LEFT) {
    if (line[0] == '0' && line[7] == '0') {
      forwardHandler(drivingTime);
      turnHandler(turningTime, LEFT);
      state = LEFT;
      readIRData();
#ifdef DEBUG
      sprintf(payload, "%s:%s ready to turn", robotID, line);
      client.publish("irobot/debug", payload);
#endif
    }
    else {
      checkCenterHandler();
    }
  }

  else if (state == RIGHT) {
    if (line[0] == '0' && line[7] == '0') {
      forwardHandler(drivingTime);
      turnHandler(turningTime, RIGHT);
      state = RIGHT;
      readIRData();
#ifdef DEBUG
      sprintf(payload, "%s:%s ready to turn", robotID, line);
      client.publish("irobot/debug", payload);
#endif
    }
    else {
      checkCenterHandler();
    }
  }

#ifdef DEBUG
  Serial.print("leftSpeed: ");
  Serial.println(leftSpeed);
  Serial.print("rightSpeed: ");
  Serial.println(rightSpeed);
#endif

#ifdef TIMER_ON
  if (flagTimer0) {
    timer0Service();
  }

  if (flagTimer1) {
    timer1Service();
  }
#endif

  motorExecute();

}
