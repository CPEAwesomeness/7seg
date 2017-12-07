/* -----------------------------------------------------------------------------

FILE:              7seg.ino

DESCRIPTION:       Main Arduino sketch file for 7seg project

DEVELOPED WITH:    avr-gcc 4.9.2

NOTES:             Using Arduino-Makefile (github.com/sudar/Arduino-Makefile)

VERSION:           none (yet)

----------------------------------------------------------------------------- */
// Counter clock interval (ms)
const int clkSpd = 167;

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
const char clkOut = 6;

// Button input pin
const char bIn = 14;

// Global variables
unsigned long clkLast = 0;
unsigned char onesNow = 0;
unsigned char tensNow = 0;

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
//*--- implement conditional here for current switch mode, initialize
//*--- those pins first
	for (unsigned char i = 0; i < 4; ++i) {
		pinMode(ones[i], OUTPUT);
		pinMode(tens[i], OUTPUT);
	};

	// Initialize the counter clock pin
	pinMode(clkOut, OUTPUT);

	// Initialize the button pin
	pinMode(bIn, INPUT_PULLUP);
}

/* -----------------------------------------------------------------------------
FUNCTION:          loop()
DESCRIPTION:       Main loop function
RETURNS:           void
NOTES:             
----------------------------------------------------------------------------- */
void loop() {
	// Monitor the clock interval
	unsigned long clkNow = millis();

	if (clkNow > clkLast + clkSpd) {
		// This is the rising edge of the counter clock
		digitalWrite(clkOut, HIGH);

		// Read the bNow state at the rising edge of the clock (active low)
		bool bNow = !digitalRead(bNow);

		/* This conditional section represents the next, incomming state--------
		----*/
		if (onesNow == 0) {
			if (tensNow == 0) tensNow = 9;
			else --tensNow;
			onesNow = 9;
		}
		// Button initiates sequence and pauses thereafter
		if ((tensNow != 9 && !bNow) || 
			(onesNow != 9 && !bNow) ||
			(tensNow == 9 && onesNow == 9 && bNow)) {
			--onesNow;
		};
		/*----
		----------------------------------------------------------------------*/

		// Update the output pins
		outputDigit(ones, BCD[onesNow]);
		outputDigit(tens, BCD[tensNow]);

		// Reset the counter timer
		clkLast = clkNow;
	}
	// This is the falling edge of the counter clock (at half the interval)
	else if (clkNow > clkLast + clkSpd / 2) {
		digitalWrite(clkOut, LOW);
	}
}
