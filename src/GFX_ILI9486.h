// created by Jean-Marc Zingg to be the GFX_ILI9486 class for the GFX_TFT library
// code extracts taken from https://github.com/Bodmer/TFT_HX8357
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//

#ifndef _GFX_ILI9486_H_
#define _GFX_ILI9486_H_

//#include <GFX_Extensions.h>
#include <GFX_IO.h>
#include "GFX_TFT_IO/GFX_TFT_IO.h"

class GFX_ILI9486 : public GFX_TFT_IO
{
  public:
    GFX_ILI9486(int8_t cs_pin, int8_t dc_pin, int8_t rst_pin = -1, int8_t bl_pin = -1);
    GFX_ILI9486(int8_t cs_pin, int8_t dc_pin, int8_t mosi_pin, int8_t sclk_pin, int8_t rst_pin, int8_t bl_pin = -1);
    GFX_ILI9486(SPIClass *spi, int8_t cs_pin, int8_t dc_pin, int8_t rst_pin = -1, int8_t bl_pin = -1);
    GFX_ILI9486(GFX_TFT_IO_Plugin& plugin);
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

#define GFX_TFT_CLASS GFX_ILI9486

#endif // _GFX_ILI9486_H_
