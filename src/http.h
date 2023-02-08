#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const String baseUrl = "รอลิ๊ง";

extern int bed_mode; 
extern int kit_mode;
extern int lou_mode;

extern bool bed_on;
extern bool kit_on;
extern bool lou_on;

extern int bed_brightness ;
extern int kit_brightness ;
extern int lou_brightness ;

extern int ldr;

void Connect_Wifi()
{
    WiFi.begin("", "");
    Serial.println("Connecting to WiFi..");
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.print("OK! IP=");
    Serial.println(WiFi.localIP());
}

void GET_value()
{
    DynamicJsonDocument doc(2048);
    const String url = baseUrl + "posts/1";
    HTTPClient http;
    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode >= 200 && httpResponseCode < 300)
    {
        Serial.print("HTTP ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        deserializeJson(doc, payload);

        Serial.println();
        Serial.println((const char *)doc["title"]);
    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
}

void POST_value(int name, int mode, bool is_on, int brightness, int sensor_status)
{
    String json;
    DynamicJsonDocument doc(2048);
    doc["name"] = name; // int
    doc["mode"] = mode; //int
    doc["brightness"] = brightness; //int
    doc["sensor_status"] = sensor_status; //int
    doc["is_on"] = is_on; //bool 
    serializeJson(doc, json);

    const String url = baseUrl + "posts";
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(json);
    if (httpResponseCode >= 200 && httpResponseCode < 300)
    {
        Serial.print("Done!!");
    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
}

void HTTP(void *param){
    while (1)
    {
        GET_value();
        POST_value(0, bed_mode, bed_on, bed_brightness, ldr);
        POST_value(1, kit_mode, kit_on, kit_brightness, ldr);
        POST_value(2, lou_mode, lou_on, lou_brightness, ldr);
        vTaskDelay(25 / portTICK_PERIOD_MS);
    }
}