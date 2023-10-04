// C++ code
//

int RED_LED = 7;
int GREEN_LED = 6;
int BLUE_LED = 5;
int ORANGE_LED = 4;
int YELLOW_LED = 3;
int WHITE_LED = 2;

int BUTTON = 12;
int STATE = LOW;
unsigned long LAST_MILLIS = 0;

void setup()
{
  	pinMode(BUTTON, INPUT);
  	Serial.begin(9600);
  
  	pinMode(RED_LED, OUTPUT);
  	pinMode(GREEN_LED, OUTPUT);
  	pinMode(BLUE_LED, OUTPUT);
  	pinMode(ORANGE_LED, OUTPUT);
  	pinMode(YELLOW_LED, OUTPUT);
  	pinMode(WHITE_LED, OUTPUT);
  
}

void stateOWYLed(int i) // tunrn on/off the orange, white, yellow led
{
    digitalWrite(ORANGE_LED, i);
  	digitalWrite(YELLOW_LED, i);
    digitalWrite(WHITE_LED, i);
}

void stateRGBLed(int i) // tuen on/off the red green blue led
{
  	digitalWrite(RED_LED, i);
  	digitalWrite(GREEN_LED, i);
  	digitalWrite(BLUE_LED, i);
}

void flickerRGBLed(int i) // do flicker led 
{
  	if(i == 1)
    {
      	digitalWrite(RED_LED, HIGH);
      	delay(1000);
      	digitalWrite(RED_LED, LOW);
      
      	digitalWrite(GREEN_LED, HIGH);
      	delay(1000);
      	digitalWrite(GREEN_LED, LOW);
      
      	digitalWrite(BLUE_LED, HIGH);
      	delay(1000);
      	digitalWrite(BLUE_LED, LOW);
    }
  	else
    {
      	digitalWrite(RED_LED, LOW);
      	digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
    }
}

void loop()
{
  	int buttonState = digitalRead(BUTTON);
  
  	if(buttonState != STATE)
  	{
      	Serial.println("Switched");
      	LAST_MILLIS = millis(); // set again millis real time
      	STATE = buttonState; // update state button 
  	}
  
  	if(buttonState == HIGH)
    {      
      	if(millis() - LAST_MILLIS >= 10000) // if keep up presssing button to 10s
        {
          	Serial.println("Button long pressed!!!");
          	stateOWYLed(1);
          	stateRGBLed(1);
        }
      	else
        {
          	Serial.println("Not pressed");
          	stateRGBLed(0);
          	stateOWYLed(1);
      		  flickerRGBLed(1);
        }
    }
  	else
    {
      	stateRGBLed(1);
      	stateOWYLed(0);
    }

}
