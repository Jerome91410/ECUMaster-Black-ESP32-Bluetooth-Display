#ifndef _DISPLAY_CONTROL_h
#define _DISPLAY_CONTROL_h

#include <Arduino.h>
#include <EMUSerial.h>
#include <TFT_eSPI.h>
#include <lvgl.h>

lv_obj_t *initDisplay();
void renderDisplay(lv_obj_t *table, EMUSerial emuSerial);

#endif