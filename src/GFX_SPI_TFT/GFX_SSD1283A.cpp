// created by Jean-Marc Zingg to be a standalone GFX_SSD1283A library (instead of the GxCTRL_SSD1283A class for the GxTFT library)
// code extracts taken from https://github.com/lcdwiki/LCDWIKI_SPI
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//

#include "GFX_SSD1283A.h"

GFX_SSD1283A::GFX_SSD1283A(int8_t cs_pin, int8_t dc_pin, int8_t rst_pin, int8_t bl_pin) :
#if defined(ESP8266)
  SPI_GFX_Class(130, 130, cs_pin, dc_pin, rst_pin)
#else
  SPI_GFX_Class(130, 130, &SPI, cs_pin, dc_pin, rst_pin)
#endif
{
  _bl_pin = bl_pin;
  _inversion_bit = 0;
}

GFX_SSD1283A::GFX_SSD1283A(int8_t cs_pin, int8_t dc_pin, int8_t mosi_pin, int8_t sclk_pin, int8_t rst_pin, int8_t bl_pin) :
  SPI_GFX_Class(130, 130, cs_pin, dc_pin, mosi_pin, sclk_pin, rst_pin, -1)
{
  _bl_pin = bl_pin;
  _inversion_bit = 0;
}

GFX_SSD1283A::GFX_SSD1283A(uint16_t width, uint16_t height,
                                   SPIClass *spi, int8_t cs_pin, int8_t dc_pin, int8_t rst_pin, int8_t bl_pin) :
#if defined(ESP8266)
  SPI_GFX_Class(width, height, cs_pin, dc_pin, rst_pin)
#else
  SPI_GFX_Class(width, height, spi, cs_pin, dc_pin, rst_pin)
#endif
{
  (void) spi;
  _bl_pin = bl_pin;
  _inversion_bit = 0;
}

GFX_SSD1283A::GFX_SSD1283A(uint16_t width, uint16_t height,
                                   int8_t cs_pin, int8_t dc_pin, int8_t mosi_pin, int8_t sclk_pin, int8_t rst_pin, int8_t bl_pin) :
  SPI_GFX_Class(width, height, cs_pin, dc_pin, mosi_pin, sclk_pin, rst_pin, -1)
{
  _bl_pin = bl_pin;
  _inversion_bit = 0;
}

void GFX_SSD1283A::begin(uint32_t freq)
{
  init(freq);
}

void GFX_SSD1283A::init(uint32_t freq)
{
  static const uint16_t TFTLCD_DELAY16 = 0xFFFF;
  static const uint16_t SSD1283A_regValues[] PROGMEM =
  {
    0x10, 0x2F8E,
    0x11, 0x000C,
    0x07, 0x0021,
    0x28, 0x0006,
    0x28, 0x0005,
    0x27, 0x057F,
    0x29, 0x89A1,
    0x00, 0x0001,
    TFTLCD_DELAY16, 100,
    0x29, 0x80B0,
    TFTLCD_DELAY16, 30,
    0x29, 0xFFFE,
    0x07, 0x0223,
    TFTLCD_DELAY16, 30,
    0x07, 0x0233,
    0x01, 0x2183,
    0x03, 0x6830,
    0x2F, 0xFFFF,
    0x2C, 0x8000,
    0x27, 0x0570,
    0x02, 0x0300,
    0x0B, 0x580C,
    0x12, 0x0609,
    0x13, 0x3100,
  };
  if (!freq) freq = 27000000;
  initSPI(freq);
  _init_table16(SSD1283A_regValues, sizeof(SSD1283A_regValues));
  setRotation(0);
  if (_bl_pin >= 0) pinMode(_bl_pin, OUTPUT);
  enableDisplay(true);
}

void GFX_SSD1283A::setRotation(uint8_t r)
{
  rotation = r & 3;
  _width = (rotation & 1) ? HEIGHT : WIDTH;
  _height = (rotation & 1) ? WIDTH : HEIGHT;
  startWrite();
  switch (rotation)
  {
    // reg(0x01) bit RL 0x0100 doesn't work
    case 0:
      _writeCommandData16(0x01, _inversion_bit | 0x2183);
      _writeCommandData16(0x03, 0x6830);
      break;
    case 1:
      _writeCommandData16(0x01, _inversion_bit | 0x2283);
      _writeCommandData16(0x03, 0x6808);
      break;
    case 2:
      _writeCommandData16(0x01, _inversion_bit | 0x2183);
      _writeCommandData16(0x03, 0x6800);
      break;
    case 3:
      _writeCommandData16(0x01, _inversion_bit | 0x2283);
      _writeCommandData16(0x03, 0x6838);
      break;
  }
  endWrite();
}

void GFX_SSD1283A::invertDisplay(boolean i)
{
  _inversion_bit = i ? 0x0800 : 0x0000;
  setRotation(rotation);
}


void GFX_SSD1283A::invert(boolean i)
{
  invertDisplay(i);
}

void GFX_SSD1283A::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t x2 = x + w - 1;
  uint16_t y2 = y + h - 1;
  switch (rotation)
  {
    case 0:
      writeCommand(0x44);
      spiWrite(x2 + 2);
      spiWrite(x + 2);
      writeCommand(0x45);
      spiWrite(y2 + 2);
      spiWrite(y + 2);
      writeCommand(0x21);
      spiWrite(y + 2);
      spiWrite(x + 2);
      break;
    case 1:
      writeCommand(0x44);
      spiWrite(HEIGHT - y + 1);
      spiWrite(HEIGHT - y2 + 1);
      writeCommand(0x45);
      spiWrite(WIDTH - x - 1);
      spiWrite(WIDTH - x2 - 1);
      writeCommand(0x21);
      spiWrite(WIDTH - x - 1);
      spiWrite(HEIGHT - y + 1);
      break;
    case 2:
      writeCommand(0x44);
      spiWrite(WIDTH - x + 1);
      spiWrite(WIDTH - x2 + 1);
      writeCommand(0x45);
      spiWrite(HEIGHT - y + 1);
      spiWrite(HEIGHT - y2 + 1);
      writeCommand(0x21);
      spiWrite(HEIGHT - y + 1);
      spiWrite(WIDTH - x + 1);
      break;
    case 3:
      writeCommand(0x44);
      spiWrite(y2 + 2);
      spiWrite(y + 2);
      writeCommand(0x45);
      spiWrite(x2);
      spiWrite(x);
      writeCommand(0x21);
      spiWrite(x);
      spiWrite(y + 2);
      break;
  }
  writeCommand(0x22);
}

void GFX_SSD1283A::enableDisplay(boolean enable)
{
  if (_bl_pin >= 0) digitalWrite(_bl_pin, enable ? HIGH : LOW);
}

void GFX_SSD1283A::_writeCommandData16(uint8_t cmd, uint16_t data)
{
  writeCommand(cmd);
  SPI_WRITE16(data);
}

void GFX_SSD1283A::_init_table16(const void *table, int16_t size)
{
  static const uint16_t TFTLCD_DELAY16 = 0xFFFF;
  uint16_t *p = (uint16_t *) table;
  while (size > 0)
  {
    uint16_t cmd = pgm_read_word(p++);
    uint16_t d = pgm_read_word(p++);
    if (cmd == TFTLCD_DELAY16)
    {
      delay(d);
    }
    else
    {
      startWrite();
      writeCommand(cmd);
      SPI_WRITE16(d);
      endWrite();
    }
    size -= 2 * sizeof(int16_t);
  }
}
