#include "WS2812.h"
#include "Repetier.h"

Adafruit_NeoPixel WS2812::ext_neopixel = Adafruit_NeoPixel(EXT_NUM_LEDS, EXT_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel WS2812::bed_neopixel = Adafruit_NeoPixel(BED_NUM_LEDS, BED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t WS2812::stat_update = 0;
uint16_t WS2812::neo_r = 0;
uint16_t WS2812::neo_g = 0;
uint16_t WS2812::neo_b = 255;
uint16_t WS2812::c_diff = 0;
uint32_t WS2812::bed_c;
uint32_t WS2812::ext_c;
uint32_t WS2812::last_bed_c;
uint32_t WS2812::last_ext_c;
float WS2812::curr_ext_temp = 0.0;
float WS2812::target_ext_temp = 0.0;
float WS2812::curr_bed_temp = 0;
float WS2812::target_bed_temp = 0;

void WS2812::setup()
{
    bed_neopixel.begin();
    bed_neopixel.show();
    ext_neopixel.begin();
    ext_neopixel.show();
}

void WS2812::handle_temperature() {
    if(HAL::timeInMilliseconds() > stat_update) {
        stat_update += 500;
        //  SET Bed LED
        if(heatedBedController.currentTemperatureC > 0) {
            if(heatedBedController.currentTemperatureC > heatedBedController.targetTemperatureC && heatedBedController.targetTemperatureC > 0) {
                curr_bed_temp = heatedBedController.targetTemperatureC;
            } else {
                curr_bed_temp = heatedBedController.currentTemperatureC;
            }
        }
        target_bed_temp = heatedBedController.targetTemperatureC;

        if(curr_bed_temp >= 30.0) {
            if(target_bed_temp > 0) {   //Heating Up
                c_diff = (curr_bed_temp - 30)*255/(target_bed_temp-30);
                if(c_diff>255) {c_diff = 255;}
            } else {  //Cooling Down
                c_diff = (curr_bed_temp-30)*6;
                if(c_diff>255) {c_diff = 255;}
            }
            neo_r = c_diff;
            neo_b = 255 - c_diff;
        } else {  //Temp is less than 30.0
            neo_r = 0;
            neo_b = 255;
        }
        last_bed_c = bed_c;
        bed_c = bed_neopixel.Color(neo_r, neo_g, neo_b);
        if(bed_c != last_bed_c) {  //only update if there's a change
            for(uint16_t i=0; i<bed_neopixel.numPixels(); i++) {
                bed_neopixel.setPixelColor(i, bed_c);
                bed_neopixel.show();
            }
        }

        //  SET Extruder LED   
        if(Extruder::current->tempControl.currentTemperatureC > 0) {
            if(Extruder::current->tempControl.currentTemperatureC > tempController[Extruder::current->id]->targetTemperatureC && tempController[Extruder::current->id]->targetTemperatureC > 0) {
                curr_ext_temp = tempController[Extruder::current->id]->targetTemperatureC;
            } else {
                curr_ext_temp = Extruder::current->tempControl.currentTemperatureC;
            }
        }
        target_ext_temp = tempController[Extruder::current->id]->targetTemperatureC;
        if(curr_ext_temp > 30.0) {
            if(target_ext_temp > 0) {
                c_diff = (curr_ext_temp - 30)*255/(target_ext_temp-30);
                if(c_diff>255) {c_diff = 255;}
            } else {
                c_diff = (curr_ext_temp-30)*2;
                if(c_diff>255) {c_diff = 255;}
            }
            neo_r = c_diff;
            neo_b = 255 - c_diff;
        } else {
            neo_r = 0;
            neo_b = 255;
        }
        last_ext_c = ext_c;
        ext_c = ext_neopixel.Color(neo_r, neo_g, neo_b);
        if(ext_c != last_ext_c) {
            for(uint16_t i=0; i<ext_neopixel.numPixels(); i++) {
                ext_neopixel.setPixelColor(i, ext_c);
                ext_neopixel.show();
            }
        }
    }
} 
void WS2812::handle_alert() {
    while(1) {
        colorWipe(ext_neopixel.Color(255, 0, 0), 50); // Red
        colorWipe(ext_neopixel.Color(0, 255, 0), 50); // Green
        colorWipe(ext_neopixel.Color(0, 0, 255), 50); // Blue
    }    
}

void WS2812::colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<ext_neopixel.numPixels(); i++) {
      ext_neopixel.setPixelColor(i, c);
      ext_neopixel.show();
      delay(wait);
  }
}
