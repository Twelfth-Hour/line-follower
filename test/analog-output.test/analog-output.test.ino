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
    Serial.print("Testing for pin number: ");
    Serial.print(analogPinOutput[i]);
    Serial.print("\n");
    analogWrite(analogPinOutput[0], 0);
    analogWrite(analogPinOutput[1], 0);
    analogWrite(analogPinOutput[2], 0);
    for (int j = 0; j < 255; ++j) {
      analogWrite(analogPinOutput[i], j);
      delay(10);
    }
    for (int j = 255; j > 0; --j) {
      analogWrite(analogPinOutput[i], j);
      delay(10);
    }
  }
  delay(1000);
}
