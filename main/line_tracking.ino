void speedAdjust() {
  readIRData();
  feedbackErr = analyzeIRData();
  leftSpeed = baseSpeed + feedbackErr * KP;
  rightSpeed = baseSpeed - feedbackErr * KP;
  leftSpeed = cramp(leftSpeed, 0, 100);
  rightSpeed = cramp(rightSpeed, 0, 100);
}

//---------------------------------------------------------------

void checkCrossHander() {
  speedAdjust();

  sprintf(payload, "%s:%s", robotID, line);
  client.publish("irobot/debug", payload);


  if (line[0] == '0' && line[7] == '0') {
    state = STOP;
    reportState();
  }


}

