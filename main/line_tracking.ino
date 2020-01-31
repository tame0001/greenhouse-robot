void speedAdjust() {
  readIRData();
  feedbackErr = analyzeIRData();
  leftSpeed = baseSpeed + feedbackErr * KP;
  rightSpeed = baseSpeed - feedbackErr * KP;
  leftSpeed = cramp(leftSpeed, 0, 100);
  rightSpeed = cramp(rightSpeed, 0, 100);
}

//---------------------------------------------------------------

void checkCrossHandler() {
  speedAdjust();

  if (line[0] == '0' && line[7] == '0' && line[3] == '0' && line[4] == '0') {
    state = STOP;
    reportState();
    
    sprintf(payload, "%s:%s found cross", robotID, line);
    client.publish("irobot/debug", payload);
  }
}

//---------------------------------------------------------------

void headingAdjust() {

  if (state == LEFT) {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
  }
  else if (state == RIGHT) {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
  }

  leftSpeed = baseSpeed;
  rightSpeed = baseSpeed;
  leftSpeed = cramp(leftSpeed, 0, 100);
  rightSpeed = cramp(rightSpeed, 0, 100);
}

//---------------------------------------------------------------

void checkCenterHandler() {


  headingAdjust();
  readIRData();

  if (line[2] == '0' && line[3] == '0' && line[4] == '0' && line[5] == '0') {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
    state = STOP;
    reportState();

    sprintf(payload, "%s:%s found center", robotID, line);
    client.publish("irobot/debug", payload);

  }
}

