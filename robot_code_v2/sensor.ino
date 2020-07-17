void readOnboardTemp() {
  Wire.beginTransmission(ONTEMP_ADDR);
  Wire.write(0x00);   
  Wire.endTransmission(false);
  Wire.requestFrom(ONTEMP_ADDR, 2, true);

  temperature = (Wire.read() << 8 | Wire.read()) >> 5;
  Serial.print("Temperature = ");
  Serial.println(temperature * 0.125);
}

void readBattLevel() {
  battLevel = analogRead(BATT_PIN);
  Serial.print("Battery level = ");
  Serial.println((battLevel + 192) / 1280.0 * 6);
}
