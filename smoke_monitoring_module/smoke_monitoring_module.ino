#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "";
const char* password = "";

HTTPClient http;
String payload;

String sensorStatusApi = "http://api.thingspeak.com/channels/1189072/fields/1.json?api_key=31AIFTXDB7583BP0";
String coLevelUpdateApi = "http://api.thingspeak.com/update?api_key=YFIW1E3ZEW1VGOFC&field1=";

void setup() {
    Serial.begin(115200); 

    WiFi.begin(ssid, password);
    Serial.println("Connecting");
    while(WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to WiFi network with IP Address: ");
    Serial.println(WiFi.localIP());

    pinMode(34, INPUT);
}

void loop() {
  if(WiFi.status()== WL_CONNECTED){
      http.begin(sensorStatusApi.c_str());

      int httpResponseCode = http.GET();

      if (httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        payload = http.getString();

        DynamicJsonDocument doc(2048); 
        deserializeJson(doc, payload);
        
        JsonArray feeds = doc["feeds"];

        int index = feeds.size() - 1;
        
        for(int i = index; i >= 0 ; i--){
            String sensor_active = feeds[i]["field1"];          
            sensor_active.trim();
           
            if(sensor_active.equals("on")){
              Serial.println("Smoke sensor is on. Starting measuring iteration.");
              int coLevel = analogRead(34);
              String requestUrl = coLevelUpdateApi + String(coLevel);

              http.end();
              http.begin(requestUrl.c_str());
              httpResponseCode = http.GET();

              if(httpResponseCode > 0){
                Serial.print("CO level updated. Value: ");
                Serial.println(coLevel);
              }else{
                Serial.print("Error occured during CO level update. HTTP response code: ");
                Serial.println(httpResponseCode);
              }
              break;
            }else if(sensor_active.equals("off")){
              Serial.println("Smoke sensor is off");
              break;           
            }else{
              Serial.print("Sensor status invalid value: ");
              Serial.println(sensor_active);
            }                
        } 
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
      delay(10000);
  }
}
