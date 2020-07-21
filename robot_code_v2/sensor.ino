void readOnboardTemp() {
  Wire.beginTransmission(ONTEMP_ADDR);
  Wire.write(0x00);
  Wire.endTransmission(false);
  Wire.requestFrom(ONTEMP_ADDR, 2, true);

  temperature = (Wire.read() << 8 | Wire.read()) >> 5;

#ifdef DEBUG
  Serial.print("Temperature = ");
  Serial.println(temperature * 0.125);
#endif
}

//---------------------------------------------------------------

void readBattLevel() {
  battLevel = analogRead(BATT_PIN);
#ifdef DEBUG
  Serial.print("Battery level = ");
  Serial.println((battLevel + 192) / 1280.0 * 5.5);
#endif
}

//---------------------------------------------------------------

void initEeprom() {

  pinMode(EEPROM_WC, OUTPUT);
  eepromWriteProtectOn();

  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(ROBOTID_ADDR);
  Wire.endTransmission(false);
  Wire.requestFrom(EEPROM_ADDR, 1, true);

  robotID = uint8_t(Wire.read());
  sprintf(robotName, "i-robot%d", robotID);
}

//---------------------------------------------------------------

void eepromWriteProtectOn() {
  digitalWrite(EEPROM_WC, LOW);
}

//---------------------------------------------------------------

void eepromWriteProtectOff() {
  digitalWrite(EEPROM_WC, HIGH);
}

//---------------------------------------------------------------

void blueLedOn() {

  char temp;

  Wire.beginTransmission(MOTORIO_ADDR);
  Wire.write(0x01);
  Wire.endTransmission(false);
  Wire.requestFrom(MOTORIO_ADDR, 1, true);
  temp = Wire.read();

  temp = temp & 0xF7;

  Wire.beginTransmission(MOTORIO_ADDR);
  Wire.write(0x01);
  Wire.write(temp);
  Wire.endTransmission(true);
}


//---------------------------------------------------------------

void blueLedOff() {

  char temp;

  Wire.beginTransmission(MOTORIO_ADDR);
  Wire.write(0x01);
  Wire.endTransmission(false);
  Wire.requestFrom(MOTORIO_ADDR, 1, true);
  temp = Wire.read();

  temp = temp | 0x08;

  Wire.beginTransmission(MOTORIO_ADDR);
  Wire.write(0x01);
  Wire.write(temp);
  Wire.endTransmission(true);
}


//---------------------------------------------------------------

void greenLedOn() {

  char temp;

  Wire.beginTransmission(MOTORIO_ADDR);
  Wire.write(0x01);
  Wire.endTransmission(false);
  Wire.requestFrom(MOTORIO_ADDR, 1, true);
  temp = Wire.read();

  temp = temp & 0xEF;

  Wire.beginTransmission(MOTORIO_ADDR);
  Wire.write(0x01);
  Wire.write(temp);
  Wire.endTransmission(true);
}


//---------------------------------------------------------------

void greenLedOff() {

  char temp;

  Wire.beginTransmission(MOTORIO_ADDR);
  Wire.write(0x01);
  Wire.endTransmission(false);
  Wire.requestFrom(MOTORIO_ADDR, 1, true);
  temp = Wire.read();

  temp = temp | 0x10;

  Wire.beginTransmission(MOTORIO_ADDR);
  Wire.write(0x01);
  Wire.write(temp);
  Wire.endTransmission(true);
}


//---------------------------------------------------------------

void checki2c() {
  readOnboardTemp();
  if (temperature == -1) {
    msg = I2C_FAULT;
  }
}

//---------------------------------------------------------------
