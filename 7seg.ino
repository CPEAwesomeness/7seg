/* -----------------------------------------------------------------------------

FILE:              7seg.ino

DESCRIPTION:       Main Arduino sketch file for 7seg project

DEVELOPED WITH:    avr-gcc 4.9.2

NOTES:             Using Arduino-Makefile (github.com/sudar/Arduino-Makefile)

VERSION:           none (yet)

----------------------------------------------------------------------------- */
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
	5,
	4,
	3,
	2
};

// Output ports for tens place
const char tens[4] = {
	9,
	8,
	7,
	16
};

// Hardware clock pin
const char counter_clock = 6;

// Global state variables
unsigned long counter_last = 0;
unsigned char ones_last = 0;
unsigned char tens_last = 0;

/* -----------------------------------------------------------------------------
FUNCTION:          outputDigit()
DESCRIPTION:       Prints a BCD code to one of the port sequences via loop
RETURNS:           void
NOTES:             
----------------------------------------------------------------------------- */
void outputDigit(const char* place, const bool* digit) {
	for (unsigned char i = 0; i < 4; ++i) digitalWrite(place[i], digit[i]);
}

/* -----------------------------------------------------------------------------
FUNCTION:          setup()
DESCRIPTION:       Initialize the environment on startup
RETURNS:           void
NOTES:             
----------------------------------------------------------------------------- */
void setup() {
	// This loop initializes the port pins we chose as outputs
	for (unsigned char i = 0; i < 4; ++i) {
		pinMode(ones[i], OUTPUT);
		pinMode(tens[i], OUTPUT);
	};

	// Initialize the counter clock pin
	pinMode(counter_clock, OUTPUT);
}

/* -----------------------------------------------------------------------------
FUNCTION:          loop()
DESCRIPTION:       Main loop function
RETURNS:           void
NOTES:             
----------------------------------------------------------------------------- */
void loop() {
	// This section is controlled by the counter_interval timing
	unsigned long counter_current = millis();
	if (counter_current >= counter_last + counter_interval) {
		// This is the rising edge of the counter clock
		digitalWrite(counter_clock, 1);

		// Update the output pins
		outputDigit(ones, BCD[ones_last]);
		outputDigit(tens, BCD[tens_last]);

		// This conditional section handles carrys and resets
		if (ones_last == 0) {
			if (tens_last == 0) tens_last = 9;
			else --tens_last;
			ones_last = 9;
		}
		else --ones_last;

		// Reset the counter timer
		counter_last = counter_current;
	}
	// This is the falling edge of the counter clock (at half the interval)
	else if (counter_current >= counter_last + counter_interval / 2) {
		digitalWrite(counter_clock, 0);
	}
}
