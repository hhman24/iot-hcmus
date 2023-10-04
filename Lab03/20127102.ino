// C++ code
//
int LIGHT_PIN = A1;
int POTE_PIN = A0;
int LED_RED = 4;
int BUZZER_PIN = 5;
int PIR_PIN = 6;

void setup()
{
  	pinMode(POTE_PIN, INPUT);
  	pinMode(LIGHT_PIN, INPUT);
  	pinMode(PIR_PIN, INPUT);
  	pinMode(BUZZER_PIN, OUTPUT);
  	pinMode(LED_RED, OUTPUT);
   	Serial.begin(9600);
}

void turnLedRed()
{
  	digitalWrite(LED_RED, HIGH);
  	delay(300);
    digitalWrite(LED_RED, LOW);  
}

void buzzerRing(int i, int milliseconds)
{
	if(i)
    {
      	tone(BUZZER_PIN, 500);
      	delay(milliseconds);
      
      	tone(BUZZER_PIN, 200);
      	delay(milliseconds);
 
       	tone(BUZZER_PIN, 500);
      	delay(milliseconds);     
    }
  	else
    {
      	noTone(BUZZER_PIN);
    }
}

void loop()
{
  	int value = analogRead(POTE_PIN);
	int light_value = map(value, 0, 1023, 0, 300);
  	int input_light_value = analogRead(LIGHT_PIN);
  	int pir_value = digitalRead(PIR_PIN);
  	if(input_light_value >= light_value)
    {
      	digitalWrite(LED_RED, LOW);
      	if(pir_value)
        {
          	buzzerRing(1, 1000);
        }
      	else
        {
          	buzzerRing(0,0);
        }
    }
  	else
    {
      	digitalWrite(LED_RED, HIGH);
      	if(pir_value)
        {	
          	turnLedRed();
          	buzzerRing(1, 500);
        }
      	else
        {
          	buzzerRing(0,0);
        }
    }
  

}

