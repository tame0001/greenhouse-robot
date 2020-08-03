#ifdef TIMER_ON

void timer0Service() {
  flagTimer0 = false;
  readBattLevel();
  readOnboardTemp();
  beaconHandler();

#ifdef MQTT_ON
  reportParameters(leftSpeed, rightSpeed);
  reportLineSensor();
#endif
}

void timer1Service() {
  flagTimer1 = false;
  readIRData();
}

#endif

//---------------------------------------------------------------
