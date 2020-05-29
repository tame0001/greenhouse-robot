#include "Wire.h" 

const int MPU_ADDR = 0x76;

int16_t temperature; // variables for temperature data

void setup() {
  Serial.begin(115200);
  Wire.begin();
}
void loop() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x00); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(MPU_ADDR, 2, true); // request a total of 7*2=14 registers

  temperature = (Wire.read()<<8 | Wire.read()) >> 5;
  Serial.println(temperature*0.125);

  // delay
  delay(1000);
}
