void motorExecute() {

  leftSpeed_255 = map(leftSpeed, 0, 100, 0, 255);
  rightSpeed_255 = map(rightSpeed, 0, 100, 0, 255);
  ledcWrite(pwmChannelB, leftSpeed_255);
  ledcWrite(pwmChannelA, rightSpeed_255);
}

//---------------------------------------------------------------

void initMotorIO() {

  Wire.beginTransmission(MOTORIO_ADDR);
  Wire.write(0x03); //
  Wire.write(0x21); //
  Wire.endTransmission(true);

  Wire.beginTransmission(MOTORIO_ADDR);
  Wire.write(0x01); //
  Wire.write(0xE7); //
  Wire.endTransmission(true);

  ledcSetup(pwmChannelA, freq, resolution);
  ledcSetup(pwmChannelB, freq, resolution);
  ledcAttachPin(LEFT_SPPED_PIN, pwmChannelB);
  ledcAttachPin(RIGHT_SPEED_PIN, pwmChannelA);

  pinMode(RIGHT_DIR_PIN, OUTPUT);
  pinMode(LEFT_DIR_PIN, OUTPUT);

  digitalWrite(RIGHT_DIR_PIN, dir);
  digitalWrite(LEFT_DIR_PIN, dir);

}

//---------------------------------------------------------------

void motorSleepOn() {

  char temp; 
  
  Wire.beginTransmission(MOTORIO_ADDR);
  Wire.write(0x01);   
  Wire.endTransmission(false);
  Wire.requestFrom(MOTORIO_ADDR, 1, true);
  temp = Wire.read();

  temp = temp & 0x7B;
  
  Wire.beginTransmission(MOTORIO_ADDR);
  Wire.write(0x01);
  Wire.write(temp);
  Wire.endTransmission(true);
  
}

//---------------------------------------------------------------

void motorSleepOff() {

  char temp; 
  
  Wire.beginTransmission(MOTORIO_ADDR);
  Wire.write(0x01);   
  Wire.endTransmission(false);
  Wire.requestFrom(MOTORIO_ADDR, 1, true);
  temp = Wire.read();

  temp = temp | 0x84;
  
  Wire.beginTransmission(MOTORIO_ADDR);
  Wire.write(0x01);
  Wire.write(temp);
  Wire.endTransmission(true);
  
}
