// created by Jean-Marc Zingg to be the GFX_ST7796S class for the GFX_TFT library
// code extracts taken from http://www.lcdwiki.com/res/Program/Parallel_Port/4.0inch/8_16BIT_ST7796S_MRB3951_V1.0/4.0inch_8&16BIT_Module_ST7796S_MRB3951_V1.0.zip (lcd.h, lcd.c)
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this class works with "4.0 inch HD TFT LCD Screen with PCB Board ILI9488 ST7796S Drive IC 320(RGB)*480 8Bit Parallel Interface (Touch/No Touch)" display from Maithoga
// e.g. https://www.aliexpress.com/item/1899444540.html
// this display matches the FSMC TFT connector of the STM32F407ZG-M4 board
// e.g. https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
// this display also matches the FSMC TFT connector of the STM32F407VxT6 board e.g. https://www.aliexpress.com/item/1005001806399987.html

#include "GFX_ST7796S.h"

#define SPI_SPEED 40000000

#define MADCTL     0x36
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08

#define RGB_or_BGR MADCTL_BGR // BGR for my screen

GFX_ST7796S::GFX_ST7796S(int8_t cs_pin, int8_t dc_pin, int8_t rst_pin, int8_t bl_pin) :
#if defined(ESP8266)
  GFX_TFT_IO(320, 480, cs_pin, dc_pin, rst_pin)
#else
  GFX_TFT_IO(320, 480, &SPI, cs_pin, dc_pin, rst_pin)
#endif
{
  _bl_pin = bl_pin;
  _bgr = RGB_or_BGR;
}

GFX_ST7796S::GFX_ST7796S(int8_t cs_pin, int8_t dc_pin, int8_t mosi_pin, int8_t sclk_pin, int8_t rst_pin, int8_t bl_pin) :
  GFX_TFT_IO(320, 480, cs_pin, dc_pin, mosi_pin, sclk_pin, rst_pin, -1)
{
  _bl_pin = bl_pin;
  _bgr = RGB_or_BGR;
}

GFX_ST7796S::GFX_ST7796S(SPIClass *spi, int8_t cs_pin, int8_t dc_pin, int8_t rst_pin, int8_t bl_pin) :
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

GFX_ST7796S::GFX_ST7796S(GFX_TFT_IO_Plugin& plugin) : GFX_TFT_IO(320, 480, &plugin)
{
  _bl_pin = -1;
  _bgr = RGB_or_BGR;
}

void GFX_ST7796S::begin(uint32_t freq)
{
  init(freq);
}

void GFX_ST7796S::init(uint32_t freq)
{
  Serial.println("init");
  if (!freq) freq = SPI_SPEED;
  initSPI(freq);
  startWrite();
  //************* ST7796S��ʼ��**********//
  writeCommand(0xF0);
  writeData(0xC3);
  writeCommand(0xF0);
  writeData(0x96);
  writeCommand(0x36);
  writeData(0x68);
  writeCommand(0x3A);
  writeData(0x05);
  writeCommand(0xB0);
  writeData(0x80);
  writeCommand(0xB6);
  writeData(0x20);
  writeData(0x02);
  writeCommand(0xB5);
  writeData(0x02);
  writeData(0x03);
  writeData(0x00);
  writeData(0x04);
  writeCommand(0xB1);
  writeData(0x80);
  writeData(0x10);
  writeCommand(0xB4);
  writeData(0x00);
  writeCommand(0xB7);
  writeData(0xC6);
  writeCommand(0xC5);
  writeData(0x24);
  writeCommand(0xE4);
  writeData(0x31);
  writeCommand(0xE8);
  writeData(0x40);
  writeData(0x8A);
  writeData(0x00);
  writeData(0x00);
  writeData(0x29);
  writeData(0x19);
  writeData(0xA5);
  writeData(0x33);
  writeCommand(0xC2);
  writeCommand(0xA7);

  writeCommand(0xE0);
  writeData(0xF0);
  writeData(0x09);
  writeData(0x13);
  writeData(0x12);
  writeData(0x12);
  writeData(0x2B);
  writeData(0x3C);
  writeData(0x44);
  writeData(0x4B);
  writeData(0x1B);
  writeData(0x18);
  writeData(0x17);
  writeData(0x1D);
  writeData(0x21);

  writeCommand(0XE1);
  writeData(0xF0);
  writeData(0x09);
  writeData(0x13);
  writeData(0x0C);
  writeData(0x0D);
  writeData(0x27);
  writeData(0x3B);
  writeData(0x44);
  writeData(0x4D);
  writeData(0x0B);
  writeData(0x17);
  writeData(0x17);
  writeData(0x1D);
  writeData(0x21);

  writeCommand(0X36);
  writeData(0xEC);
  writeCommand(0xF0);
  writeData(0xC3);
  writeCommand(0xF0);
  writeData(0x69);
  writeCommand(0X13);
  writeCommand(0X11);
  writeCommand(0X29);
  endWrite();
  setRotation(0);
  if (_bl_pin >= 0) pinMode(_bl_pin, OUTPUT);
  enableDisplay(true);
}

void GFX_ST7796S::setRotation(uint8_t r)
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
      writeData(MADCTL_MY | MADCTL_ML | _bgr);
      break;
    case 3:
      writeData(MADCTL_MY | MADCTL_MX | MADCTL_MV | MADCTL_ML | _bgr);
      break;
  }
  endWrite();
}

void GFX_ST7796S::invertDisplay(bool i)
{
  _bgr = i ? 0x00 : MADCTL_BGR;
  setRotation(rotation);
}


void GFX_ST7796S::invert(bool i)
{
  invertDisplay(i);
}

void GFX_ST7796S::enableDisplay(bool enable)
{
  startWrite();
  writeCommand(enable ? 0x29 : 0x28);  // Display ON / Display OFF
  endWrite();
  if (_bl_pin >= 0) digitalWrite(_bl_pin, enable ? HIGH : LOW);
  enableBacklight(enable); // for _plugin
}

void GFX_ST7796S::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t xe = x + w - 1;
  uint16_t ye = y + h - 1;
  writeCommand(0x2A);
  writeData(x >> 8);
  writeData(x);
  writeData(xe >> 8);
  writeData(xe);
  writeCommand(0x2B);
  writeData(y >> 8);
  writeData(y);
  writeData(ye >> 8);
  writeData(ye);
  writeCommand(0x2C);
}
