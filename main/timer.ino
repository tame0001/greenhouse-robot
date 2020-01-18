#ifdef TIMER_ON

void timer0Service() {
  flagTimer0 = false;
  Serial.println(line);

#ifdef MQTT_ON
  reportParameters(leftSpeed, rightSpeed);
#endif
}

void timer1Service() {
  flagTimer1 = false;
  readIRData();

#ifdef MQTT_ON
  //    reportState();
#endif
}

#endif
