
//FirebaseESP8266.h must be included before ESP8266WiFi.h
#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>  // The NTP client starts a time-request exchange with the NTP server
#include <TimeLib.h>    // sending and receiving UDP messages WiFiUdp.h library is used        


#define ENA   5          // Điều khiển tốc độ động cơ bên trái     GPIO5(D1)
#define ENB   12          // Điều khiển tốc độ động cơ bên phải    GPIO12(D6)
#define IN1  4          // L298N in1 Động cơ trái quay             GPIO4(D2)
#define IN2  0          // L298N in2 Động cơ trái quay ngược lại   GPIO0(D3)
#define IN3  2           // L298N in3 Động cơ phải quay            GPIO2(D4)
#define IN4  14           // L298N in4 Động cơ phải quay ngược lại GPIO14(D5)

#define TRIG_PIN D7
#define ECHO_PIN D8

#define TEMPERATURE  A0 // nhiệt độ

#define FIREBASE_HOST "deliverybot-55058-default-rtdb.firebaseio.com/" //Without http:// or https:// schemes
#define FIREBASE_AUTH "43HIl5VpxpTgCA26CB5xaMuawFYE3Qx8yvec2KCN"
#define WIFI_SSID "BAE CITY LAND 2"
#define WIFI_PASSWORD "79797979"

// hiện thông báo
const char* host = "maker.ifttt.com";
const int port = 80;
const char* request1 = "/trigger/ABB/with/key/b-eoMvXt9-_y6xhWgMKGMm";
const char* request2 = "/trigger/Temperature/with/key/b-eoMvXt9-_y6xhWgMKGMm";

//Define FirebaseESP8266 data object
FirebaseData firebaseData;

FirebaseJson json;

int toi,luii,trai,phai;
int tocdoxe = 150;         // 400 - 1023.

// Đo thời gian 
WiFiUDP ntpUDP;

NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 25200, 60000); // make changes in the line below to match your time zone.
// By default 'pool.ntp.org' is used with 60 seconds update interval and no offse

String Time = "Time000000";
String Date = "Date00002000";
byte last_second, second_, minute_, hour_, day_, month_;
int year_;

// do nhiệt độ
int t = 0;
void setup()
{
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT); 
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(TRIG_PIN, OUTPUT); // của cảm biến khoảng cách
  pinMode(ECHO_PIN, INPUT);

  pinMode(TEMPERATURE, INPUT);
  
  Serial.begin(115200);

  toi=0;
  luii=0;
  trai=0;
  phai=0;
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  // lấy thời gian
  timeClient.begin();
}

void loop()
{
  detectionObject();
  getTemperature();
  controller();
  getTime();
}

/**************Lấy thời gian thực ******************/
void getTime()
{
  timeClient.update();
  unsigned long unix_epoch = timeClient.getEpochTime();    // Get Unix epoch time from the NTP server returns the Unix epoch
  // which are the seconds elapsed since 00:00:00 UTC on 1 January 1970

  second_ = second(unix_epoch); 
  if (last_second != second_) {
 

    minute_ = minute(unix_epoch); // the minute now , time elements are functions returning int
    hour_   = hour(unix_epoch); // the hour now
    day_    = day(unix_epoch); // the second now
    month_  = month(unix_epoch); // the month now
    year_   = year(unix_epoch); // the full four digit year

 

    Time[9] = second_ % 10 + 48;
    Time[8] = second_ / 10 + 48;
    Time[7]  = minute_ % 10 + 48;
    Time[6]  = minute_ / 10 + 48;
    Time[5]  = hour_   % 10 + 48;
    Time[4]  = hour_   / 10 + 48;

 

    Date[4]  = day_   / 10 + 48;
    Date[5]  = day_   % 10 + 48;
    Date[6]  = month_  / 10 + 48;
    Date[7]  = month_  % 10 + 48;
    Date[10] = (year_   / 10) % 10 + 48;
    Date[11] = year_   % 10 % 10 + 48;

    Serial.println(Time);
    Serial.println(Date);

    last_second = second_;

  }
}

/**************Nhiệt độ      *****************/
void sendRequests2()
{
  WiFiClient client;
  while(!client.connect(host, port))
  {
      Serial.println("connect fail");
      delay(1000);
  }

  client.print(String("GET ") + request2 + " HTTP/1.1\r\n"
               + "Host: " + host + "\r\n"
               + "Connection: close\r\n\r\n");

  delay(500);

  while(client.available())
  {
      String line = client.readStringUntil('r');
      Serial.print(line);
  }   
}

void getTemperature()
{
    float value = analogRead(TEMPERATURE);
    int celsius = (value * 5 / 1023) / 0.01 - 50;
    t = 273 - celsius;
    Firebase.setInt(firebaseData, "/Controll/temp", t);
    Serial.print("Temp: ");
    Serial.println(t);
    if(t > 40)
    {
        // send thong bao
        sendRequests2();
        Serial.println("Nhiệt độ quá cao");
    }
}

/*********Phát hiện vật cản***************************************************************************/
void sendRequests1()
{
  WiFiClient client;
  while(!client.connect(host, port))
  {
      Serial.println("connect fail");
      delay(1000);
  }

  client.print(String("GET ") + request1 + " HTTP/1.1\r\n"
               + "Host: " + host + "\r\n"
               + "Connection: close\r\n\r\n");

  delay(500);

  while(client.available())
  {
      String line = client.readStringUntil('r');
      Serial.print(line);
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

void detectionObject()
{
      int distanceCm = getDistance();
      Serial.print("Distance: ");
      Serial.println(distanceCm);
      if(distanceCm <= 100)
      {
              Firebase.setInt(firebaseData, "/Controll/dis", distanceCm);
              if(distanceCm <= 20)
              {
                tocdoxe = tocdoxe + 50;
                dung();
                lui();
                delay(500);
                dung();
                sendRequests1();
                if(Firebase.getInt(firebaseData, "/delivery/numberOf") == true)
                {
                  int count = firebaseData.intData();
                  Serial.println("Đang dừng gặp vật cản");
                  String s =  Date + "" + Time + "" + "Temperature" + String(t);
                  Serial.println(s);
                  Firebase.setString(firebaseData, "/delivery/good/" + String(count), s);
                  Firebase.setInt(firebaseData, "/delivery/numberOf", count + 1);
                }
              }
              tocdoxe = 150;
      }
}

/****** Điều Khiển Xe *****************************************************/
void controller()
{
  if (Firebase.getString(firebaseData, "/Controll/up") == true){
      //Serial.println(firebaseData.stringData());
      if (firebaseData.stringData().indexOf("ON")>=0 ) {
        tien(); toi=1;
        Serial.println("Đang đi tới");
        Serial.println(firebaseData.stringData());
        return;
      }
      else if (firebaseData.stringData().indexOf("OFF")>=0 && toi==1){
        dung();toi=0;Serial.println("Đang dừng toi");
        Serial.println(firebaseData.stringData());
        return;
      }
    }
  if (Firebase.getString(firebaseData, "/Controll/down")==true){
      //Serial.println(firebaseData.stringData());
      if (firebaseData.stringData().indexOf("ON")>=0) {
        lui();luii=1;
        Serial.println("Đang lùi");
        Serial.println(firebaseData.stringData());
        return;
      }
      else if (firebaseData.stringData().indexOf("OFF")>=0 && luii==1){
        dung();luii=0;Serial.println("Đang dừng lui");
        Serial.println(firebaseData.stringData());
        return;
      }
    }
 if (Firebase.getString(firebaseData, "/Controll/left")==true){
      //Serial.println(firebaseData.stringData());
      if (firebaseData.stringData().indexOf("ON")>=0) {
        retrai();trai=1;
        Serial.println("Đang rẽ trái");
        delay(500);
        dung();
        Serial.println(firebaseData.stringData());
        return;
      }
      else if (firebaseData.stringData().indexOf("OFF")>=0 && trai == 1){
        dung();trai=0;Serial.println("Đang dừng trai");
        Serial.println(firebaseData.stringData());
        return;
      }
    }
  if (Firebase.getString(firebaseData, "/Controll/right")==true){
      //Serial.println(firebaseData.stringData());
      if (firebaseData.stringData().indexOf("ON")>=0) {
        rephai();phai=1;
        Serial.println("Đang rẽ phải");
        delay(500);
        dung();
        Serial.println(firebaseData.stringData());
        return;
      }
      else if (firebaseData.stringData().indexOf("OFF")>=0 && phai == 1){
        dung();phai=0;Serial.println("Đang dừng phai");
        Serial.println(firebaseData.stringData());
        return;
      }
    }    
}

/********************************************* lui lai *****************************************************/
void lui()
{
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENA, tocdoxe);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      analogWrite(ENB, tocdoxe);
}
/********************************** tien toi ******************************************/
void tien()
{
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, tocdoxe);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, tocdoxe);
}
/********************************** Dừng lại ******************************************/
void dung()
{
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, 0);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, 0);
}
/********************************** Rẽ trái ******************************************/
void rephai() //rephai()
{
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, 120);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, 120);
}
/********************************** Rẽ phải ******************************************/
void retrai()
{
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, 120);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      analogWrite(ENB, 120);
}
