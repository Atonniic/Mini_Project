#include <Arduino.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const String baseUrl = "รอลิ๊ง";

extern int mode_kit; //mode 0-2
extern int mode_bed; //mode 0-2
extern int mode_lou; //mode 0-2

extern bool light_kit_on; //is_on true/false
extern bool light_bed_on; //is_on true/false
extern bool light_lou_on; //is_on true/false

extern int light_kit_value; //brightness 0-3
extern int light_bed_value; //brightness 0-3
extern int light_lou_value; //brightness 0-3

extern int ldr; //sensor_status 0-255

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
        POST_value(0, mode_bed, light_bed_on, light_bed_value, ldr);
        POST_value(1, mode_kit, light_kit_on, light_kit_value, ldr);
        POST_value(2, mode_lou, light_lou_on, light_lou_value, ldr);
        vTaskDelay(25 / portTICK_PERIOD_MS);
    }
}