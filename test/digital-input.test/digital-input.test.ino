int inPinsCount = 8;
int inPins[8] = {2, 3, 4, 5, 6, 7, 8, 12};
void setup() {
  Serial.begin(9600);
  for(int i = 0; i < inPinsCount; ++i) {
    pinMode(inPins[i], INPUT);
  }
}

void loop() {
  Serial.print("CLEAR\n");
  for(int i = 0; i < inPinsCount; ++i) {
    if (digitalRead(inPins[i]) == HIGH) {
      Serial.write("PIN HIGH: ");
      Serial.print(inPins[i]);
      Serial.print("\n");
    }
  }
  delay(1000);
}
