#include <WiFi.h>
#include "DHT.h"
#include <HTTPClient.h>
#define DHT11PIN 18

WiFiClient wifi;
DHT dht(DHT11PIN, DHT11); 

const char* ssid = "070323";     
const char* password = "Anya8kali";
const char* serverAddress = "http://192.168.136.207:5000/sensor/data"; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  

  dht.begin();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while(WiFi.status()!=WL_CONNECTED){
    delay(200);
    Serial.print(".");
  }
  Serial.println("Wifi is Connected!");
}

void loop() {

  // Set URL server web
  HTTPClient http;
  http.begin(serverAddress);
  http.addHeader("Content-Type", "application/json");

  float hum = dht.readHumidity();
  String humStr = String(hum, 2);

  float temp = dht.readTemperature();
  String tempStr = String(temp, 2);

  String timestampStr = String(millis());

  String data = "{"
                "  \"humidity\": " + humStr + ","
                "  \"temperature\": " + tempStr + ","
                "  \"timestamp\": " + timestampStr +
                "}";

  // Send POST request
  int httpCode = http.POST(data);

  Serial.println(data);
  if (httpCode > 0) { // Check for the returning code
    if (httpCode == HTTP_CODE_OK) {
      Serial.println("data sent successfully");
    } else {
      Serial.println("Unexpected HTTP code: " + String(httpCode));
    }
  } else {
    // Print detailed error message
    Serial.println("Error on sending POST: " + String(http.errorToString(httpCode).c_str()));
  }
  // Tutup objek HTTPClient
  http.end();
  delay(10000);
}

