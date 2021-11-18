// created by Jean-Marc Zingg to be the GFX_ILI9806 class for the GFX_TFT library
// code extracts taken from code and documentation from Ruijia Industry (Tiky_LCD.h, Tiky_LCD.c)
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// note: readRect does not work correctly with my only ILI9806 display (pixel sequence)
//       workaround added

#ifndef _GFX_ILI9806_H_
#define _GFX_ILI9806_H_

//#include <GFX_Extensions.h>
#include <GFX_IO.h>
#include "GFX_TFT_IO/GFX_TFT_IO.h"

class GFX_ILI9806 : public GFX_TFT_IO
{
  public:
    GFX_ILI9806(int16_t cs_pin, int16_t dc_pin, int16_t rst_pin = -1, int16_t bl_pin = -1);
    GFX_ILI9806(int16_t cs_pin, int16_t dc_pin, int16_t mosi_pin, int16_t sclk_pin, int16_t rst_pin, int16_t bl_pin = -1);
    GFX_ILI9806(SPIClass *spi, int16_t cs_pin, int16_t dc_pin, int16_t rst_pin = -1, int16_t bl_pin = -1);
    GFX_ILI9806(GFX_TFT_IO_Plugin& plugin);
    void begin(uint32_t freq);
    void init(uint32_t freq = 0);
    void setRotation(uint8_t r);
    void invertDisplay(bool i);
    void invert(bool i);
    void enableDisplay(bool enable);
    void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  private:
    void _writeCommandTransaction(uint8_t cmd);
    void _writeDataTransaction(uint8_t data);
  private:
    int16_t _bl_pin;
    int8_t _bgr;
};

#define GFX_TFT_CLASS GFX_ILI9806

#endif
