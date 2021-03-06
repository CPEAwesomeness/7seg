/* -----------------------------------------------------------------------------

FILE:              7seg.ino

DESCRIPTION:       Main Arduino sketch file for 7seg project

DEVELOPED WITH:    avr-gcc 4.9.2

NOTES:             Using Arduino-Makefile (github.com/sudar/Arduino-Makefile)

VERSION:           none (yet)

----------------------------------------------------------------------------- */
// Counter clock interval (ms)
const int clkSpd = 166;

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

// Switch pins
const char sAct = A1;
const char sPass = A3;

// Global variables
unsigned long clkLast = 0;
unsigned char onesNow = 0;
unsigned char tensNow = 0;

/* -----------------------------------------------------------------------------
FUNCTION:          disableOutput()
DESCRIPTION:       Disables the register output pins
RETURNS:           void
NOTES:             
----------------------------------------------------------------------------- */
void disableOutput() {
	// Clear the registers
	onesNow = 0;
	tensNow = 0;

	// Disable the register output pins so that the hardware can run
	for (unsigned char i = 0; i < 4; ++i) {
		pinMode(tens[i], INPUT);
		pinMode(ones[i], INPUT);
	};
}

/* -----------------------------------------------------------------------------
FUNCTION:          enableOutput()
DESCRIPTION:       Enables the register output pins
RETURNS:           void
NOTES:             
----------------------------------------------------------------------------- */
void enableOutput() {
	// Enable the register output pins
	for (unsigned char i = 0; i < 4; ++i) {
		pinMode(tens[i], OUTPUT);
		pinMode(ones[i], OUTPUT);
	};
}

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
	// Initialize the switch pins (active low) 
	pinMode(sAct, INPUT_PULLUP);
	pinMode(sPass, INPUT_PULLUP);

	// Initialize the counter clock pin
	pinMode(clkOut, OUTPUT);

	// Initialize the button pin (active low)
	pinMode(bIn, INPUT_PULLUP);

	(!sAct && sPass) ? enableOutput() : disableOutput();
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
		// Reset the clock timer
		clkLast = clkNow;

		// Switch disables Arduino output and allows hardware to run on clk
		// (active low)
		if (!digitalRead(sAct) && digitalRead(sPass)) {
			enableOutput();
			// Read the button (active low)
			bool bNow = !digitalRead(bIn);

			// This conditional section represents the next, incomming state----
			if (onesNow == 0) {
				tensNow == 0 ? tensNow = 9 : --tensNow;
				onesNow = 9;
			}
			// Button initiates sequence and pauses thereafter
			if ((tensNow != 9 && !bNow) || 
				(onesNow != 9 && !bNow) ||
				(tensNow == 9 && onesNow == 9 && bNow)) {
				--onesNow;
			};
			//------------------------------------------------------------------

			// Update the output pins
			outputDigit(ones, BCD[onesNow]);
			outputDigit(tens, BCD[tensNow]);
		}
		else disableOutput();
	}
	// This is the falling edge of the counter clock (at half the interval)
	else if (clkNow > clkLast + clkSpd / 2) {
		digitalWrite(clkOut, LOW);
	}
}
