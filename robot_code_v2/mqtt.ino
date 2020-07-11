#ifdef MQTT_ON
//  Report robot current state of operation
void reportState() {
  sprintf(payload, "%d:%d", robotID, state);
  mqtt_client->publish("irobot/feedback", payload);
}

//  Report current operation parameters
void reportParameters(int left, int right) {
  sprintf(payload, "%d:%d:%d", robotID, left, right);
  mqtt_client->publish("irobot/parameters", payload);
}

//   MQTT Callback when connection is established
void onConnectionEstablished()
{
  sprintf(commandTopic, "irobot/command/%d", robotID);
  reportState();
  //  getParameters();

  //    subcription to command topic
  mqtt_client->subscribe(commandTopic, [](const String & payload) {
#ifdef DEBUG
    Serial.print("Recieve message: ");
    Serial.println(payload);
#endif

    if (payload == "s") {
      state = STOP;
    }
    else if (payload == "w") {
      state = RUN;
    }

    else if (payload == "r") {
      state = FORWARD;
    }

    else if (payload == "q") {
      state = LEFT;
    }

    else if (payload == "e") {
      state = RIGHT;
    }

    else if (payload == "u") {
      //      getParameters();
    }

    reportState();
  });
}
#endif
