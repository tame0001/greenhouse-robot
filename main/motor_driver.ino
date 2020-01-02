void turnHandler(long turning_time, int turning_direction) {
  //  Serial.println(turning_time);
  if (turning_direction == LEFT) {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, HIGH);
  }
  else if (turning_direction == RIGHT) {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, HIGH);
    digitalWrite(motor2Pin2, LOW);
  }

  leftSpeed = baseSpeed;
  rightSpeed = baseSpeed;
  leftSpeed = cramp(leftSpeed, 0, 100);
  rightSpeed = cramp(rightSpeed, 0, 100);
  leftSpeed = map(leftSpeed, 0, 100, 0, 255);
  rightSpeed = map(rightSpeed, 0, 100, 0, 255);
  ledcWrite(pwmChannelB, leftSpeed);
  ledcWrite(pwmChannelA, rightSpeed);

  long start_time = millis();
  while (millis() - start_time < turning_time) {

  }
  leftSpeed = 0;
  rightSpeed = 0;
  leftSpeed = cramp(leftSpeed, 0, 100);
  rightSpeed = cramp(rightSpeed, 0, 100);
  leftSpeed = map(leftSpeed, 0, 100, 0, 255);
  rightSpeed = map(rightSpeed, 0, 100, 0, 255);
  ledcWrite(pwmChannelB, leftSpeed);
  ledcWrite(pwmChannelA, rightSpeed);
  digitalWrite(motor1Pin1, HIGH);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, HIGH);
  state = STOP;
  reportState();
}

//---------------------------------------------------------------

void forwardHandler(long driving_time) {
  //  Serial.println(driving_time);

  leftSpeed = baseSpeed;
  rightSpeed = baseSpeed;
  leftSpeed = cramp(leftSpeed, 0, 100);
  rightSpeed = cramp(rightSpeed, 0, 100);
  leftSpeed = map(leftSpeed, 0, 100, 0, 255);
  rightSpeed = map(rightSpeed, 0, 100, 0, 255);
  ledcWrite(pwmChannelB, leftSpeed);
  ledcWrite(pwmChannelA, rightSpeed);

  long start_time = millis();
  while (millis() - start_time < driving_time) {

  }

  leftSpeed = 0;
  rightSpeed = 0;
  leftSpeed = cramp(leftSpeed, 0, 100);
  rightSpeed = cramp(rightSpeed, 0, 100);
  leftSpeed = map(leftSpeed, 0, 100, 0, 255);
  rightSpeed = map(rightSpeed, 0, 100, 0, 255);
  ledcWrite(pwmChannelB, leftSpeed);
  ledcWrite(pwmChannelA, rightSpeed);
  state = STOP;
  reportState();
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
