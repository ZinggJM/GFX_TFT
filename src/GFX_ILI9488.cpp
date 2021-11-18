// created by Jean-Marc Zingg to be the GFX_ILI9488 class for the GFX_TFT library
// init register values taken from MCUFRIEND_kbv (David Prentice)
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//

#include "GFX_ILI9488.h"

#define SPI_SPEED 4000000

#define CASET 0x2A
#define PASET 0x2B
#define RAMWR 0x2C
#define RAMRD 0x2E
#define MADCTL 0x36
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

#define RGB_or_BGR MADCTL_BGR // BGR for my screen

GFX_ILI9488::GFX_ILI9488(int16_t cs_pin, int16_t dc_pin, int16_t rst_pin, int16_t bl_pin) :
#if defined(ESP8266)
  GFX_TFT_IO(320, 480, cs_pin, dc_pin, rst_pin)
#else
  GFX_TFT_IO(320, 480, &SPI, cs_pin, dc_pin, rst_pin)
#endif
{
  _bl_pin = bl_pin;
  _bgr = RGB_or_BGR;
}

GFX_ILI9488::GFX_ILI9488(int16_t cs_pin, int16_t dc_pin, int16_t mosi_pin, int16_t sclk_pin, int16_t rst_pin, int16_t bl_pin) :
  GFX_TFT_IO(320, 480, cs_pin, dc_pin, mosi_pin, sclk_pin, rst_pin, -1)
{
  _bl_pin = bl_pin;
  _bgr = RGB_or_BGR;
}

GFX_ILI9488::GFX_ILI9488(SPIClass *spi, int16_t cs_pin, int16_t dc_pin, int16_t rst_pin, int16_t bl_pin) :
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

GFX_ILI9488::GFX_ILI9488(GFX_TFT_IO_Plugin& plugin) : GFX_TFT_IO(320, 480, &plugin)
{
  _bl_pin = -1;
  _bgr = RGB_or_BGR;
}

void GFX_ILI9488::begin(uint32_t freq)
{
  init(freq);
}

void GFX_ILI9488::init(uint32_t freq)
{
  Serial.println("init");
  if (!freq) freq = SPI_SPEED;
  initSPI(freq);
  startWrite();
  writeCommand(0x01); // Soft Reset
  endWrite();
  delay(150);
  startWrite();
  writeCommand(0x28); // Display Off
  writeCommand(0x3A); // Pixel read=565, write=565.
  writeData(0x55);
  writeCommand(0xC0); // Power Control 1
  writeData(0x10);
  writeData(0x10);
  writeCommand(0xC1); // Power Control 2
  writeData(0x41);
  writeCommand(0xC5); // VCOM  Control 1
  writeData(0x00);
  writeData(0x22);
  writeData(0x80);
  writeData(0x40);
  writeCommand(0x36); // Memory Access
  writeData(0x68);
  writeCommand(0xB0); // Interface
  writeData(0x00);
  writeCommand(0xB1); // Frame Rate Control [B0 11]
  writeData(0xB0);
  writeData(0x11);
  writeCommand(0xB4); //Inversion Control [02]
  writeData(0x02);
  writeCommand(0xB6); // Display Function Control [02 02 3B] .kbv NL=480
  writeData(0x02);
  writeData(0x02);
  writeData(0x3B);
  writeCommand(0xB7); // Entry Mode
  writeData(0xC6);
  writeCommand(0x3A); // Interlace Pixel Format
  writeData(0x55);
  writeCommand(0xF7); // Adjustment Control 3 [A9 51 2C 82]
  writeData(0xA9);
  writeData(0x51);
  writeData(0x2C);
  writeData(0x82);
  writeCommand(0x11); // Sleep Out
  endWrite();
  delay(150);
  //  startWrite();
  //  writeCommand(0x29); // Display On
  //  endWrite();
  setRotation(0);
  enableDisplay(true);
}

void GFX_ILI9488::setRotation(uint8_t r)
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

void GFX_ILI9488::invertDisplay(bool i)
{
  _bgr = i ? MADCTL_BGR : 0x00;
  setRotation(rotation);
}


void GFX_ILI9488::invert(bool i)
{
  invertDisplay(i);
}

void GFX_ILI9488::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
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

void GFX_ILI9488::enableDisplay(bool enable)
{
  startWrite();
  writeCommand(enable ? 0x29 : 0x28);  // Display ON / Display OFF
  endWrite();
  if (_bl_pin >= 0) digitalWrite(_bl_pin, enable ? HIGH : LOW);
  enableBacklight(enable); // for _plugin
}
