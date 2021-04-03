// created by Jean-Marc Zingg to be the GFX_SPI_ILI9488 class for the GFX_TFT library
// code extracts taken from https://github.com/Bodmer/TFT_HX8357
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//

#ifndef _GFX_SPI_ILI9488_H_
#define _GFX_SPI_ILI9488_H_

//#include <GFX_Extensions.h>
#include <GFX_IO.h>
#include "GFX_TFT_IO/GFX_TFT_IO.h"
#include "GFX_Canvas16T.h"

class GFX_SPI_ILI9488 : public GFX_TFT_IO
{
  public:
    GFX_SPI_ILI9488(int8_t cs_pin, int8_t dc_pin, int8_t rst_pin = -1, int8_t bl_pin = -1);
    GFX_SPI_ILI9488(int8_t cs_pin, int8_t dc_pin, int8_t mosi_pin, int8_t sclk_pin, int8_t rst_pin, int8_t bl_pin = -1);
    GFX_SPI_ILI9488(uint16_t width, uint16_t height, SPIClass *spi, int8_t cs_pin, int8_t dc_pin, int8_t rst_pin = -1, int8_t bl_pin = -1);
    GFX_SPI_ILI9488(uint16_t width, uint16_t height, int8_t cs_pin, int8_t dc_pin, int8_t mosi_pin, int8_t sclk_pin, int8_t rst_pin = -1, int8_t bl_pin = -1);
    void begin(uint32_t freq);
    void init(uint32_t freq = 0);
    void setRotation(uint8_t r);
    void invertDisplay(bool i);
    void invert(bool i);
    void enableDisplay(bool enable);
    void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    void writePixel(int16_t x, int16_t y, uint16_t color);
    void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    //void writePixels(uint16_t *colors, uint32_t len, bool block = true, bool bigEndian = false); // TO BE ADDED
    void writeColor(uint16_t color, uint32_t len);
    void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  private:
    void _writeCommand16(uint16_t cmd);
    // note: only use for pixel data, RGB888 on ILI9488
    void _writeColor16(uint16_t data, uint32_t n);
    void _writeColor16(const uint16_t* data, uint32_t n);
  private:
    int8_t _bl_pin;
    int8_t _bgr;
};

#endif // _GFX_SPI_ILI9488_H_
