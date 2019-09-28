//#include <PubSubClient.h>
//#include <WiFi.h>
#include <Wire.h>
#include "EspMQTTClient.h"

//#define DEBUG
#define MQTT_ON
//#define TIMER_ON

// WiFi and MQTT parameters
#define robotID "17"
#define robot "i-robot"
#define robotName robot robotID
#define wifiSSID "i-robot"
#define wifiPassword "1n1t1al0"
#define brokerIP "128.46.109.133"
#define brokerPort 1883

char payload[30];
char commandTopic[30];

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

enum State {STOP, RUN};
State state = STOP;

#ifdef TIMER_ON
  // timer
  volatile int interruptCounter;
  hw_timer_t* timer = NULL;
  portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
  
  void IRAM_ATTR onTimer(){
    portENTER_CRITICAL_ISR(&timerMux);
    sprintf(payload, "%s current state is %d", robotName, state);
    Serial.println("Timer Overflow");
    portEXIT_CRITICAL_ISR(&timerMux);
  }
#endif

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

//Movement parameters
const int baseSpeed = 80;
const int KP = 10;
//Line follower parameters
unsigned char lineData[16];
int theshore = 200;

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  
  #ifdef MQTT_ON
    client.enableDebuggingMessages();
  #endif

  #ifdef TIMER_ON
  //  timer setup ** doesn't work with publish function **
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 1000000, true);
    timerAlarmEnable(timer);
  #endif

  //Pin setup
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

#ifdef MQTT_ON
  void reportState(){
    sprintf(payload, "%s current state is %d", robotName, state);
    client.publish("irobot/feedback", payload); 
  }
  
  void reportParameters(int left, int right){
    sprintf(payload, "%s:%d:%d", robotName, left, right);
    client.publish("irobot/parameters", payload); 
  }

  // MQTT Call back
  void onConnectionEstablished()
  {
    sprintf(commandTopic, "irobot/command/%s", robotID);
    client.subscribe(commandTopic, [](const String & payload) {
      Serial.print("Recieve message: ");
      Serial.println(payload);
      if (payload == "s"){
        state = STOP;
      }
      else if (payload == "r"){
        state = RUN;
      }
      reportState();
    });
  
    sprintf(payload, "%s is connected", robotName);
    client.publish("irobot/feedback", payload); 
  }
#endif

int cramp(int value, int minimum, int maximum){
  if (value > maximum){
    return maximum;
  }
  else if(value < minimum){
    return minimum;
  }
  else{
    return value;
  }
}

void readIRData(){
  unsigned char t;

  Wire.requestFrom(9, 16);
  Serial.println("Reading Line Sensor");
  while (Wire.available())   
  {
    lineData[t] = Wire.read(); 
    if (t < 15)
      t++;
    else
      t = 0;
  }
}

int analyzeIRData(){
  int sum = 0;
  for (int i=0; i<16; i+=2){
    if (lineData[i] > theshore){
      if (i < 8){
        sum++;
      }
      else{
        sum--;
      }
    }
  }
  return sum;
}

void printIRDataRaw(){
  Serial.print("lineData[0]:");
  Serial.println(lineData[0]);
  Serial.print("lineData[2]:");
  Serial.println(lineData[2]);
  Serial.print("lineData[4]:");
  Serial.println(lineData[4]);
  Serial.print("lineData[6]:");
  Serial.println(lineData[6]);
  Serial.print("lineData[8]:");
  Serial.println(lineData[8]);
  Serial.print("lineData[10]:");
  Serial.println(lineData[10]);
  Serial.print("lineData[12]:");
  Serial.println(lineData[12]);
  Serial.print("lineData[14]:");
  Serial.println(lineData[14]);
}

void loop()
{
  static int leftSpeed, rightSpeed, feedbackErr;
  
  #ifdef DEBUG
    Serial.println("----------------------------------------");
    readIRData();
    printIRDataRaw(); 
    Serial.print("IR Data: ");
    Serial.println(analyzeIRData());
  #endif

  #ifdef MQTT_ON
    client.loop();
  #endif
  
  if (state == STOP){
    leftSpeed = 0;
    rightSpeed = 0;
  }
  else if (state == RUN){
    feedbackErr = analyzeIRData();
    leftSpeed = baseSpeed + feedbackErr * KP;
    rightSpeed = baseSpeed - feedbackErr * KP;
    leftSpeed = cramp(leftSpeed, 0, 100);
    rightSpeed = cramp(rightSpeed, 0, 100);
  }

  #ifdef DEBUG
    Serial.print("leftSpeed: ");
    Serial.println(leftSpeed);
    Serial.print("rightSpeed: ");
    Serial.println(rightSpeed);
  #endif
  
  leftSpeed = map(leftSpeed, 0, 100, 0, 255);
  rightSpeed = map(rightSpeed, 0, 100, 0, 255);
  ledcWrite(pwmChannelB, leftSpeed);
  ledcWrite(pwmChannelA, rightSpeed);

  #ifdef MQTT_ON
    reportParameters(leftSpeed, rightSpeed);
  #endif
  delay(1000);
}
