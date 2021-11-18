// created by Jean-Marc Zingg to be the GFX_TFT_IO class for the ZxTFT library
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//

#ifndef _GFX_TFT_IO_H_
#define _GFX_TFT_IO_H_

//#include <GFX_Extensions.h>
#include <GFX_IO.h>
#include "GFX_TFT_IO_Plugin.h"

class GFX_TFT_IO : public GFX_IO
{
  public:
    GFX_TFT_IO(uint16_t w, uint16_t h, int16_t cs, int16_t dc, int16_t mosi, int16_t sck, int16_t rst = -1, int16_t miso = -1);
    GFX_TFT_IO(uint16_t w, uint16_t h, int16_t cs, int16_t dc, int16_t rst = -1);
#if !defined(ESP8266)
    GFX_TFT_IO(uint16_t w, uint16_t h, SPIClass *spiClass, int16_t cs, int16_t dc, int16_t rst = -1);
#endif // end !ESP8266
    GFX_TFT_IO(uint16_t w, uint16_t h, tftBusWidth busWidth, int16_t d0, int16_t wr, int16_t dc, int16_t cs = -1, int16_t rst = -1, int16_t rd = -1);
    GFX_TFT_IO(uint16_t w, uint16_t h, GFX_TFT_IO_Plugin* plugin);
    // (overridden) virtual methods, use writeData16() for use with GFX_TFT_IO_Plugin!
    virtual void startWrite();
    virtual void endWrite();
    virtual void writePixel(int16_t x, int16_t y, uint16_t color);
    virtual void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    virtual void drawPixel(int16_t x, int16_t y, uint16_t color);
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
    virtual void fillScreen(uint16_t color);
    // Name is outdated (interface may be parallel) but for compatibility:
    void initSPI(uint32_t freq = 0, uint8_t spiMode = SPI_MODE0) 
    {
      init(freq, spiMode);
    };
    // the preferred name, can still be used for SPI
    void init(uint32_t freq = 0, uint8_t spiMode = SPI_MODE0);
  protected:
    void writeCommand(uint8_t cmd);
    void writeCommand16(uint16_t cmd);
    void writeData(uint8_t data);
    void writeData(uint8_t* data, uint32_t n);
    void writeData16(uint16_t data, uint32_t n = 1);
    void writeData16(const uint16_t* data, uint32_t n);
    void enableBacklight(bool enable = true);
  private:
    GFX_TFT_IO_Plugin* _plugin;
};

#endif
