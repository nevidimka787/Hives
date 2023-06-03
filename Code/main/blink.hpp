#pragma once

// infinite blink 2 times per second
void blinkAndWait_2 ();
// infinite blink 4 times per second
void blinkAndWait_4 ();
// infinite blink 5 times in second and wait one second
void blinkAndWait_alarm ();
// turn on LED
void LEDOn();
// turn off LED
void LEDOff();

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

void LEDOn() {
  digitalWrite(13, HIGH);
}

void LEDOff() {
  digitalWrite(13, LOW);
}