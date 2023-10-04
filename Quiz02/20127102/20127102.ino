// C++ code
//
int LED_RED = 4;
int LIGHT_PIN = A0;

void setup()
{
  pinMode(LED_RED, OUTPUT);
  pinMode(LIGHT_PIN, INPUT);
  Serial.begin(9600);
}

void loop()
{
	int val = analogRead(LIGHT_PIN);
  	Serial.println(val);
  	if(val < 500)
    {
      	digitalWrite(LED_RED, HIGH);
    }
  	else
    {
      	digitalWrite(LED_RED, LOW);
    }
}