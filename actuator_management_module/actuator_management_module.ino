#include <ArduinoJson.h>
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "";
const char* password = "";

HTTPClient http;
int httpResponseCode;

String smokeSensorStatusApi = "http://api.thingspeak.com/channels/1189072/feeds.json?api_key=31AIFTXDB7583BP0";
String airQualityLogApi = "http://api.thingspeak.com/channels/1199842/fields/1.json?api_key=OWAJG6IJS9ZVTXO2";
String bedroomLightStatusApi = "http://api.thingspeak.com/channels/1199845/feeds.json?api_key=1F4CJVZV752JQG39";
String bedroomOccupancyApi = "http://api.thingspeak.com/channels/1199848/feeds.json?api_key=MAJLJ1EOJJ1BLP5F";

String smokeSensorActive;
String maxAcceptableCoStr;
int maxAcceptableCo;
String sensedCoStr;
int sensedCo;
String bedroomOccupied;
String bedroomLightsActive;

String payload;

const int ledRoomLight = 33;
const int ledCoLight = 32;

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

  pinMode(ledRoomLight, OUTPUT);
  pinMode(ledCoLight, OUTPUT);

  ledcSetup(0,1E5,12);
  ledcAttachPin(25,0);
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Starting new loop iteration");
    //Do the smoke sensor work
    http.begin(smokeSensorStatusApi);
    httpResponseCode = http.GET();
    if(httpResponseCode > 0){
      payload = http.getString();
      DynamicJsonDocument doc(4096);

      deserializeJson(doc, payload);

      JsonArray feeds = doc["feeds"];
      
      for(int i = (feeds.size() - 1); i >= 0; i--){
        smokeSensorActive = (const char*)feeds[i]["field1"];
    
        if(smokeSensorActive.equals("on")){
          Serial.println("Smoke sensor is on. Starting co level examination");
          
          for(int j = (feeds.size() - 1); j >= 0; j--){
            maxAcceptableCoStr = (const char*)feeds[j]["field2"];
            
            if(maxAcceptableCoStr != NULL){
              maxAcceptableCo = maxAcceptableCoStr.toInt();
              break;
            }
          }
          break;
        }else if(smokeSensorActive.equals("off")){
          Serial.println("Smoke sensor is off");
          break;
        }else{
          Serial.print("Smoke sensor status invalid value: ");
          Serial.println(smokeSensorActive);
        }    
      }      
    }else{
      Serial.print("Error occured during smoke sensor status data fetch. Code: ");
      Serial.println(httpResponseCode);
      Serial.print("Error text: ");
      Serial.println(http.errorToString(httpResponseCode));
    }
    http.end();
    http.begin(airQualityLogApi);
    httpResponseCode = http.GET();
    payload = http.getString();

    if(httpResponseCode > 0){
      Serial.println("Fetching air quality logs...");
      DynamicJsonDocument doc(10240);
      deserializeJson(doc, payload);

      JsonArray feeds = doc["feeds"];

      for(int i = (feeds.size() - 1); i >= 0; i--){
        sensedCoStr = (const char*)feeds[i]["field1"];
        if(sensedCoStr != NULL) break;
      }
      sensedCo = sensedCoStr.toInt();
        
      Serial.print("Last sensed CO level: ");
      Serial.println(sensedCo);
      Serial.print("Max acceptable CO level: ");
      Serial.println(maxAcceptableCo);

      if(sensedCo > maxAcceptableCo){
        Serial.println("CO above safety level");
        for(int i = 0; i < 20; i++){
          digitalWrite(ledCoLight, HIGH);
          //tone(25, 33);
          ledcWriteTone(0,800);
          delay(200);
          digitalWrite(ledCoLight, LOW);
          uint8_t octave = 1;
          ledcWriteNote(0,NOTE_C,octave); 
          //tone(25, 80);
          delay(200);
        }
      }
    }else{
      Serial.print("Error occured during air quality log data fetch. Code: ");
      Serial.println(httpResponseCode);
      Serial.print("Error text: ");
      Serial.println(http.errorToString(httpResponseCode));
    }

    http.end();

    //Do the bedroom light work
    http.begin(bedroomLightStatusApi);
    httpResponseCode = http.GET();
    if(httpResponseCode > 0){
      DynamicJsonDocument doc(10240);
      payload = http.getString();
      deserializeJson(doc, payload);

      JsonArray feeds = doc["feeds"];

      for(int i = (feeds.size() - 1); i >= 0; i--){
        bedroomLightsActive = (const char*)feeds[i]["field1"];
        bedroomLightsActive.trim();

        if(bedroomLightsActive.equals("on")){
          Serial.println("Bedroom lights on");
          http.end();
          http.begin(bedroomOccupancyApi);
          httpResponseCode = http.GET();

          if(httpResponseCode > 0){
            payload = http.getString();
            DynamicJsonDocument doc(10240);
            deserializeJson(doc, payload);
            
            JsonArray feeds = doc["feeds"];

            for(int i = (feeds.size() - 1); i >= 0; i--){
              bedroomOccupied = (const char*)feeds[i]["field1"];
              if(bedroomOccupied.equals("true")){
                Serial.println("Bedroom is occupied, turning lights on...");
                digitalWrite(ledRoomLight, HIGH);
                break;
              }else if(bedroomOccupied.equals("false")){
                Serial.println("Bedroom is empty, turning lights off...");
                digitalWrite(ledRoomLight, LOW);
                break;
              }else{
                Serial.print("Beedroom occupancy invalid value: ");
                Serial.println(smokeSensorActive);
              }
            }
          }else{
            Serial.print("Error occured during bedroom occupancy data fetch. Code: ");
            Serial.println(httpResponseCode);
            Serial.print("Error text: ");
            Serial.println(http.errorToString(httpResponseCode));
          }
          http.end();
          break;
        }else if(bedroomLightsActive.equals("off")){
          Serial.println("Bedroom lights off");
          digitalWrite(ledRoomLight, LOW);
          break;         
        }else{
          Serial.print("Bedroom lights status invalid value: ");
          Serial.println(bedroomLightsActive);
        }
      }
    }else{
      Serial.print("Error occured during bedroom light status fetch. Code: ");
      Serial.println(httpResponseCode);
      Serial.print("Error text: ");
      Serial.println(http.errorToString(httpResponseCode));
    }
  }
  delay(1000);
}
