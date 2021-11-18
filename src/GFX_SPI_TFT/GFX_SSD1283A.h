// created by Jean-Marc Zingg to be the GFX_SSD1283A class for the GFX_TFT library
// code extracts taken from https://github.com/lcdwiki/LCDWIKI_SPI
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//

#ifndef _GFX_SSD1283A_H_
#define _GFX_SSD1283A_H_

//#include <GFX_Extensions.h>
#include <GFX_IO.h>
#include "GFX_Canvas16T.h"
// comment out to use only GFX_IO
//#include "GFX_TFT_IO/GFX_TFT_IO.h"

#ifdef _GFX_TFT_IO_H_
#define SPI_GFX_Class GFX_TFT_IO
#else
#define SPI_GFX_Class GFX_IO
#endif

class GFX_SSD1283A : public SPI_GFX_Class
{
  public:
    GFX_SSD1283A(int16_t cs_pin, int16_t dc_pin, int16_t rst_pin = -1, int16_t bl_pin = -1);
    GFX_SSD1283A(int16_t cs_pin, int16_t dc_pin, int16_t mosi_pin, int16_t sclk_pin, int16_t rst_pin, int16_t bl_pin = -1);
    GFX_SSD1283A(uint16_t width, uint16_t height, SPIClass *spi, int16_t cs_pin, int16_t dc_pin, int16_t rst_pin = -1, int16_t bl_pin = -1);
    GFX_SSD1283A(uint16_t width, uint16_t height, int16_t cs_pin, int16_t dc_pin, int16_t mosi_pin, int16_t sclk_pin, int16_t rst_pin = -1, int16_t bl_pin = -1);
    void begin(uint32_t freq);
    void init(uint32_t freq = 0);
    void setRotation(uint8_t r);
    void invertDisplay(bool i);
    void invert(bool i);
    void enableDisplay(bool enable);
    void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  private:
    void _writeCommandData16(uint8_t cmd, uint16_t data);
    void _init_table16(const void *table, int16_t size);
  private:
    int16_t _bl_pin;
    uint16_t _inversion_bit;
};

#endif // _GFX_SSD1283A_H_
