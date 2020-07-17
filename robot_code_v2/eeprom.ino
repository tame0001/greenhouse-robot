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

void eepromWriteProtectOn() {
  digitalWrite(EEPROM_WC, LOW);
}

void eepromWriteProtectOff() {
  digitalWrite(EEPROM_WC, HIGH);
}
