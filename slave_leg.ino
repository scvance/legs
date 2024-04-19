#include <Wire.h>

const int LEFT_ANK_A = 2;
const int LEFT_ANK_B = 6;
const int RIGHT_ANK_A = 3;
const int RIGHT_ANK_B = 7;

volatile int lAnka, lAnkb, rAnka, rAnkb;
volatile long lAnkPos, rAnkPos;

void setup() {
  Wire.begin(0x50);                  // join i2c bus with address #8
  Wire.onRequest(requestEvent);      // register event
  lAnkPos = 0;
  rAnkPos = 0;
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(LEFT_ANK_A), updateLAnk, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_ANK_A), updateRAnk, CHANGE);
}

void loop() {//all code is interrupt based so nothing goes here}

void updateLAnk() {
  int stateA = digitalRead(LEFT_ANK_A);
  int stateB = digitalRead(LEFT_ANK_B);
  if ((stateA == HIGH && lAnkb == LOW) || (stateA == LOW && lAnkb == HIGH)) {
    lAnkPos++;
  } else if ((stateA == HIGH && lAnkb == HIGH) || (stateA == LOW && lAnkb == LOW)) {
    lAnkPos--;
  }
  lAnka = stateA;
  lAnkb = stateB;
}

void updateRAnk() {
  int stateA = digitalRead(RIGHT_ANK_A);
  int stateB = digitalRead(RIGHT_ANK_B);
  if ((stateA == HIGH && rAnkb == LOW) || (stateA == LOW && rAnkb == HIGH)) {
    rAnkPos++;
  } else if ((stateA == HIGH && rAnkb == HIGH) || (stateA == LOW && rAnkb == LOW)) {
    rAnkPos--;
  }
  rAnka = stateA;
  rAnkb = stateB;
}

void requestEvent() {
  byte dataBuffer[8]; // Buffer to hold the serialized data
  // Serialize the data (convert longs to bytes)
  memcpy(&dataBuffer[0], &lAnkPos, sizeof(lAnkPos));
  memcpy(&dataBuffer[sizeof(lAnkPos)], &rAnkPos, sizeof(rAnkPos));
  // Send the serialized data
  Wire.write(dataBuffer, sizeof(dataBuffer));
}
