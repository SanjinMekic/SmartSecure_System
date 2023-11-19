#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID "YOUR_WIFI"
#define WIFI_PASSWORD "YOUR_PASSWORD"

#define API_KEY "YOUR_API_KEY"

#define DATABASE_URL "YOUR_DATABASE_URL" 

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

  config.api_key = API_KEY;

  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback; 
  
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
