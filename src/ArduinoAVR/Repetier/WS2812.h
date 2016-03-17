#ifndef WS2812_H_INCLUDED
#define WS2812_H_INCLUDED

#include "Adafruit_NeoPixel.h"

#define NEOPIXEL_LEDS
#define EXT_NUM_LEDS 10
#define BED_NUM_LEDS 2
#define EXT_PIN 23
#define BED_PIN 21

class WS2812
{
public:
    static Adafruit_NeoPixel ext_neopixel;
    static Adafruit_NeoPixel bed_neopixel;

    static void setup();
    static void handle_temperature();
    static void handle_alert();
    static void colorWipe(uint32_t c, uint8_t wait);
  
private:
    static uint32_t stat_update;
    static uint16_t neo_r;
    static uint16_t neo_g;
    static uint16_t neo_b;
    static uint16_t c_diff;
    static uint32_t bed_c;
    static uint32_t ext_c;
    static uint32_t last_bed_c;
    static uint32_t last_ext_c;
    static float curr_ext_temp;
    static float target_ext_temp;
    static float curr_bed_temp;
    static float target_bed_temp;
};
#endif // WS2812_H_INCLUDED
