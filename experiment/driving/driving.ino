#include <PubSubClient.h>
#include <WiFi.h>
#include <Wire.h>
#include "EspMQTTClient.h"

// WiFi and MQTT parameters
const char* robotName = "i-robot17";
const char* wifiSSID = "ME588G4";
const char* wifiPassword = "1n1t1al0";
const char* brokerIP = "192.168.1.139";
const short brokerPort = 8883;
const char* robotID = "17";

// create MQTT client
EspMQTTClient client(
  wifiSSID,
  wifiPassword,
  brokerIP,   
  robotName,
  brokerPort              
);

char payload[30];
char commandTopic[30];

enum State {STOP, RUN};
State state = RUN;

// timer
volatile int interruptCounter;
hw_timer_t* timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer(){
  portENTER_CRITICAL_ISR(&timerMux);
  sprintf(payload, "%s current state is %d", robotName, state);
//  readIRData();
//  if (client.isConnected() == 1){
//    Serial.println(payload);
//    client.publish("irobot/feedback", payload);
//  } 
  portEXIT_CRITICAL_ISR(&timerMux);
}

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
//Movement parameters (initial)
const int baseSpeed = 80;
const int KP = 5;

//Line follower parameters
unsigned char data[16];
int theshore = 300;
String lineResult;
unsigned char t;

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  t = 0;
  Serial.println("Setup");
  
  client.enableDebuggingMessages();
//  timer setup ** doesn't work with publish function **
//  timer = timerBegin(0, 80, true);
//  timerAttachInterrupt(timer, &onTimer, true);
//  timerAlarmWrite(timer, 1000000, true);
//  timerAlarmEnable(timer);

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

void reportState(){
  sprintf(payload, "%s current state is %d", robotName, state);
  client.publish("irobot/feedback", payload); 
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
    data[t] = Wire.read(); 
    if (t < 15)
      t++;
    else
      t = 0;
  }
}

int analyzeIRData(){
  int sum = 0;
  for (int i=0; i<16; i+=2){
    if (data[i] > theshore){
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
  Serial.print("data[0]:");
  Serial.println(data[0]);
  Serial.print("data[2]:");
  Serial.println(data[2]);
  Serial.print("data[4]:");
  Serial.println(data[4]);
  Serial.print("data[6]:");
  Serial.println(data[6]);
  Serial.print("data[8]:");
  Serial.println(data[8]);
  Serial.print("data[10]:");
  Serial.println(data[10]);
  Serial.print("data[12]:");
  Serial.println(data[12]);
  Serial.print("data[14]:");
  Serial.println(data[14]);

  lineResult = "";
  for(int i=0; i<8; i++){
    if(data[i*2] > theshore){
//    Serial.println("1");
    lineResult.concat("1");
    }
    else{
//      Serial.println("0");
      lineResult.concat("0");
    }
  }
  Serial.println(lineResult);
}

void loop()
{
  static int leftSpeed, rightSpeed, feedbackErr;
  client.loop();
  readIRData();
  printIRDataRaw(); 
  Serial.println(analyzeIRData());
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
    
//    leftSpeed = baseSpeed;
//    rightSpeed = baseSpeed;
  }
  Serial.print("leftSpeed: ");
  Serial.println(leftSpeed);
  Serial.print("rightSpeed: ");
  Serial.println(rightSpeed);
  leftSpeed = map(leftSpeed, 0, 100, 0, 255);
  rightSpeed = map(rightSpeed, 0, 100, 0, 255);
  ledcWrite(pwmChannelB, leftSpeed);
  ledcWrite(pwmChannelA, rightSpeed);
  delay(100);
}
