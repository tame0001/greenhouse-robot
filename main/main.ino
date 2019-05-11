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
State state = STOP;

// timer
volatile int interruptCounter;
hw_timer_t* timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer(){
  portENTER_CRITICAL_ISR(&timerMux);
  sprintf(payload, "%s current state is %d", robotName, state);
  if (client.isConnected() == 1){
//    Serial.println(payload);
    client.publish("irobot/feedback", payload);
  } 
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

//Line follower parameters
unsigned char lineData[16];
int theshore =  70;

void setup()
{
  Wire.begin();
  Serial.begin(115200);
  client.enableDebuggingMessages();
//  timer setup ** doesn't work with publish function **
//  timer = timerBegin(0, 80, true);
//  timerAttachInterrupt(timer, &onTimer, true);
//  timerAlarmWrite(timer, 1000000, true);
//  timerAlarmEnable(timer);

  //Pin setup
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enB, OUTPUT);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
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
  });

  sprintf(payload, "%s is connected", robotName);
  client.publish("irobot/feedback", payload); 
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
  client.loop();
  readIRData();
  printIRDataRaw();
  sprintf(payload, "%s current state is %d", robotName, state);
  client.publish("irobot/feedback", payload); 
  if (state == STOP){
    digitalWrite(enA, LOW);
    digitalWrite(enB, LOW);
  }
  else if (state == RUN){
    digitalWrite(enA, HIGH);
    digitalWrite(enB, HIGH);
  }
  delay(1000);
}
