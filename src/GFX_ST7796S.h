// created by Jean-Marc Zingg to be the GFX_ST7796S class for the GFX_TFT library
// code extracts taken from http://www.lcdwiki.com/res/Program/Parallel_Port/4.0inch/8_16BIT_ST7796S_MRB3951_V1.0/4.0inch_8&16BIT_Module_ST7796S_MRB3951_V1.0.zip (lcd.h, lcd.c)
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this class works with "4.0 inch HD TFT LCD Screen with PCB Board ILI9488 ST7796S Drive IC 320(RGB)*480 8Bit Parallel Interface (Touch/No Touch)" display from Maithoga
// e.g. https://www.aliexpress.com/item/1899444540.html
// this display matches the FSMC TFT connector of the STM32F407ZG-M4 board
// e.g. https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
// this display also matches the FSMC TFT connector of the STM32F407VxT6 board e.g. https://www.aliexpress.com/item/1005001806399987.html

#ifndef _GFX_ST7796S_H_
#define _GFX_ST7796S_H_

//#include <GFX_Extensions.h>
#include <GFX_IO.h>
#include "GFX_TFT_IO/GFX_TFT_IO.h"

class GFX_ST7796S : public GFX_TFT_IO
{
  public:
    GFX_ST7796S(int16_t cs_pin, int16_t dc_pin, int16_t rst_pin = -1, int16_t bl_pin = -1);
    GFX_ST7796S(int16_t cs_pin, int16_t dc_pin, int16_t mosi_pin, int16_t sclk_pin, int16_t rst_pin, int16_t bl_pin = -1);
    GFX_ST7796S(SPIClass *spi, int16_t cs_pin, int16_t dc_pin, int16_t rst_pin = -1, int16_t bl_pin = -1);
    GFX_ST7796S(GFX_TFT_IO_Plugin& plugin);
    void begin(uint32_t freq);
    void init(uint32_t freq = 0);
    void setRotation(uint8_t r);
    void invertDisplay(bool i);
    void invert(bool i);
    void enableDisplay(bool enable);
    void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  private:
    int16_t _bl_pin;
    int8_t _bgr;
};

#define GFX_TFT_CLASS GFX_ST7796S

#endif // _GFX_ST7796S_H_
