void readIRData() {
  unsigned char t;
  String result;

#ifdef DEBUG
  Serial.println("Reading Line Sensor");
#endif

  Wire.requestFrom(9, 16);
  while (Wire.available())
  {
    lineData[t] = Wire.read();
    if (t < 15)
      t++;
    else
      t = 0;
  }

#ifdef DEBUG
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
    if (lineData[i * 2] > theshore) {
#ifdef DEBUG
      Serial.println("1");
#endif
      result.concat("1");
    }
    else {
#ifdef DEBUG
      Serial.println("0");
#endif
      result.concat("0");
    }
  }
#ifdef DEBUG
  Serial.println(result);
#endif

  result.toCharArray(line, 9);
}

void reportLineSensor() {
  sprintf(payload, "%s:%s", robotID, line);
  client.publish("irobot/linesensor", payload);
}

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


