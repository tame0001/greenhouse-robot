void readIRData() {

#ifdef LINEDEBUG_ON
  Serial.println("Reading Line Sensor");
#endif
  
  linePos = 0;
  Wire.requestFrom(LINE_ADDR, 16);
  while (Wire.available())
  {
    lineData[linePos] = Wire.read();
    if (linePos < 15)
      linePos++;
    else
      linePos = 0;
  }

#ifdef LINEDEBUG_ON
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

#endif

  result = "";
  for (int i = 0; i < 8; i++) {
    if (lineData[i * 2] > LINE_THESHORE) {
#ifdef LINEDEBUG_ON
      Serial.println("1");
#endif
      result.concat("1");
    }
    else {
#ifdef LINEDEBUG_ON
      Serial.println("0");
#endif
      result.concat("0");
    }
  }
#ifdef LINEDEBUG_ON
  Serial.println(result);
  Serial.println("-------------------------------------");
#endif
  result.toCharArray(line, 9);

  linePtr = result.c_str();

  Wire.beginTransmission(LEDARRAY_ADDR);
  Wire.write(0x01);
  Wire.write(strtol(linePtr, NULL, 2));
  Wire.endTransmission(true);

}

#ifdef MQTT_ON
void reportLineSensor() {
  sprintf(payload, "%d:%s", robotID, line);
  mqttClient->publish("irobot/linesensor", payload);
}
#endif

//Process raw value from line IR sensor
int analyzeIRData() {
  int sum = 0;

  for (int i = 0; i < 4; i++) {
    sum += int(line[i]);
  }

  for (int i = 4; i < 8; i++) {
    sum -= int(line[i]);
  }
  return sum;
}

void initLedArray() {
  Wire.beginTransmission(LEDARRAY_ADDR);
  Wire.write(0x03);
  Wire.write(0x00);
  Wire.endTransmission(true);

}
