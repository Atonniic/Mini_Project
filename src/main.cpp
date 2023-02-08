#include <Arduino.h>
#include "http.h"

#define BED 33 // room 0
#define BEDSWITCH 2

#define KIT 26 // room 1
#define KITSWITCH 0

#define LOU 25 // room 2
#define LOUSWITCH 15

#define LDR 32

int bed_mode = 2; 
int kit_mode = 2;
int lou_mode = 2;

bool bed_on = false;
bool kit_on = false;
bool lou_on = false;

int bed_brightness = 255;
int kit_brightness = 255;
int lou_brightness = 255;

int ldr = 255;

void manual_hw_control(void *param) {
    while (true) {
        if (light_kit_on) {
            ledcWrite(0, light_kit_value);
        } else {
            ledcWrite(0, 0);
        }
        if (light_bed_on) {
            ledcWrite(1, light_bed_value);
        } else {
            ledcWrite(1, 0);
        }
        if (light_lou_on) {
            ledcWrite(2, light_lou_value);
        } else {
            ledcWrite(2, 0);
        }
    }
}

void detect_on_off(void *param) {
    int kit_last = 50;
    int kit_current = 50;
    int bed_last = 50;
    int bed_current = 50;
    int lou_last = 50;
    int lou_current = 50;
    while (true) {
        kit_current = touchRead(KITSWITCH);
        bed_current = touchRead(BEDSWITCH);
        lou_current = touchRead(LOUSWITCH);
        if (kit_current - kit_last < -20) {
            light_kit_on = !light_kit_on;
        }
        if (bed_current - bed_last < -20) {
            light_bed_on = !light_bed_on;
        }
        if (lou_current - lou_last < -20) {
            light_lou_on = !light_lou_on;
        }
        kit_last = kit_current;
        bed_last = bed_current;
        lou_last = lou_current;
        vTaskDelay(25/portTICK_PERIOD_MS);
    }
}

void bed_control(void *param){
    while(1) {
        if (bed_mode == 0) {
            Serial.println("Mode: bed auto");
            if (ldr <= 60) {
                ledcWrite(0, bed_brightness);
                Serial.println("Bed: on");
                bed_on = true;
            }
        }
        else if (bed_mode == 1) {
            
        }
        else if (bed_mode == 2) {
        
        }
    }
}

void Read_LDR(void *param) {
    while(1){
        if (bed_mode == 0 || kit_mode == 0 || lou_mode == 0) {
            ldr = map(analogRead(LDR),500,3600,0,255);
            Serial.println("LDR: " + String(ldr));
        }
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}

void setup(){
    Serial.begin(115200);
    Connect_Wifi();
    ledcSetup(0, 5000, 8);
    ledcAttachPin(BED, 0);
    ledcSetup(1, 5000, 8);
    ledcAttachPin(KIT, 1);
    ledcSetup(2, 5000, 8);
    ledcAttachPin(LOU, 2);
    xTaskCreatePinnedToCore(Read_LDR, "Read_LDR", 10000, NULL, 1, NULL, 0); 
    xTaskCreatePinnedToCore(HTTP, "HTTP", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(detect_on_off, "detect_on_off", 20000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(manual_hw_control, "control", 20000, NULL, 1, NULL, 0);
}

void loop(){
}

