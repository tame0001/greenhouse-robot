#include <Wire.h>
const int MPU_ADDR = 0x21;

//Setting PWM properties
const unsigned int freq = 500;
const char pwmChannelB = 1;
const char pwmChannelA = 0;
const char resolution = 8;


void setup() {

  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(112500);  // start serial for output
  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x03); // 
  Wire.write(0x21); // 
  Wire.endTransmission(true);

  Wire.beginTransmission(MPU_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x01); // 
  Wire.write(0xE7); // 
  Wire.endTransmission(true);

  pinMode(32, OUTPUT);
  pinMode(33, OUTPUT);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
//  ledcSetup(pwmChannelA, freq, resolution);
//  ledcSetup(pwmChannelB, freq, resolution);
//  ledcAttachPin(19, pwmChannelA);
//  ledcAttachPin(25, pwmChannelB);

  digitalWrite(32, LOW);
  digitalWrite(33, LOW);
  digitalWrite(25, LOW);
  digitalWrite(26, LOW);

}

void loop() {
//  ledcWrite(pwmChannelA, 255);
//  ledcWrite(pwmChannelB, 50);


}
