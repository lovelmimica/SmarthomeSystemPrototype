#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "";
const char* password = "";

HTTPClient http;
int httpResponseCode;

String bedroomOccupiedApi = "http://api.thingspeak.com/update?api_key=3VKD465UUEGQPV7N&field1=true";
String beedroomEmptyApi = "http://api.thingspeak.com/update?api_key=3VKD465UUEGQPV7N&field1=false";

const int sensor_00 = 34;
const int sensor_01 = 35;
const int sensor_02 = 32;
int val_00 = 0;
int val_01 = 0;
int val_02 = 0;
int state = LOW;
int counter = 0;

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
  
  pinMode(sensor_00, INPUT);
  pinMode(sensor_01, INPUT);
  pinMode(sensor_02, INPUT);
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
      val_00 = digitalRead(sensor_00);
      val_01 = digitalRead(sensor_01);
      val_02 = digitalRead(sensor_02);

      if(val_00 == HIGH || val_01 == HIGH || val_02 == HIGH){
        //Motion detected
        if(state == LOW){
          Serial.println("Motion detected");
          state = HIGH;
          http.begin(bedroomOccupiedApi);
          httpResponseCode = http.GET();
          if(httpResponseCode > 0){
            Serial.println("Bedroom occupation set to occupied. ");
          }else{
            Serial.print("Error occured during bedroom occupation update. Code: ");
            Serial.println(httpResponseCode);
            Serial.print("Error text: ");
            Serial.println(http.errorToString(httpResponseCode));
          }
        }
      }else{
        //Motion not detected
        if(state == HIGH){
          Serial.println("Motion stopped");
          state = LOW;
          counter = 0;
        }
        
        if(counter == 200){
          http.begin(beedroomEmptyApi);
          httpResponseCode = http.GET();
          if(httpResponseCode > 0){
            Serial.println("Bedroom occupation set to empty.");
          }else{
            Serial.print("Error occured during bedroom occupation update. Code: ");
            Serial.println(httpResponseCode);
          }
        }
        counter++;
      }
  }
  delay(200);
}
