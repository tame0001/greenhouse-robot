void timer0Service() {
  flagTimer0 = false;
#ifdef MQTT_ON
  reportParameters(leftSpeed, rightSpeed);
#endif
}

void timer1Service() {
  flagTimer1 = false;
#ifdef MQTT_ON
  //    reportState();
#endif
}
