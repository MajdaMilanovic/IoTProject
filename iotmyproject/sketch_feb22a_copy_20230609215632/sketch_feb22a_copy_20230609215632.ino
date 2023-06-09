#include <DHT.h>
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
#define WIFI_SSID "HONOR 9X"
#define WIFI_PASSWORD "12345678"

// Insert Firebase project API Key
#define API_KEY "AIzaSyD5XvBEDm6yjXdbqDZ7p3y-C9WzH0UvkK0"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://myproject-1ff6c-default-rtdb.europe-west1.firebasedatabase.app/" 

#define BUTTON_PIN
//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
DHT dht;
const int plava=D1;
const int crvena=D2;
const int zelena=D3;
bool signupOK = false;
bool red=false;
bool blue=false;
int tempGranica=0;
int vlaznostGranica =0;
int senzor=D0;
void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  pinMode(senzor,INPUT);
  pinMode(plava,OUTPUT);
  pinMode(crvena,OUTPUT);
  pinMode(zelena,OUTPUT);
  pinMode(BUTTON_PIN, OUTPUT);

  dht.setup(senzor);
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
    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();
    if (Firebase.RTDB.getInt(&fbdo, "/vrijednosti/TemperaturnaGranica")) {
      if (fbdo.dataType() == "int") {
        tempGranica = fbdo.intData();
        Serial.println(tempGranica);
        Serial.println("POVUKLO");
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
    }
    if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    if (Firebase.RTDB.getInt(&fbdo, "/vrijednosti/VlaznostGranica")) {
      if (fbdo.dataType() == "int") {
        vlaznostGranica = fbdo.intData();
        Serial.println(vlaznostGranica);
        Serial.println("POVUKLO VLAZNOST");
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
    }
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // Write an Int number on the database path test/int
    float temperatura=dht.getTemperature();
    if(isnan(temperatura))
    {
      Serial.println("Greska kod ocitanja temperature ");
    }
    if (Firebase.RTDB.setFloat(&fbdo, "vrijednosti/temperatura", temperatura)){
      Serial.println("Prošlo temperatura");

    }
    else {
      Serial.println("Greska");
      Serial.println("REASON: " + fbdo.errorReason());
    }
    
    float vlaznost=dht.getHumidity();
    if(isnan(vlaznost))
    Serial.println("Greska kod ocitanja vlaznosti");
    if (Firebase.RTDB.setFloat(&fbdo, "vrijednosti/vlaznost", 0.01 + vlaznost)){
      Serial.println("Proslo vlaznost");
     
    }
    else {
      Serial.println("greska");
      Serial.println("REASON: " + fbdo.errorReason());
    }
// if(temperatura>tempGranica)
//     {
//     Serial.println("Preslo granicu");
//     digitalWrite(crvena,HIGH);
//     red=true;
//     }
//     else{
//     Serial.println("Temperatura je uredu");
//     digitalWrite(crvena,LOW);
//     red=false;
//     }

//     if(vlaznost > vlaznostGranica){
//     Serial.println("Preslo granicu");
//     digitalWrite(plava,HIGH);
//     blue=true;
//     }    
//     else{
//     Serial.println("Vlaznost je uredu");
//     digitalWrite(plava,LOW);
//     blue =false;
//     }
// if(!red&&!blue)
// digitalWrite(zelena,HIGH);
// else
// digitalWrite(zelena,LOW);
    // Write an Float number on the database path test/float

  }
  if{ (temperatura<tempGranica && vlaznost <vlaznostGranica)
    digitalWrite(crvena, HIGH);
    digitalWrite(plava, HIGH);
    digitalWrite(zelena, HIGH);
  }
  else {
    digitalWrite(crvena, LOW);
     digitalWrite(plava, LOW);
    digitalWrite(zelena, LOW);
  }
}
}