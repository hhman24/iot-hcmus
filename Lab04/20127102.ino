// C++ code
//
/*
  LiquidCrystal Library - Hello World

   Demonstrates the use of a 16x2 LCD display.
  The LiquidCrystal library works with all LCD
  displays that are compatible with the  Hitachi
  HD44780 driver. There are many of them out
  there, and you  can usually tell them by the
  16-pin interface.

  This sketch prints "Hello World!" to the LCD
  and shows the time.

  The circuit:
  * LCD RS pin to digital pin 12
  * LCD Enable pin to digital pin 11
  * LCD D4 pin to digital pin 5
  * LCD D5 pin to digital pin 4
  * LCD D6 pin to digital pin 3
  * LCD D7 pin to digital pin 2
  * LCD R/W pin to ground
  * LCD VSS pin to ground
  * LCD VCC pin to 5V
  * 10K resistor:
  * ends to +5V and ground
  * wiper to LCD VO pin (pin 3)

  Library originally added 18 Apr 2008  by David
  A. Mellis
  library modified 5 Jul 2009  by Limor Fried
  (http://www.ladyada.net)
  example added 9 Jul 2009  by Tom Igoe
  modified 22 Nov 2010  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/LiquidCrystal
*/

#include <LiquidCrystal.h>

int BUTTON_PIN = 6;
int TEM_PIN = A0;
int LAST_STAE_BUTTON = LOW;
int BUTTON_COUNT = 1;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

float getTemperature()
{
  	float value = analogRead(TEM_PIN);
  	float celsius = (value * 5 / 1023) / 0.01 - 50;
  	return celsius;
}

void show1()
{
  	lcd.setCursor(5, 0);
  	lcd.print("CLASS");
  	lcd.setCursor(4, 1);
  	lcd.print("20CLC01");
  	delay(100);
}

void show2()
{
  	lcd.setCursor(0, 0);
  	lcd.print("HOANG H. MINH AN");
  	lcd.setCursor(4, 1);
  	lcd.print("20127102");
  	delay(100);
}

void show3()
{
  	float t = getTemperature();
	lcd.setCursor(2, 0);
  	lcd.print("TEM:");
  	lcd.setCursor(7, 0);
  	lcd.print(t);
  	delay(100);
}

void setup()
{
 	lcd.begin(16, 2); // Set up the number of columns and rows on the LCD.
	pinMode(TEM_PIN, INPUT);
  	pinMode(BUTTON_PIN, INPUT);
  	Serial.begin(9600);
}

void loop()
{  
  	if(digitalRead(BUTTON_PIN) == HIGH)
    {
        lcd.clear();
       	BUTTON_COUNT++;
        if(BUTTON_COUNT > 3)
    	{
          BUTTON_COUNT = 1;
    	}
    	Serial.println(BUTTON_COUNT);
    }
  	if(BUTTON_COUNT == 1)
    {
      	show1();
    }
  
  	if(BUTTON_COUNT == 2)
    {	
      	lcd.noDisplay();
      	delay(100);
      	show2();
        lcd.display();
      	delay(100);
    }
  
  	if(BUTTON_COUNT == 3)
    {
      	show3();
    }
}




