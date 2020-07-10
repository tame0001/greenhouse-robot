#include "Wire.h" 

const int MPU_ADDR = 0x50;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x00);
  Wire.write(7);
  Wire.endTransmission(true);
}
void loop() {
  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x00); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 1, true);
  Serial.println(Wire.read());
  delay(1000);
}
