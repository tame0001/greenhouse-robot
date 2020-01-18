#ifdef TIMER_ON

void timer0Service() {
  flagTimer0 = false;

#ifdef MQTT_ON
  reportParameters(leftSpeed, rightSpeed);
#endif
}

void timer1Service() {
  flagTimer1 = false;
  readIRData();

#ifdef MQTT_ON
      reportLineSensor();
#endif
}

#endif
