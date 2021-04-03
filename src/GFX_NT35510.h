// created by Jean-Marc Zingg to be the GFX_NT35510 class for the GFX_TFT library
// code extracts taken from http://www.lcdwiki.com/res/Program/Parallel_Port/3.97inch/8_16BIT_NT35510_800x480_MRB3973_V1.2/3.97inch_8&16BIT_Module_NT35510_800x480_MRB3973_V1.2.zip (lcd.h, lcd.c)
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this class works with "maithoga IPS 3.97 inch 34P 16.7M HD TFT LCD Screen with Adapter Board (Touch/No Touch) NT35510 Drive IC 480(RGB)*800" display from Ruijia Industry
// e.g. https://www.aliexpress.com/item/32676929794.html
// this display matches the FSMC TFT connector of the STM32F407ZG-M4 board
// e.g. https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
// this display also matches the FSMC TFT connector of the STM32F407VxT6 board e.g. https://www.aliexpress.com/item/1005001806399987.html
//
// note: this display class uses 16bit commands, for 16 bit solder jumper settings

#ifndef _GFX_NT35510_H_
#define _GFX_NT35510_H_

//#include <GFX_Extensions.h>
#include <GFX_IO.h>
#include "GFX_TFT_IO/GFX_TFT_IO.h"

class GFX_NT35510 : public GFX_TFT_IO
{
  public:
    GFX_NT35510(int8_t cs_pin, int8_t dc_pin, int8_t rst_pin = -1, int8_t bl_pin = -1);
    GFX_NT35510(int8_t cs_pin, int8_t dc_pin, int8_t mosi_pin, int8_t sclk_pin, int8_t rst_pin, int8_t bl_pin = -1);
    GFX_NT35510(SPIClass *spi, int8_t cs_pin, int8_t dc_pin, int8_t rst_pin = -1, int8_t bl_pin = -1);
    GFX_NT35510(GFX_TFT_IO_Plugin& plugin);
    void begin(uint32_t freq);
    void init(uint32_t freq = 0);
    void setRotation(uint8_t r);
    void invertDisplay(bool i);
    void invert(bool i);
    void enableDisplay(bool enable);
    void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  private:
    int8_t _bl_pin;
    int8_t _bgr;
};

#define GFX_TFT_CLASS GFX_NT35510

#endif // _GFX_NT35510_H_
