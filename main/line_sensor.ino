void readIRData() {
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

//Process raw value from line IR sensor
int analyzeIRData() {
  int sum = 0;
  for (int i = 0; i < 16; i += 2) {
    if (lineData[i] > theshore) {
      if (i < 8) {
        sum++;
      }
      else {
        sum--;
      }
    }
  }
  return sum;
}

//Print raw line IR sensor values
void printIRDataRaw() {
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
