/**
 * Arduino Alchemist does not support comments (yet), which is why this documentation is in a separate file.
 **/

void setup()
{
	pinMode(0, OUTPUT);		// test top of ddrD
	pinMode(7, INPUT);		// test bottom of ddrD
	pinMode(8, OUTPUT);		// test top of ddrB
	pinMode(13, OUTPUT);	// test bottom of ddrB
	pinMode(14, OUTPUT);	// test top of ddrC
	pinMode(19, INPUT);		// test bottom of ddrC
	pinMode(20, OUTPUT);	// test nonexistent ddr
}

void loop()
{
	digitalWrite(0, HIGH);	// test top of portD
	digitalWrite(7, LOW);	// test bottom of portD
	digitalWrite(8, LOW);	// test top of portB
	digitalWrite(13, HIGH);	// test bottom of portB
	digitalWrite(14, HIGH);	// test top of portC
	digitalWrite(19, LOW);	// test bottom of portC
	digitalWrite(20, LOW);	// test nonexistent port
}