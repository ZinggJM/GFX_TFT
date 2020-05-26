// created by Jean-Marc Zingg to be the GFX_ILI9486 class for the ZxTFT library (instead of the GxCTRL_ILI9486 class for the GxTFT library)
// code extracts taken from https://github.com/Bodmer/TFT_HX8357
// spi kludge handling solution found in https://github.com/Bodmer/TFT_eSPI
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//

#ifndef _GFX_ILI9486_H_
#define _GFX_ILI9486_H_

//#include <GFX_Extensions.h>
#include <GFX_IO.h>
#include "GFX_Canvas16T.h"
// comment out to use only Adafruit_SPITFT
#include "GXF_TFT_IO.h"

#ifdef _GXF_TFT_IO_H_
#define SPI_GFX_Class GXF_TFT_IO
#else
#define SPI_GFX_Class GFX_IO
#endif

class GFX_ILI9486 : public SPI_GFX_Class
{
  public:
    GFX_ILI9486(int8_t cs_pin, int8_t dc_pin, int8_t rst_pin);
    GFX_ILI9486(int8_t cs_pin, int8_t dc_pin, int8_t mosi_pin, int8_t sclk_pin, int8_t rst_pin = -1);
    GFX_ILI9486(uint16_t width, uint16_t height, SPIClass *spi, int8_t cs_pin, int8_t dc_pin, int8_t rst_pin = -1);
    GFX_ILI9486(uint16_t width, uint16_t height, int8_t cs_pin, int8_t dc_pin, int8_t mosi_pin, int8_t sclk_pin, int8_t rst_pin = -1);
    void setSpiKludge(bool rpi_spi16_mode = true); // call with false before init or begin to disable
    void begin(uint32_t freq);
    void init(uint32_t freq = 0);
    void setRotation(uint8_t r);
    void invertDisplay(boolean i);
    void invert(boolean i);
    void enableDisplay(boolean enable);
    void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
  private:
    void _writeCommand16(uint16_t cmd);
  private:
    bool _spi16_mode;
    int8_t _bgr;
};

#endif // _GFX_ILI9486_H_
