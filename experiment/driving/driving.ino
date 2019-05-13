#include <WiFi.h>

//Right Motor Pins
const char motor1Pin1 = 18;
const char motor1Pin2 = 19;
const char enA = 5;
//Left Motor Pins
const char motor2Pin1 = 16;
const char motor2Pin2 = 17;
const char enB = 4;
//Setting PWM properties
const unsigned int freq = 30000;
const char pwmChannelB = 1;
const char pwmChannelA = 0;
const char resolution = 8;
//Movement parameters
const int baseSpeed = 80;
const int KP = 10;

//Line follower parameters
unsigned char lineData[16];
int theshore = 200;

void setup()
{
  Wire.begin();
  Serial.begin(115200);

  //Pin setup
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  ledcSetup(pwmChannelA, freq, resolution);
  ledcSetup(pwmChannelB, freq, resolution);
  ledcAttachPin(enA, pwmChannelA);
  ledcAttachPin(enB, pwmChannelB);
}

int cramp(int value, int minimum, int maximum){
  if (value > maximum){
    return maximum;
  }
  else if(value < minimum){
    return minimum;
  }
  else{
    return value;
  }
}

void readIRData(){
  unsigned char t;

  Wire.requestFrom(9, 16);
  Serial.println("Reading Line Sensor");
  while (Wire.available())   
  {
    lineData[t] = Wire.read(); 
    if (t < 15)
      t++;
    else
      t = 0;
  }
}

int analyzeIRData(){
  int sum = 0;
  for (int i=0; i<16; i+=2){
    if (lineData[i] > theshore){
      if (i < 8){
        sum++;
      }
      else{
        sum--;
      }
    }
  }
  return sum;
}

void printIRDataRaw(){
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
}

void loop()
{
  static int leftSpeed, rightSpeed, feedbackErr;
  client.loop();
  readIRData();
  printIRDataRaw(); 
  Serial.println(analyzeIRData());
  if (state == STOP){
    leftSpeed = 0;
    rightSpeed = 0;
  }
  else if (state == RUN){
    feedbackErr = analyzeIRData();
    leftSpeed = baseSpeed + feedbackErr * KP;
    rightSpeed = baseSpeed - feedbackErr * KP;
    leftSpeed = cramp(leftSpeed, 0, 100);
    rightSpeed = cramp(rightSpeed, 0, 100);
    
//    leftSpeed = baseSpeed;
//    rightSpeed = baseSpeed;
  }
  Serial.println(leftSpeed);
  Serial.println(rightSpeed);
  leftSpeed = map(leftSpeed, 0, 100, 0, 255);
  rightSpeed = map(rightSpeed, 0, 100, 0, 255);
  ledcWrite(pwmChannelB, leftSpeed);
  ledcWrite(pwmChannelA, rightSpeed);
  delay(100);
}
