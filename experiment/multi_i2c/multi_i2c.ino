#include <Wire.h>

//---------------------------------------------------------------
//On-board Temperature Sensor
#define ONTEMP_ADDR 0x49
int16_t temperature;

//---------------------------------------------------------------
// Battery Checker
#define BATT_PIN 34
int BattLevel;

//---------------------------------------------------------------
//LED Array

#define LEDARRAY_ADDR 0x23
const char* linePtr;

//---------------------------------------------------------------
//Line follower parameters

#define LINE_THESHORE 150
#define LINE_ADDR 0x09
unsigned char lineData[16];
char line[8];

//--------------------------------------------------------------

unsigned char t;
String result;


void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(112500);  // start serial for output
  

  initLedArray();
}
void loop()
{


  readIRData();
  readOnboardTemp();
  readBattLevel();

  delay(500);
}

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
  BattLevel = analogRead(BATT_PIN);
  Serial.print("Battery level = ");
  Serial.println((BattLevel + 192) / 1280.0 * 6);
}

void readIRData() {

  t = 0;
  Wire.requestFrom(LINE_ADDR, 16);    // request 16 bytes from slave device #9
  Serial.println("Reading Line Sensor");
  while (Wire.available())   // slave may send less than requested
  {
    lineData[t] = Wire.read(); // receive a byte as character
    if (t < 15)
      t++;
    else
      t = 0;
  }

  Serial.print("lineData[0]:");
  Serial.println(lineData[0]);
  Serial.print("lineData[2]:");
  Serial.println(lineData[2]);
  Serial.print("lineData[4]:");
  Serial.println(lineData[4]);
  Serial.print("lineData[6]:");
  Serial.println(lineData[6]);
  Serial.print("lineData[8]:");
  Serial.println(lineData[8]);
  Serial.print("lineData[10]:");
  Serial.println(lineData[10]);
  Serial.print("lineData[12]:");
  Serial.println(lineData[12]);
  Serial.print("lineData[14]:");
  Serial.println(lineData[14]);

  result = "";
  for (int i = 0; i < 8; i++) {
    if (lineData[i * 2] > LINE_THESHORE) {
      //    Serial.println("1");
      result.concat("1");
    }
    else {
      //      Serial.println("0");
      result.concat("0");
    }
  }
  Serial.println(result);

  linePtr = result.c_str();
  Wire.beginTransmission(LEDARRAY_ADDR);
  Wire.write(0x01);
  Wire.write(strtol(linePtr, NULL, 2));
  Wire.endTransmission(true);
}

void initLedArray() {
  Wire.beginTransmission(LEDARRAY_ADDR);
  Wire.write(0x03);
  Wire.write(0x00);
  Wire.endTransmission(true);

}
