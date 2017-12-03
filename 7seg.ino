bool BCD[10][4] = {
  {0, 0, 0, 0},
  {0, 0, 0, 1},
  {0, 0, 1, 0},
  {0, 0, 1, 1},
  {0, 1, 0, 0},
  {0, 1, 0, 1},
  {0, 1, 1, 0},
  {0, 1, 1, 1},
  {1, 0, 0, 0},
  {1, 0, 0, 1}
};

char ones[4] = {
  A0,
  A1,
  A2,
  A3
};

char tens[4] = {
  10,
  16,
  14,
  15
};

void output_digit(char* place, bool* digit) {
  for (char i = 0; i < 4; ++i) {
    digitalWrite(place[i], digit[i]);
  };
}

void setup() {
  for (char i = 0; i < 4; ++i) {
    pinMode(ones[i], OUTPUT);
    pinMode(tens[i], OUTPUT);
  };
}

void loop() {
  for (char i = 0; i < 100; ++i) {
    output_digit(tens, BCD[i / 10]);
    output_digit(ones, BCD[i % 10]);
    delay(166);
  }
}
