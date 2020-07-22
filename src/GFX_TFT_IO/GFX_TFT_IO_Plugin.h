// created by Jean-Marc Zingg to be the GFX_TFT_IO class for the ZxTFT library
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//

#ifndef _GFX_TFT_IO_Plugin_H_
#define _GFX_TFT_IO_Plugin_H_

#include <Arduino.h>

class GFX_TFT_IO_Plugin
{
  public:
    virtual void init() = 0;
    virtual void writeCommand(uint8_t cmd) = 0;
    virtual void writeCommand16(uint16_t cmd) = 0;
    virtual void writeData(uint8_t data) = 0;
    virtual void writeData(uint8_t* data, uint32_t n) = 0;
    virtual void writeData16(uint16_t data, uint32_t n) = 0;
    virtual void writeData16(const uint16_t* data, uint32_t n);
    virtual void enableBacklight(bool enable = true) = 0;
};

#endif
