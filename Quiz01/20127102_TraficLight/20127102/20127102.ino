// C++ code
//

int redLed = 4;
int greenLed = 3;
int yellowLed = 2;


void setup()
{
  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);
  pinMode(greenLed, OUTPUT);
}

void loop()
{
  digitalWrite(redLed, HIGH);
  delay(7000); // Red Led on 7s 
  digitalWrite(redLed, LOW); // Red Led off
  
  digitalWrite(greenLed, HIGH);
  delay(10000); // Green Led on 10s  
  digitalWrite(greenLed, LOW); // Red Led off
  
  digitalWrite(yellowLed, HIGH);
  delay(3000); // Yellow Led on 3s  
  digitalWrite(yellowLed, LOW); 
  
}