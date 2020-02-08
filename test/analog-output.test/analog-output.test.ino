int outPinsCount = 3;
int analogPinOutput[3] = {9, 10, 11};
int analogPinInputTest[3] = {A1, A2, A3};
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < outPinsCount; ++i) {
    pinMode(analogPinOutput[i], OUTPUT);
  }
}

void loop() {
  Serial.print("CLEAR\n");
  for (int i = 0; i < outPinsCount; ++i) {
    analogWrite(analogPinOutput[i], 255);
  }
  delay(100);
}
