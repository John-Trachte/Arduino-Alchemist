void setup()
{
	pinMode(0, OUTPUT);
	pinMode(7, INPUT);
	pinMode(8, OUTPUT);
	pinMode(13, OUTPUT);
	pinMode(14, OUTPUT);
	pinMode(19, INPUT);
	pinMode(20, OUTPUT);	
}

void loop()
{
	digitalWrite(0, HIGH);
	digitalWrite(7, LOW);
	digitalWrite(8, LOW);
	digitalWrite(13, HIGH);
	digitalWrite(14, HIGH);
	digitalWrite(19, LOW);
	digitalWrite(20, LOW);
}