#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h"

const char* endpoint ="https://ecse-three-led-api.onrender.com/api/state" ;


const int led1 = 15;
const int led2 = 2;
const int led3 = 4;

void setup() {
  Serial.begin(9600);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("The Bluetooth Device is Ready to Pair");
  Serial.println("Connected @");
  Serial.print(WiFi.localIP());
}

void loop() {
  
  if(WiFi.status()== WL_CONNECTED){
    HTTPClient http;
    
    String http_response;


    String route = endpoint;
    http.begin(route);
    http.addHeader("X-API-Key", API_KEY);

    int httpResponseCode = http.GET();


    if (httpResponseCode>0) {
        Serial.print("Error");
        Serial.print(httpResponseCode);
        http_response = http.getString();
        Serial.println(http_response);}
      else {
        Serial.print("Response: ");
        Serial.println(httpResponseCode);}
      http.end();

      
      StaticJsonDocument<1024> doc;
      DeserializationError error = deserializeJson(doc, http_response);

      if (error) 
      { Serial.print("deserializeJson() failed:");
        Serial.println(error.c_str());
        return;}
      
      bool light1 = doc["light_switch_1"];
      bool light2 = doc["light_switch_2"];
      bool light3 = doc["light_switch_3"];
  
      Serial.println("Light 1:");
      Serial.println(light1);
      Serial.println("Light 2:");
      Serial.println(light2);
      Serial.println("Light 3:");
      Serial.println(light3);
      digitalWrite(led1, light1);
      digitalWrite(led2,light2);
      digitalWrite(led3, light3);
      Serial.println("Lights Switched Successfully");    
  }
  else {return;}
}