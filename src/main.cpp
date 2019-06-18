#include <Arduino.h>

#include <ESP8266WiFi.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ESP8266HTTPClient.h>

// Valores dos sensores
int preference_light;

// Pin semaforo 1 
int rv = 5;
int av = 4;
int vv = 0;

// Pin semaforo 2
int rh = 14;
int ah = 12;
int vh = 13;

void semaforo1(int t1, int t2) {
  digitalWrite(rv, HIGH);
  digitalWrite(vh, HIGH);
  delay(t1);
  digitalWrite(vh, LOW);
  digitalWrite(ah, HIGH);
  delay(t2);
  digitalWrite(rv, LOW);
  digitalWrite(ah, LOW);
}

void semaforo2(int t1, int t2) {
  digitalWrite(rh, HIGH);
  digitalWrite(vv, HIGH);
  delay(t1);
  digitalWrite(vv, LOW);
  digitalWrite(av, HIGH);
  delay(t2);
  digitalWrite(rh, LOW);
  digitalWrite(av, LOW);
}

void setup()
{ 
  Serial.begin(9600);
  WiFiManager wifiManager;
  wifiManager.autoConnect("NodeMCU-Arduino-PlatformIO");
  Serial.println("Connected!");

  pinMode(rv, OUTPUT);
  pinMode(av, OUTPUT);
  pinMode(vv, OUTPUT);
  pinMode(rh, OUTPUT);
  pinMode(ah, OUTPUT);
  pinMode(vh, OUTPUT);
}

void loop()
{
  HTTPClient http;  //Declare an object of class HTTPClient
 
  http.begin("http://smart-traffic-light.herokuapp.com/preference_light");  //Specify request destination
  
  int httpCode = http.GET(); //Send the request
  
  if (httpCode > 0) { //Check the returning code 
    String payload = http.getString(); //Get the request response payload
    preference_light = payload.toInt();
    Serial.println(preference_light);
  }

  http.end(); //Close connection

  // Traffic lights logic
  int th1 = 3000;
  int th2 = 400;
  
  int tv1 = 3000;
  int tv2 = 400;
  
  if (preference_light == 1) {
  	Serial.println("Raise time 1");
    th1 = th1 * 4;
    th2 = th2 * 4;
  } else if (preference_light == 2) {
    Serial.println("Raise time 2");
    tv1 = tv1 * 4;
  	tv2 = tv2 * 4;
  }
  
  semaforo1(th1, th2);
  semaforo2(tv1, tv2);
}
