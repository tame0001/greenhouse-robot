#ifdef MQTT_ON
//  Report robot current state of operation
void reportState() {
  sprintf(payload, "%d:%d", robotID, state);
  mqttClient->publish("irobot/feedback", payload);
}

//  Report current operation parameters
void reportParameters(int left, int right) {
  sprintf(payload, "%d:%d:%d", robotID, left, right);
  mqttClient->publish("irobot/parameters", payload);
}

//   MQTT Callback when connection is established

void commandTopicCallBack(const String &inPayload) {
#ifdef DEBUG
  Serial.print("Recieve message: ");
  Serial.println(inPayload);
#endif

  if (inPayload == "s") {
    state = STOP;
  }
  else if (inPayload == "w") {
    state = RUN;
  }

  else if (inPayload == "r") {
    state = FORWARD;
  }

  else if (inPayload == "q") {
    state = LEFT;
  }

  else if (inPayload == "e") {
    state = RIGHT;
  }

  else if (inPayload == "u") {
    //      getParameters();
  }

  reportState();
}

void onConnectionEstablished()
{
  sprintf(commandTopic, "irobot/command/%d", robotID);
  reportState();
  //  getParameters();

  mqttClient->subscribe(commandTopic, commandTopicCallBack);
}
#endif
