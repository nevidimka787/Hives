#pragma once

void blinkAndWait_4 () {
  delay(250);
  digitalWrite(13, HIGH);
  delay(250);
  digitalWrite(13, LOW);
  delay(250);
  digitalWrite(13, HIGH);
  delay(250);
  digitalWrite(13, LOW);
}

void blinkAndWait_2 () {
  delay(500);
  digitalWrite(13, HIGH);
  delay(500);
  digitalWrite(13, LOW);
}

void blinkAndWait_alarm () {
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(200);
  digitalWrite(13, HIGH);
  delay(200);
  digitalWrite(13, LOW);
  delay(1000);
}