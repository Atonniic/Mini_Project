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
    //xTaskCreatePinnedToCore(Control, "Control", 10000, NULL, 1, NULL, 0);
    //xTaskCreatePinnedToCore(Read_LDR, "Read_LDR", 10000, NULL, 1, NULL, 0); 
    xTaskCreatePinnedToCore(HTTP, "HTTP", 10000, NULL, 1, NULL, 1);
}

void loop(){
}

