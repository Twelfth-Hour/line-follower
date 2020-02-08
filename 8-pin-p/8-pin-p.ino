int weights[] = {-4, -3, -2, -1, 1, 2, 3, 4};
int pins[] = {2, 3, 4, 5, 7, 8, 12, 13};
int high[] = {0, 0, 0, 0, 0, 0, 0, 0};
int p = 7;
int history;
int rpm = 255;

int number_of_pins_high() {
  int n = 0;
  for (int i = 0; i < 8; ++i) {
    n += high[i];
  }
  return n;
}

double get_centroid() {
  double score = 0;
  int n = 0;
  for (int i = 0; i < 8; ++i) {
    if (high[i] == 1) {
      ++n;
      score += weights[i];
    }
  }
  if (n == 0) {
    n = 1;
  }
  return score / (double)n;
}

void read_pins() {
  for (int i = 0; i < 8; ++i) {
    if (digitalRead(pins[i]) == HIGH) {
      high[i] = 1;
    } else {
      high[i] = 0;
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < 8; ++i) {
    pinMode(pins[i], INPUT);
  }
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  read_pins();
  double c = 10 * get_centroid();
  if (number_of_pins_high() == 8) {
    analogWrite(9, 0);
    analogWrite(10, 0);
    Serial.print("8 PINS HIGH \n");
  } else {
    if (number_of_pins_high() == 0) {
      Serial.print("History \n");
      c = history;
    } else {
      history = c;
    }
    if (c < 0) {
      c *= -1;
      analogWrite(9, rpm);
      analogWrite(10, (rpm - p * c) < 0 ? 0 : (rpm - p * c));
    } else {
      analogWrite(10, rpm);
      analogWrite(9, (rpm - p * c) < 0 ? 0 : (rpm - p * c));
    }
  }
}