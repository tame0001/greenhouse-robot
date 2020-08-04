#include "Wire.h" 

const int MPU_ADDR = 0x50;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x00);
  Wire.write(7); //robot ID
  Wire.write(0); // u-turn direction 0-left 1-right
  Wire.endTransmission(true);

}
void loop() {
  
  int addr = 0;
  
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x00); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 16);

  while (Wire.available()) {
    Serial.print("Addr 0x");
    Serial.print(addr, HEX);
    Serial.print(" value: ");
    Serial.println(Wire.read(), HEX);
    addr++;
  }
  
  delay(10000);
  Serial.println("---------------------------------");
}
