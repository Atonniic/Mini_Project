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

void control(void *param) {
    while (true) {
        //Bed
        if (bed_on) {
            ledcWrite(0, bed_brightness);
        } 
        else {
            ledcWrite(0, 0);
        }
        //Kitchen
        if (kit_on) {
            ledcWrite(1, kit_brightness);
        } 
        else {
            ledcWrite(1, 0);
        }
        //lounge
        if (lou_on) {
            ledcWrite(2, lou_brightness);
        } 
        else {
            ledcWrite(2, 0);
        }
    }
}

void bed_control(void *param){
    int bed_on_last = 50;
    int bed_on_current = 50;
    while(1) {
        if (bed_mode == 0) {
            Serial.println("Mode: bed auto");
            if (ldr <= 60) {
                ledcWrite(0, bed_brightness);
                bed_on = true;
            }
        }
        else if (bed_mode == 1) {
            Serial.println("Mode: HW");
            bed_on_current = touchRead(BEDSWITCH);
            if (bed_on_current - bed_on_last < -20) {
                bed_on = !bed_on;
            }
            bed_on_last = bed_on_current;
        }
        vTaskDelay(25/portTICK_PERIOD_MS);
    }
}

void kit_control(void *param){
    int kit_on_last = 50;
    int kit_on_current = 50;
    while(1) {
        if (kit_mode == 0) {
            Serial.println("Mode: kit auto");
            if (ldr <= 60) {
                ledcWrite(0, kit_brightness);
                kit_on = true;
            }
        }
        else if (kit_mode == 1) {
            Serial.println("Mode: HW");
            kit_on_current = touchRead(KITSWITCH);
            if (kit_on_current - kit_on_last < -20) {
                kit_on = !kit_on;
            }
            kit_on_last = kit_on_current;
        }
        vTaskDelay(25/portTICK_PERIOD_MS);
    }
}

void lou_control(void *param){
    int lou_on_last = 50;
    int lou_on_current = 50;
    while(1) {
        if (lou_mode == 0) {
            Serial.println("Mode: lou auto");
            if (ldr <= 60) {
                ledcWrite(0, bed_brightness);
                bed_on = true;
            }
        }
        else if (lou_mode == 1) {
            Serial.println("Mode: HW");
            lou_on_current = touchRead(LOUSWITCH);
            if (lou_on_current - lou_on_last < -20) {
                lou_on = !lou_on;
            }
            lou_on_last = lou_on_current;
        }
        vTaskDelay(25/portTICK_PERIOD_MS);
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

    xTaskCreatePinnedToCore(control, "control", 10000, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(Read_LDR, "detect_on_off", 20000, NULL, 1, NULL, 0); 
    xTaskCreatePinnedToCore(bed_control, "bed_control", 10000, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(kit_control, "kit_control", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(lou_control, "lou_control", 10000, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(HTTP, "HTTP", 20000, NULL, 1, NULL, 1);
}

void loop(){
}

