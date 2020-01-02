#include <Wire.h>
#include <HTTPClient.h>

#include "EspMQTTClient.h"
#include "ArduinoJson.h"

//#define DEBUG
#define MQTT_ON
#define TIMER_ON
//---------------------------------------------------------------

//WiFi and MQTT parameters
#define robotID "2"
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
int theshore = 200;
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

  //    timer1 setup 5 seconds
  timer1 = timerBegin(1, 400, true);
  timerAttachInterrupt(timer1, &onTimer1, true);
  timerAlarmWrite(timer1, 1000000, true);
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
#ifdef DEBUG
  delay(1000);
  Serial.println("----------------------------------------");
  readIRData();
  printIRDataRaw();
  Serial.print("IR Data: ");
  Serial.println(analyzeIRData());
#endif

#ifdef MQTT_ON
  client.loop();
#endif

  if (state == STOP) {
    leftSpeed = 0;
    rightSpeed = 0;
  }
  else if (state == RUN) {
    readIRData();
    feedbackErr = analyzeIRData();
    leftSpeed = baseSpeed + feedbackErr * KP;
    rightSpeed = baseSpeed - feedbackErr * KP;
    leftSpeed = cramp(leftSpeed, 0, 100);
    rightSpeed = cramp(rightSpeed, 0, 100);
  }

  else if (state == FORWARD) {
    forwardHandler(drivingTime);
  }

  else if (state == LEFT) {
    turnHandler(turningTime, LEFT);
  }

  else if (state == RIGHT) {
    turnHandler(turningTime, RIGHT);
  }

#ifdef DEBUG
  Serial.print("leftSpeed: ");
  Serial.println(leftSpeed);
  Serial.print("rightSpeed: ");
  Serial.println(rightSpeed);
#endif

  if (flagTimer0) {
    timer0Service();
  }

  if (flagTimer1) {
    timer1Service();
  }

  leftSpeed = map(leftSpeed, 0, 100, 0, 255);
  rightSpeed = map(rightSpeed, 0, 100, 0, 255);
  ledcWrite(pwmChannelB, leftSpeed);
  ledcWrite(pwmChannelA, rightSpeed);


}
