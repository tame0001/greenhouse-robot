void stateHandler() {

  if (state == STOP) {
    leftSpeed = 0;
    rightSpeed = 0;
  }
  //----------------------------
  else if (state == RUN) {
    speedAdjust();
    digitalWrite(LEFT_DIR_PIN, FORWARD);
    digitalWrite(RIGHT_DIR_PIN, FORWARD);
  }
  //----------------------------
  else if (state == MOVE) {
    speedAdjust();
    digitalWrite(LEFT_DIR_PIN, FORWARD);
    digitalWrite(RIGHT_DIR_PIN, FORWARD);

    if (moveStep == INITIAL) {
      moveStep = DEPART_CROSS;
    }

    else if (moveStep == DEPART_CROSS) {
      if (line[0] == '1' && line[7] == '1') {
        moveStep = FIND_CROSS;
      }
    }

    else if (moveStep == FIND_CROSS) {
      if (line[0] == '0' && line[7] == '0') {
        moveStep = INITIAL;
        state = STOP;
        reportState();
      }
    }

  }
  //----------------------------
  else if (state == LEFT) {

    if (moveStep == INITIAL) {
      speedAdjust();
      digitalWrite(LEFT_DIR_PIN, BACKWARD);
      digitalWrite(RIGHT_DIR_PIN, BACKWARD);
      if (line[0] == '0' && line[7] == '0') {
        moveStep = DEPART_CENTER;
      }
    }

    else if (moveStep == DEPART_CENTER) {
      leftSpeed = baseSpeed;
      rightSpeed = baseSpeed;
      digitalWrite(LEFT_DIR_PIN, BACKWARD);
      digitalWrite(RIGHT_DIR_PIN, FORWARD);
      if (line[0] == '1' || line[7] == '1') {
        moveStep = DEPART_CROSS;
      }
    }

    else if (moveStep == DEPART_CROSS) {
      leftSpeed = baseSpeed;
      rightSpeed = baseSpeed;
      digitalWrite(LEFT_DIR_PIN, FORWARD);
      digitalWrite(RIGHT_DIR_PIN, FORWARD);
      if (line[3] == '1' && line[4] == '1') {
        moveStep = FIND_CENTER;
        time_limit = millis();
      }
    }

    else if (moveStep == FIND_CENTER) {
      leftSpeed = baseSpeed;
      rightSpeed = baseSpeed;
      digitalWrite(LEFT_DIR_PIN, BACKWARD);
      digitalWrite(RIGHT_DIR_PIN, FORWARD);
      if (line[4] == '0' && line[5] == '0' && line[6] == '0' or millis() - time_limit > 1500) {
        moveStep = INITIAL;
        state = STOP;
        reportState();
      }
    }
  }

  //----------------------------

  else if (state == RIGHT) {

    if (moveStep == INITIAL) {
      speedAdjust();
      digitalWrite(LEFT_DIR_PIN, BACKWARD);
      digitalWrite(RIGHT_DIR_PIN, BACKWARD);
      if (line[0] == '0' && line[7] == '0') {
        moveStep = DEPART_CENTER;
      }
    }

    else if (moveStep == DEPART_CENTER) {
      leftSpeed = baseSpeed;
      rightSpeed = baseSpeed;
      digitalWrite(LEFT_DIR_PIN, FORWARD);
      digitalWrite(RIGHT_DIR_PIN, BACKWARD);
      if (line[0] == '1' || line[7] == '1') {
        moveStep = DEPART_CROSS;
      }
    }

    else if (moveStep == DEPART_CROSS) {
      leftSpeed = baseSpeed;
      rightSpeed = baseSpeed;
      digitalWrite(LEFT_DIR_PIN, FORWARD);
      digitalWrite(RIGHT_DIR_PIN, FORWARD);
      if (line[3] == '1' && line[4] == '1') {
        moveStep = FIND_CENTER;
        time_limit = millis();
      }
    }

    else if (moveStep == FIND_CENTER) {
      leftSpeed = baseSpeed;
      rightSpeed = baseSpeed;
      digitalWrite(LEFT_DIR_PIN, FORWARD);
      digitalWrite(RIGHT_DIR_PIN, BACKWARD);
      if (line[1] == '0' && line[2] == '0' && line[3] == '0' or millis() - time_limit > 1500) {
        moveStep = INITIAL;
        state = STOP;
        reportState();
      }
    }
  }

  //----------------------------

}

//---------------------------------------------------------------

void speedAdjust() {
  //  readIRData();
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
