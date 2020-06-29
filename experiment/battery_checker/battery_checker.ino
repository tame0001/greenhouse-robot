// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int batt = 34;

// variable for storing the potentiometer value
int batt_level = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  // Reading potentiometer value
  batt_level = analogRead(batt);
  Serial.println((batt_level+192)/1280.0*6);
  delay(1000);
}
