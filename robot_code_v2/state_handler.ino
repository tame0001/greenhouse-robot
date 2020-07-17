void stateHandler() {
  if (state == STOP) {
    leftSpeed = 0;
    rightSpeed = 0;
  }
  else if (state == RUN) {
    speedAdjust();
    dir = FORWARD;
    digitalWrite(RIGHT_DIR_PIN, dir);
    digitalWrite(LEFT_DIR_PIN, dir);
  }
}

//---------------------------------------------------------------

void speedAdjust() {
  readIRData();
  feedbackErr = analyzeIRData();
  leftSpeed = baseSpeed + feedbackErr * KP;
  rightSpeed = baseSpeed - feedbackErr * KP;
  leftSpeed = cramp(leftSpeed, 0, 100);
  rightSpeed = cramp(rightSpeed, 0, 100);
}

//---------------------------------------------------------------

int cramp(int value, int minimum, int maximum) {
  if (value > maximum) {
    return maximum;
  }
  else if (value < minimum) {
    return minimum;
  }
  else {
    return value;
  }
}

//---------------------------------------------------------------
