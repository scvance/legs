#include <Wire.h>
#include <MPU6050.h>
#include <PWFusion_TCA9548A.h>
#include "/Users/samvance/Documents/Arduino/Autonomous_Leg/Motor_Positions.h"

// TCA9548A address, might be 0x77
#define TCA_ADDR 0x70

// make the mux and set up the mpu array
TCA9548A mux;
MPU6050 mpu[7];

const int TCA_ADDRESS = 0x70;
const uint8_t channels[] = {CHAN0, CHAN1, CHAN2, CHAN3, CHAN4, CHAN5, CHAN6, CHAN7};

// these are the motor control pins
// they must ALL be PWM pins with 2 per motor.
// one for forwards and the other for backwards
const int lHipF = 6;
const int lHipB = 7;
const int rHipF = 8;
const int rHipB = 9;
const int lKneeF = 46;
const int lKneeB = 45;
const int rKneeF = 4;
const int rKneeB = 5;
const int lAnkF = 10;
const int lAnkB = 11;
const int rAnkF = 12;
const int rAnkB = 13;

int motorPins[12] = {lHipF, lHipB, rHipF, rHipB, lKneeF, lKneeB, rKneeB, rKneeF, lAnkF, lAnkB, rAnkF, rAnkB};

// since I only have enough interrupt pins for 4 motors on the arduino mega
// we will gather hip and knee data here, then gather ankle data on the Nano
const int LEFT_HIP_A = 2;
const int LEFT_HIP_B = 14;
const int RIGHT_HIP_A = 3;
const int RIGHT_HIP_B = 15;
const int LEFT_KNEE_A = 18;
const int LEFT_KNEE_B = 16;
const int RIGHT_KNEE_A = 19;
const int RIGHT_KNEE_B = 17;

// these variables will hold state
int lHipa, lHipb;
int rHipa, rHipb;
int lKneea, lKneeb;
int rKneea, rKneeb;

// these are volatile because they are modified by external interrupts
volatile long lHipPos;
volatile long rHipPos;
volatile long lKneePos;
volatile long rKneePos;
// these are not volatile because they are modified through SPI
long lAnkPos;
long rAnkPos;

// Function prototypes
void readMotorPositions();

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mux.begin(TCA_ADDRESS, Wire);

  // setup motor pins
  for (int i = 0; i < 12; i++) {
    pinMode(motorPins[i], OUTPUT);
  }

  // setup motor position pins
  pinMode(LEFT_HIP_A, INPUT);
  pinMode(LEFT_HIP_B, INPUT);
  pinMode(RIGHT_HIP_A, INPUT);
  pinMode(RIGHT_HIP_B, INPUT);
  pinMode(LEFT_KNEE_A, INPUT);
  pinMode(LEFT_KNEE_B, INPUT);
  pinMode(RIGHT_KNEE_A, INPUT);
  pinMode(RIGHT_KNEE_B, INPUT);

  // initiialize gyros
  for (int i = 0; i < 7; i++) {
    mux.setChannel(channels[i]);
    mpu[i].initialize();
    delay(100); // stabilize mux
  }

  // setup interrupts
  attachInterrupt(digitalPinToInterrupt(LEFT_HIP_A), updateLHip, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_HIP_A), updateRHip, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LEFT_KNEE_A), updateLKnee, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_KNEE_A), updateRKnee, CHANGE);

  // initialize quadrature encoded states
  lHipa = digitalRead(LEFT_HIP_A);
  lHipb = digitalRead(LEFT_HIP_B);
  rHipa = digitalRead(RIGHT_HIP_A);
  rHipb = digitalRead(RIGHT_HIP_B);
  rKneea = digitalRead(RIGHT_KNEE_A);
  rKneeb = digitalRead(RIGHT_KNEE_B);
  lKneea = digitalRead(LEFT_KNEE_A);
  lKneeb = digitalRead(LEFT_KNEE_B);

  analogWrite(lHipF, 0);
  analogWrite(lHipB, 0);
  analogWrite(rHipF, 0);
  analogWrite(rHipB, 0);
  analogWrite(lKneeF, 0);
  analogWrite(lKneeB, 0);
  analogWrite(rKneeF, 0);
  analogWrite(rKneeB, 0);
  analogWrite(lAnkF, 0);
  analogWrite(lAnkB, 0);
  analogWrite(rAnkF, 0);
  analogWrite(rAnkB, 0);
  
}

void loop() {
  // read motor positions over i2c
  readMotorPositions();
  Serial.print("Left Hip: ");
  Serial.println(lHipPos);
  Serial.print("Right Hip: ");
  Serial.println(rHipPos);
  Serial.print("Left Knee: ");
  Serial.println(lKneePos);
  Serial.print("Right Knee: ");
  Serial.println(rKneePos);
  Serial.print("Left Ankle: ");
  Serial.println(lAnkPos);
  Serial.print("Right Ankle: ");
  Serial.println(rAnkPos);
  // Read sensor data from all mpus
  for (int i = 0; i < 7; i++) {
    mux.setChannel(channels[i]);
    delay(1); // Wait for selection to stabilize
    int16_t ax, ay, az, gx, gy, gz; 
    // we will likely have to hold all of this data in an array outside of this loop
    mpu[i].getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    Serial.print("MPU ");
    Serial.print(i);
    Serial.print(": AccX=");
    Serial.print(ax);
    Serial.print(", AccY=");
    Serial.print(ay);
    Serial.print(", AccZ=");
    Serial.print(az);
    Serial.print(", GyroX=");
    Serial.print(gx);
    Serial.print(", GyroY=");
    Serial.print(gy);
    Serial.print(", GyroZ=");
    Serial.println(gz);
  }
  moveMotorsForward();

  delay(1000); // this is just here for testing purposes. Remove once satisfied with telemetry gathering
}

void readMotorPositions() {
  mux.setChannel(channels[7]); // select the nano
  Wire.requestFrom(0x50, 8); // Request 8 bytes from nano with address 0x50

  // Wait until data is available
  while (Wire.available() < 8);

  // Read the received data
  byte dataBuffer[8];
  Wire.readBytes(dataBuffer, 8);

  // Interpret the received data as long integers
  long receivedData[2];
  memcpy(&receivedData[0], &dataBuffer[0], sizeof(receivedData));
  // assign our globals
  lAnkPos = receivedData[0];
  rAnkPos = receivedData[1];
}

void moveMotorsForward() {
  analogWrite(lHipF, 100);
  analogWrite(rHipF, 100);
  analogWrite(lKneeF, 100);
  analogWrite(rKneeF, 100);
  analogWrite(lAnkF, 100);
  analogWrite(rAnkF, 100);
  delay(500);
  analogWrite(lHipF, 0);
  analogWrite(rHipF, 0);
  analogWrite(lKneeF, 0);
  analogWrite(rKneeF, 0);
  analogWrite(lAnkF, 0);
  analogWrite(rAnkF, 0);
}