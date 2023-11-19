/*
  Rui Santos
  Complete project details at our blog.
    - ESP32: https://RandomNerdTutorials.com/esp32-firebase-realtime-database/
    - ESP8266: https://RandomNerdTutorials.com/esp8266-nodemcu-firebase-realtime-database/
  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files.
  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
  Based in the RTDB Basic Example by Firebase-ESP-Client library by mobizt
  https://github.com/mobizt/Firebase-ESP-Client/blob/main/examples/RTDB/Basic/Basic.ino
*/

#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "PrenosniInternet"
#define WIFI_PASSWORD "Mekic2211"

// Insert Firebase project API Key
#define API_KEY "AIzaSyA7Blc7jBTfiM8M8zNYC6jeKWf9e3q6K2E"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://smart-living-dfd88-default-rtdb.europe-west1.firebasedatabase.app/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int sensor = D2;    
int led = D0;
int alarm = D1; 
int val = 0; 
int alarmOn = 0;
int lightOn = 0;
int sistemUkljucen = 1;
int blink = 0;


void setup(){
  pinMode(sensor, INPUT);
  pinMode(alarm, OUTPUT); 
  pinMode(led, OUTPUT); 
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)){
    if(Firebase.RTDB.getInt(&fbdo, "/data/sistemUkljucen")){
        if(fbdo.dataType() == "int"){
          sistemUkljucen = fbdo.intData();
       }
    }
    if(sistemUkljucen == 1){
        sendDataPrevMillis = millis();
    val = digitalRead(sensor);
    Firebase.RTDB.setInt(&fbdo, "data/sensor", val);

    if (val == HIGH) {      

    alarmOn = 1;
    lightOn = 1;
    Firebase.RTDB.setInt(&fbdo, "data/alarm", alarmOn);
    Firebase.RTDB.setInt(&fbdo, "data/light", lightOn);

    val = LOW;
    Firebase.RTDB.setInt(&fbdo, "data/sensor", val);
    }

    int izadji = true;
    while(izadji==true)
    {
      if(Firebase.RTDB.getInt(&fbdo, "/data/alarm")){
        if(fbdo.dataType() == "int"){
          alarmOn = fbdo.intData();
         }
        }

       if(Firebase.RTDB.getInt(&fbdo, "/data/light")){
        if(fbdo.dataType() == "int"){
          lightOn = fbdo.intData();
          }
        }

      Serial.println(alarmOn);
      Serial.println(lightOn);

      if(alarmOn==1){
        tone(alarm, 100);
        }
      if(lightOn==1){
          digitalWrite(led, HIGH);
        }
      
      if(alarmOn == 0){
        noTone(alarm);
        }

      if(lightOn == 0){
        digitalWrite(led, LOW);
        }

      if(alarmOn == 0 && lightOn == 0){
        izadji = false;
        }
      } 
    }
  }
}