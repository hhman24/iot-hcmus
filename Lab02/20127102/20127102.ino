// C++ code
//

int BUZZER_PIN = 5;
int TRIG_PIN = 2;
int ECHO_PIN = 3;
int LED_BLUE = 10;
int LED_RED = 9;
int LED_ORANGE = 8;

void setup()
{
  	Serial.begin(9600);
  	pinMode(TRIG_PIN, OUTPUT);
  	pinMode(ECHO_PIN, INPUT);
  	pinMode(BUZZER_PIN, OUTPUT);
  	pinMode(LED_BLUE, OUTPUT);
  	pinMode(LED_RED, OUTPUT);
  	pinMode(LED_ORANGE, OUTPUT);
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

long getDistance()
{
  	digitalWrite(TRIG_PIN, LOW);
  	delayMicroseconds(2);
  	digitalWrite(TRIG_PIN, HIGH);
  	delayMicroseconds(10);
  	digitalWrite(TRIG_PIN, LOW);
  
  
  	long duration = pulseIn(ECHO_PIN, HIGH);
  	long distanceCm = duration*0.034/2;
  	
  	return distanceCm;
}

void loop()
{
  	int distanceCm = getDistance();
  	Serial.println(distanceCm);
  
  	if(distanceCm >= 150)
    {
        digitalWrite(LED_RED, LOW);
        digitalWrite(LED_ORANGE, LOW);
      	buzzerRing(0,0);
      
      	digitalWrite(LED_BLUE, HIGH);
    }
  	else if(50 < distanceCm && distanceCm < 150)
    {
      	digitalWrite(LED_BLUE, LOW);
        digitalWrite(LED_ORANGE, LOW);
      
      	digitalWrite(LED_RED, HIGH);
      	delay(1000);
        digitalWrite(LED_RED, LOW);
      	buzzerRing(1,1000);
    }
  	else
    {
      	int m = distanceCm;
      	
      	digitalWrite(LED_BLUE, HIGH);
      	digitalWrite(LED_RED, HIGH);
        digitalWrite(LED_ORANGE, HIGH);
      	delay(m);
      	digitalWrite(LED_BLUE, LOW);
      	digitalWrite(LED_RED, LOW);
        digitalWrite(LED_ORANGE, LOW);
      
      	buzzerRing(1,m);
    }
	
}





















