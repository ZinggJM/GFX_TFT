// created by Jean-Marc Zingg to be the GFX_ILI9486 class for the GFX_TFT library
// code extracts taken from https://github.com/Bodmer/TFT_HX8357
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//

#include "GFX_ILI9486.h"

#define SPI_SPEED 4000000

#define CASET 0x2A
#define PASET 0x2B
#define RAMWR 0x2C
#define RAMRD   0x2E
#define MADCTL 0x36
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

#define RGB_or_BGR MADCTL_BGR // BGR for my screen

GFX_ILI9486::GFX_ILI9486(int16_t cs_pin, int16_t dc_pin, int16_t rst_pin, int16_t bl_pin) :
#if defined(ESP8266)
  GFX_TFT_IO(320, 480, cs_pin, dc_pin, rst_pin)
#else
  GFX_TFT_IO(320, 480, &SPI, cs_pin, dc_pin, rst_pin)
#endif
{
  _bl_pin = bl_pin;
  _bgr = RGB_or_BGR;
}

GFX_ILI9486::GFX_ILI9486(int16_t cs_pin, int16_t dc_pin, int16_t mosi_pin, int16_t sclk_pin, int16_t rst_pin, int16_t bl_pin) :
  GFX_TFT_IO(320, 480, cs_pin, dc_pin, mosi_pin, sclk_pin, rst_pin, -1)
{
  _bl_pin = bl_pin;
  _bgr = RGB_or_BGR;
}

GFX_ILI9486::GFX_ILI9486(SPIClass *spi, int16_t cs_pin, int16_t dc_pin, int16_t rst_pin, int16_t bl_pin) :
#if defined(ESP8266)
  GFX_TFT_IO(320, 480, cs_pin, dc_pin, rst_pin)
#else
  GFX_TFT_IO(320, 480, spi, cs_pin, dc_pin, rst_pin)
#endif
{
  (void) spi;
  _bl_pin = bl_pin;
  _bgr = RGB_or_BGR;
}

GFX_ILI9486::GFX_ILI9486(GFX_TFT_IO_Plugin& plugin) : GFX_TFT_IO(320, 480, &plugin)
{
  _bl_pin = -1;
  _bgr = RGB_or_BGR;
}

void GFX_ILI9486::begin(uint32_t freq)
{
  init(freq);
}

void GFX_ILI9486::init(uint32_t freq)
{
  Serial.println("init");
  if (!freq) freq = SPI_SPEED;
  initSPI(freq);
  startWrite();
  writeCommand(0x3A);
  writeData(0x55);  // use 16 bits per pixel color
  writeCommand(0x36);
  writeData(0x48);  // MX, BGR == rotation 0
  // PGAMCTRL(Positive Gamma Control)
  writeCommand(0xE0);
  writeData(0x0F);
  writeData(0x1F);
  writeData(0x1C);
  writeData(0x0C);
  writeData(0x0F);
  writeData(0x08);
  writeData(0x48);
  writeData(0x98);
  writeData(0x37);
  writeData(0x0A);
  writeData(0x13);
  writeData(0x04);
  writeData(0x11);
  writeData(0x0D);
  writeData(0x00);
  // NGAMCTRL(Negative Gamma Control)
  writeCommand(0xE1);
  writeData(0x0F);
  writeData(0x32);
  writeData(0x2E);
  writeData(0x0B);
  writeData(0x0D);
  writeData(0x05);
  writeData(0x47);
  writeData(0x75);
  writeData(0x37);
  writeData(0x06);
  writeData(0x10);
  writeData(0x03);
  writeData(0x24);
  writeData(0x20);
  writeData(0x00);
  // Digital Gamma Control 1
  writeCommand(0xE2);
  writeData(0x0F);
  writeData(0x32);
  writeData(0x2E);
  writeData(0x0B);
  writeData(0x0D);
  writeData(0x05);
  writeData(0x47);
  writeData(0x75);
  writeData(0x37);
  writeData(0x06);
  writeData(0x10);
  writeData(0x03);
  writeData(0x24);
  writeData(0x20);
  writeData(0x00);
  writeCommand(0x11);  // Sleep OUT
  delay(150);   // wait some time
  writeCommand(0x29);  // Display ON
  endWrite();
  enableDisplay(true);
}

void GFX_ILI9486::setRotation(uint8_t r)
{
  GFX_Root::setRotation(r);
  startWrite();
  writeCommand(MADCTL);
  switch (r & 3)
  {
    case 0:
      writeData(MADCTL_MX | _bgr);
      break;
    case 1:
      writeData(MADCTL_MV | _bgr);
      break;
    case 2:
      writeData(MADCTL_MY | _bgr);
      break;
    case 3:
      writeData(MADCTL_MX | MADCTL_MY | MADCTL_MV | _bgr);
      break;
  }
  endWrite();
}

void GFX_ILI9486::invertDisplay(bool i)
{
  _bgr = i ? MADCTL_BGR : 0x00;
  setRotation(rotation);
}


void GFX_ILI9486::invert(bool i)
{
  invertDisplay(i);
}

void GFX_ILI9486::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t xe = x + w - 1;
  uint16_t ye = y + h - 1;
  writeCommand(CASET);
  writeData(x >> 8);
  writeData(x & 0xFF);
  writeData(xe >> 8);
  writeData(xe & 0xFF);
  writeCommand(PASET);
  writeData(y >> 8);
  writeData(y & 0xFF);
  writeData(ye >> 8);
  writeData(ye & 0xFF);
  writeCommand(RAMWR);
}

void GFX_ILI9486::enableDisplay(bool enable)
{
  startWrite();
  writeCommand(enable ? 0x29 : 0x28);  // Display ON / Display OFF
  endWrite();
  if (_bl_pin >= 0) digitalWrite(_bl_pin, enable ? HIGH : LOW);
  enableBacklight(enable); // for _plugin
}
