// created by Jean-Marc Zingg to be the GFX_OTM8009A class for the GFX_TFT library
// code extracts taken from code and documentation from Ruijia Industry (lcd.h, lcd.c)
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this class works with "IPS 3.97 inch 16.7M HD TFT LCD Touch Screen Module OTM8009A Drive IC 480(RGB)*800" display from Ruijia Industry
// e.g. https://www.aliexpress.com/item/IPS-3-97-inch-HD-TFT-LCD-Touch-Screen-Module-OTM8009A-Drive-IC-800-480/32676929794.html
// this display matches the FSMC TFT connector of the STM32F407ZG-M4 board, EXCEPT FOR POWER SUPPLY PINS
// e.g. https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
// CAUTION: the display needs 5V on VCC pins; data and control pins are 3.3V
//
// note: this display needs 16bit commands, aka "(MDDI/SPI) Address" in some OTM8009A.pdf

#ifndef _GFX_OTM8009A_H_
#define _GFX_OTM8009A_H_

//#include <GFX_Extensions.h>
#include <GFX_IO.h>
#include "GFX_TFT_IO/GFX_TFT_IO.h"

class GFX_OTM8009A : public GFX_TFT_IO
{
  public:
    GFX_OTM8009A(int16_t cs_pin, int16_t dc_pin, int16_t rst_pin = -1, int16_t bl_pin = -1);
    GFX_OTM8009A(int16_t cs_pin, int16_t dc_pin, int16_t mosi_pin, int16_t sclk_pin, int16_t rst_pin, int16_t bl_pin = -1);
    GFX_OTM8009A(SPIClass *spi, int16_t cs_pin, int16_t dc_pin, int16_t rst_pin = -1, int16_t bl_pin = -1);
    GFX_OTM8009A(GFX_TFT_IO_Plugin& plugin);
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

#define GFX_TFT_CLASS GFX_OTM8009A

#endif // _GFX_OTM8009A_H_
