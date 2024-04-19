#ifndef MOTOR_POSITION_UPDATE_H
#define MOTOR_POSITION_UPDATE_H

// all of these variables are globals declared in the main controller function
// for that reason we must redeclare them here, but with the extern keyword
extern const int LEFT_HIP_A;
extern const int LEFT_HIP_B;
extern const int LEFT_KNEE_A;
extern const int LEFT_KNEE_B;
extern const int RIGHT_HIP_A;
extern const int RIGHT_HIP_B;
extern const int RIGHT_KNEE_A;
extern const int RIGHT_KNEE_B;
extern int lHipa, lHipb, rHipa, rHipb, lKneea, lKneeb, rKneea, rKneeb;
extern volatile long lHipPos;
extern volatile long rHipPos;
extern volatile long lKneePos;
extern volatile long rKneePos;

// every function here performs quadrature decoding logic

void updateLHip() {
  int stateA = digitalRead(LEFT_HIP_A);
  int stateB = digitalRead(LEFT_HIP_B);
  if ((stateA == HIGH && lHipb == LOW) || (stateA == LOW && lHipb == HIGH)) {
    lHipPos++;
  } else if ((stateA == HIGH && lHipb == HIGH) || (stateA == LOW && lHipb == LOW)) {
    lHipPos--;
  }
  lHipa = stateA;
  lHipb = stateB;
}

void updateRHip() {
  int stateA = digitalRead(RIGHT_HIP_A);
  int stateB = digitalRead(RIGHT_HIP_B);
  if ((stateA == HIGH && rHipb == LOW) || (stateA == LOW && rHipb == HIGH)) {
    rHipPos++;
  } else if ((stateA == HIGH && rHipb == HIGH) || (stateA == LOW && rHipb == LOW)) {
    rHipPos--;
  }
  rHipa = stateA;
  rHipb = stateB;
}

void updateLKnee() {
  int stateA = digitalRead(LEFT_KNEE_A);
  int stateB = digitalRead(LEFT_KNEE_B);
  if ((stateA == HIGH && lKneeb == LOW) || (stateA == LOW && lKneeb == HIGH)) {
    lKneePos++;
  } else if ((stateA == HIGH && lKneeb == HIGH) || (stateA == LOW && lKneeb == LOW)) {
    lKneePos--;
  }
  lKneea = stateA;
  lKneeb = stateB;
}

void updateRKnee() {
  int stateA = digitalRead(RIGHT_KNEE_A);
  int stateB = digitalRead(RIGHT_KNEE_B);
  if ((stateA == HIGH && rKneeb == LOW) || (stateA == LOW && rKneeb == HIGH)) {
    rKneePos++;
  } else if ((stateA == HIGH && rKneeb == HIGH) || (stateA == LOW && rKneeb == LOW)) {
    rKneePos--;
  }
  rKneea = stateA;
  rKneeb = stateB;
}

#endif
