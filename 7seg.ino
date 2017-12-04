// Counter interval (ms)
const int counter_interval = 167;

// BCD definition table
const bool BCD[10][4] = {
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

// Output ports for ones place
const char ones[4] = {
	A0,
	A1,
	A2,
	A3
};

// Output ports for tens place
const char tens[4] = {
	10,
	16,
	14,
	15
};

// Global state variables
unsigned long counter_last = 0;
unsigned char ones_last = 0;
unsigned char tens_last = 0;

// This function prints a BCD code to one of the port sequences via loop
void outputDigit(const char* place, const bool* digit) {
	for (unsigned char i = 0; i < 4; ++i) digitalWrite(place[i], digit[i]);
}

void setup() {
	// This loop initializes the port pins we chose as outputs
	for (unsigned char i = 0; i < 4; ++i) {
		pinMode(ones[i], OUTPUT);
		pinMode(tens[i], OUTPUT);
	};
}

void loop() {
	// This section is controlled by the counter_interval timing
	unsigned long counter_current = millis();
	if (counter_current >= counter_last + counter_interval) {
		// Update the output pins
		outputDigit(ones, BCD[ones_last]);
		outputDigit(tens, BCD[tens_last]);

		// This conditional section handles carrys and resets
		if (ones_last == 9) {
			if (tens_last == 9) tens_last = 0;
			else ++tens_last;
			ones_last = 0;
		}
		else ++ones_last;

		// Reset the counter timer
		counter_last = counter_current;
	}
}
