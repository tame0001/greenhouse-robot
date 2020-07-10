void initiate_eeprom() {

  pinMode(EEPROM_WC, OUTPUT);
  eeprom_write_protect_on();

  Wire.beginTransmission(EEPROM_ADDR);
  Wire.write(ROBOTID_ADDR); 
  Wire.endTransmission(false);
  Wire.requestFrom(EEPROM_ADDR, 1, true);
  
  robotID = uint8_t(Wire.read());
  sprintf(robotName, "i-robot%d", robotID);
}

void eeprom_write_protect_on() {
  digitalWrite(EEPROM_WC, LOW);
}

void eeprom_write_protect_off() {
  digitalWrite(EEPROM_WC, HIGH);
}
