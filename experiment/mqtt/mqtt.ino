#include "EspMQTTClient.h"

const char* robotName = "i-robot07";
const char* wifiSSID = "ME588G4";
const char* wifiPassword = "1n1t1al0";
const char* brokerIP = "192.168.1.147";
const short brokerPort = 8883;
const char* robotID = "07";

EspMQTTClient client(
  wifiSSID,
  wifiPassword,
  brokerIP,   
  robotName,
  brokerPort              
);

char payload[30];
char commandTopic[30];

void setup()
{
  Serial.begin(115200);
  client.enableDebuggingMessages();
}

void onConnectionEstablished()
{
  sprintf(commandTopic, "irobot/command/%s", robotID);
  client.subscribe(commandTopic, [](const String & payload) {
    Serial.print("Recieve message: ");
    Serial.println(payload);
  });

  sprintf(payload, "%s is connected", robotName);
  client.publish("irobot/feedback", payload); 
}

void loop()
{
  client.loop();
}
