void getParameters() {
  sprintf(requestURL, "%s/parameter/%s", HTTPURL, robotID);
  //  Serial.println(requestURL);
  http.begin(requestURL);
  int httpCode = http.GET();
  //  Serial.println(httpCode);
  if (httpCode > 0) {
    //    Serial.println(http.getString());
    char json[capacity];
    strcpy(json, http.getString().c_str());
    StaticJsonDocument<capacity> doc;
    DeserializationError error = deserializeJson(doc, json);
    if (error) {
      Serial.println(error.c_str());
      return;
    }
    //    Serial.println(doc["robot_id"].as<char*>());
    //    Serial.println(doc["driving_time"].as<int>());
    //    Serial.println(doc["turning_time"].as<int>());
    drivingTime = doc["driving_time"].as<int>();
    turningTime = doc["turning_time"].as<int>();
  }
  http.end();
}
