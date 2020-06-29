#include <Wire.h>
unsigned char t;
//void send_data(short a1,short b1,short c1,short d1,short e1,short f1);
unsigned char data[16];
String result;
const char* result_ptr;
int theshore =  150;

const int batt = 34;
int batt_level = 0;

const int TEMP_ADDR = 0x49;
int16_t temperature; // variables for temperature data

const int LED_ADDR = 0x23;

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(112500);  // start serial for output
  t = 0;
  Wire.beginTransmission(LED_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x03); // 
  Wire.write(0x00); // 
  Wire.endTransmission(true);
  Serial.println("Setup");
}
void loop()
{
  Wire.requestFrom(9, 16);    // request 16 bytes from slave device #9
  Serial.println("Reading Line Sensor");
  while (Wire.available())   // slave may send less than requested
  {
    data[t] = Wire.read(); // receive a byte as character
    if (t < 15)
      t++;
    else
      t = 0;
  }

//  for (t=0; t < 16; t++){
//    if (t%2 == 0){
//      Serial.println(data[t]);
//    }
//////      if(data[t] < theshore){
//////         result[t/2] = '1';
//////      }
//////      else{
//////        result[t/2] = '0';
//////      }
//////    }
//  }
  Serial.print("data[0]:");
  Serial.println(data[0]);
  Serial.print("data[2]:");
  Serial.println(data[2]);
  Serial.print("data[4]:");
  Serial.println(data[4]);
  Serial.print("data[6]:");
  Serial.println(data[6]);
  Serial.print("data[8]:");
  Serial.println(data[8]);
  Serial.print("data[10]:");
  Serial.println(data[10]);
  Serial.print("data[12]:");
  Serial.println(data[12]);
  Serial.print("data[14]:");
  Serial.println(data[14]);

  result = "";
  for(int i=0; i<8; i++){
    if(data[i*2] > theshore){
//    Serial.println("1");
    result.concat("1");
    }
    else{
//      Serial.println("0");
      result.concat("0");
    }
  }
  Serial.println(result);
  result_ptr = result.c_str();

  Wire.beginTransmission(LED_ADDR); // Begins a transmission to the I2C slave (GY-521 board)
  Wire.write(0x01); // 
  Wire.write(strtol(result_ptr, NULL, 2)); // 
  Wire.endTransmission(true);

  Wire.beginTransmission(TEMP_ADDR);
  Wire.write(0x00); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  Wire.requestFrom(TEMP_ADDR, 2, true); // request a total of 7*2=14 registers

  temperature = (Wire.read()<<8 | Wire.read()) >> 5;
  Serial.print("Temperature = ");
  Serial.println(temperature*0.125);

  batt_level = analogRead(batt);
  Serial.print("Battery level = ");
  Serial.println((batt_level+192)/1280.0*6);
  
  delay(1000);
}
