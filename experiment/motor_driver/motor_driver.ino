//Setting PWM properties
const unsigned int freq = 500;
const char pwmChannelB = 1;
const char pwmChannelA = 0;
const char resolution = 8;




void setup() {

  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(33, OUTPUT);
  ledcSetup(pwmChannelA, freq, resolution);
  ledcSetup(pwmChannelB, freq, resolution);
  ledcAttachPin(19, pwmChannelA);
  ledcAttachPin(25, pwmChannelB);

}

void loop() {
  ledcWrite(pwmChannelA, 255);
  ledcWrite(pwmChannelB, 50);
  digitalWrite(5, LOW);
  digitalWrite(4, HIGH);
  digitalWrite(26, HIGH);
  digitalWrite(33, HIGH);

}
