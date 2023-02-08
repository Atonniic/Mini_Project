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

extern int bed_brightness;
extern int kit_brightness;
extern int lou_brightness;

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
    const String url = baseUrl + "";
    HTTPClient http;
    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode >= 200 && httpResponseCode < 300)
    {
        Serial.print("HTTP ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        deserializeJson(doc, payload);
    }
    else
    {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
    }
}

void PUT_value(int name, int sensor_status)
{
    const String url = baseUrl + "update/sensor/" + String(name) + "/" + String(sensor_status);
    HTTPClient http;
    http.begin(url);
    int httpResponseCode = http.PUT("");
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
        if (bed_mode == 0) {
            PUT_value(0, ldr);
        }
        if (kit_mode == 0) {
            PUT_value(1, ldr);
        }
        if (lou_mode == 0) {
            PUT_value(2, ldr);
        }
        vTaskDelay(25/portTICK_PERIOD_MS);
    }
}