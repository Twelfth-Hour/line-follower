#include <PID_v1.h>

/*
   PID is used to control whether to fasten or slow down based on difference between Input and
   Setpoint . Basic logic right now is if 3 or less than 3 sensors are on , based on the position
   known we increase the tuning parameters to make an aggressive turn. However, if 4 or more sensors
   are on, we find the position and keep the turning parameters less to make a slow turn and when
   all the sensors are on, i.e the input value is 1023 itself we need to stop the loop and tuning parameters
   to 0 and stop the pid computation.
*/

// Initializing the input and output pins
int inCount = 8;
int outCount = 3;
int inPins[8] = {2, 3, 4, 5, 6, 7, 8, 12};
int inWeights[8] = {+4, +3, +2, +1, -1, -2, -3, -4};
int analogPinOutput[3] = {9, 10, 11};
int analogPinInput[3] = {A1, A2, A3};
int inRange[8] = {};

// Define Variables we'll be connecting to
double Setpoint, Input, Output;

// Define the aggressive and conservative Tuning Parameters
// Experimental tuning values
double aggKp = 2, aggKi = 0.1, aggKd = 0.5;
double consKp = 0.75, consKi = 0.03, consKd = 0.25;

// Specfiying tuning params and initializing the PID
PID pid(&Input, &Output, &Setpoint, consKp, consKi, consKd, DIRECT);

void setup() {
  Serial.begin(9600);
  // Setting the input pin mode
  for (int i = 0; i < inCount; ++i) {
    pinMode(inPins[i], INPUT);
  }
  // Setting the output pin mode
  for (int j = 0; j < outCount; ++j) {
    pinMode(analogPinOutput[j], OUTPUT);
  }

  // initialize the variables we are linked to
  Input = 0;
  Setpoint = 383.625;  // Min 3 sensors need to be on ((3*1023)/8)

  // Turn the PID on
  pid.SetMode(AUTOMATIC);
}

int totalPinHigh(int inRange[]) {
  int count = 0;
  for (int i = 0; i < inCount; ++i) {
    if (inRange[i] == 1023)
      count++;  // count the number of sensors that are on
  }
  return count;
}

int calcPositionInput(int inRange[]) {
  int sum = 0;
  int count = totalPinHigh(inRange);
  for (int i = 0; i < inCount; ++i) {
    if (inRange[i] == 1023)
      sum += inWeights[i];
  }

  if (sum > 1) {
    return 0;  // most sensors are on left
  } else if (sum < 0 && count > 1) {
    return 2;  // most sensors are on right
  } else {
    return 1;  // most centers are in center
  }
}

double calcAnalogValue(int inRange[]) {
  int count = totalPinHigh(inRange);
  return ((count * 1023) / inCount);
}

void loop() {
  for (int i = 0; i < inCount; ++i) {
    if (digitalRead(inPins[i]) == HIGH) {
      inRange[i] = 1023;
    } else {
      inRange[i] = 0;
    }
  }

  int position = calcPositionInput(inRange);
  Serial.print(position);
  Serial.print("\n");
  Input = calcAnalogValue(inRange);  // Get value of input ranging from 0 to 1023
  if (Input == 1023) {
    // all sensors are set to one hence both motor needs to stop
    analogWrite(analogPinOutput[0], 0);
    analogWrite(analogPinOutput[1], 0);
  } else {
    double gap = Input - Setpoint;
    if (gap <= 0) {
      //we're far from setpoint, use aggressive tuning parameters
      pid.SetTunings(aggKp, aggKi, aggKd);
    } else if (gap > 0) {
      //we're close to setpoint, use conservative tuning parameters
      pid.SetTunings(consKp, consKi, consKd);
    }
    pid.Compute();

    // Setting the analog Output values;
    if (position == 0) {
      // right motor needs to be on to turn left
      analogWrite(analogPinOutput[1], Output);
    } else if (position == 2) {
      // left motor needs to be on to turn right
      analogWrite(analogPinOutput[0], Output);
    } else if (position == 1) {
      // both left and right motor needs to be on to move forward
      analogWrite(analogPinOutput[0], Output);
      analogWrite(analogPinOutput[1], Output);
    }
  }
}
