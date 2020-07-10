#include <Wire.h>

#include "EspMQTTClient.h"
#include "ArduinoJson.h"

//#define DEBUG
#define MQTT_ON
#define TIMER_ON
#define LINEPRINTOUT_ON

//---------------------------------------------------------------

//WiFi and MQTT parameters
#define wifiSSID "i-robot"
#define wifiPassword "1n1t1al0"
//#define brokerIP "128.46.109.133"
#define brokerIP "192.168.1.191"
#define brokerPort 1883

#ifdef MQTT_ON
// create MQTT client using pointer
EspMQTTClient *mqtt_client;
#endif

//---------------------------------------------------------------
//Global variables
char robotID[3], robotName[10];
int leftSpeed, rightSpeed, feedbackErr, drivingTime, turningTime;
char payload[30], commandTopic[30];
enum State {STOP, RUN, FORWARD, LEFT, RIGHT, UTURN, NONE};
State state = NONE;
enum ForwatdStep {DEPART, FINDCROSS};
ForwatdStep forward_step = DEPART;

//---------------------------------------------------------------

void setup() {
  Wire.begin();
  Serial.begin(115200);

  sprintf(robotID, "%s", "20");
  sprintf(robotName, "i-robot%s", robotID);

#ifdef MQTT_ON
  mqtt_client = new EspMQTTClient(
    wifiSSID,
    wifiPassword,
    brokerIP,
    robotName,
    brokerPort
  );

  mqtt_client->enableDebuggingMessages();
  mqtt_client->enableHTTPWebUpdater();
#endif
}

void loop() {
#ifdef MQTT_ON
  mqtt_client->loop();
#endif

}
