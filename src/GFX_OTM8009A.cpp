// created by Jean-Marc Zingg to be the GFX_OTM8009A class for the GFX_TFT library
// code extracts taken from code and documentation from Ruijia Industry (lcd.h, lcd.c)
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this class works with "IPS 3.97 inch 16.7M HD TFT LCD Touch Screen Module OTM8009A Drive IC 480(RGB)*800" display from Ruijia Industry
// e.g. https://www.aliexpress.com/item/IPS-3-97-inch-HD-TFT-LCD-Touch-Screen-Module-OTM8009A-Drive-IC-800-480/32676929794.html
// this display matches the FSMC TFT connector of the STM32F407ZG-M4 board, EXCEPT FOR POWER SUPPLY PINS
// e.g. https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
// CAUTION: the display needs 5V on VCC pins; data and control pins are 3.3V
//
// note: this display needs 16bit commands, aka "(MDDI/SPI) Address" in some OTM8009A.pdf

#include "GFX_OTM8009A.h"

#define SPI_SPEED 40000000

#define OTM8009A_MADCTL     0x3600
#define OTM8009A_MADCTL_MY  0x80
#define OTM8009A_MADCTL_MX  0x40
#define OTM8009A_MADCTL_MV  0x20
#define OTM8009A_MADCTL_ML  0x10
#define OTM8009A_MADCTL_RGB 0x00
#define OTM8009A_MADCTL_BGR 0x08

GFX_OTM8009A::GFX_OTM8009A(int8_t cs_pin, int8_t dc_pin, int8_t rst_pin, int8_t bl_pin) :
#if defined(ESP8266)
  GFX_TFT_IO(480, 800, cs_pin, dc_pin, rst_pin)
#else
  GFX_TFT_IO(480, 800, &SPI, cs_pin, dc_pin, rst_pin)
#endif
{
  _bl_pin = bl_pin;
  _bgr = 0;
}

GFX_OTM8009A::GFX_OTM8009A(int8_t cs_pin, int8_t dc_pin, int8_t mosi_pin, int8_t sclk_pin, int8_t rst_pin, int8_t bl_pin) :
  GFX_TFT_IO(480, 800, cs_pin, dc_pin, mosi_pin, sclk_pin, rst_pin, -1)
{
  _bl_pin = bl_pin;
  _bgr = 0;
}

GFX_OTM8009A::GFX_OTM8009A(SPIClass *spi, int8_t cs_pin, int8_t dc_pin, int8_t rst_pin, int8_t bl_pin) :
#if defined(ESP8266)
  GFX_TFT_IO(480, 800, cs_pin, dc_pin, rst_pin)
#else
  GFX_TFT_IO(480, 800, spi, cs_pin, dc_pin, rst_pin)
#endif
{
  (void) spi;
  _bl_pin = bl_pin;
  _bgr = 0;
}

GFX_OTM8009A::GFX_OTM8009A(GFX_TFT_IO_Plugin& plugin) : GFX_TFT_IO(480, 800, &plugin)
{
  _bl_pin = -1;
  _bgr = 0;
}

void GFX_OTM8009A::begin(uint32_t freq)
{
  init(freq);
}

void GFX_OTM8009A::init(uint32_t freq)
{
  Serial.println("init");
  if (!freq) freq = SPI_SPEED;
  initSPI(freq);
  startWrite();
  //============ OTM8009A+HSD3.97 20140613 ===============================================//
  writeCommand16(0xff00);      writeData16(0x80);    //enable access command2
  writeCommand16(0xff01);      writeData16(0x09);    //enable access command2
  writeCommand16(0xff02);      writeData16(0x01);    //enable access command2
  writeCommand16(0xff80);      writeData16(0x80);    //enable access command2
  writeCommand16(0xff81);      writeData16(0x09);    //enable access command2
  writeCommand16(0xff03);      writeData16(0x01);    //
  writeCommand16(0xc5b1);      writeData16(0xA9);    //power control

  writeCommand16(0xc591);      writeData16(0x0F);               //power control
  writeCommand16(0xc0B4);      writeData16(0x50);

  //panel driving mode : column inversion

  //////  gamma
  writeCommand16(0xE100);      writeData16(0x00);
  writeCommand16(0xE101);      writeData16(0x09);
  writeCommand16(0xE102);      writeData16(0x0F);
  writeCommand16(0xE103);      writeData16(0x0E);
  writeCommand16(0xE104);      writeData16(0x07);
  writeCommand16(0xE105);      writeData16(0x10);
  writeCommand16(0xE106);      writeData16(0x0B);
  writeCommand16(0xE107);      writeData16(0x0A);
  writeCommand16(0xE108);      writeData16(0x04);
  writeCommand16(0xE109);      writeData16(0x07);
  writeCommand16(0xE10A);      writeData16(0x0B);
  writeCommand16(0xE10B);      writeData16(0x08);
  writeCommand16(0xE10C);      writeData16(0x0F);
  writeCommand16(0xE10D);      writeData16(0x10);
  writeCommand16(0xE10E);      writeData16(0x0A);
  writeCommand16(0xE10F);      writeData16(0x01);
  writeCommand16(0xE200);      writeData16(0x00);
  writeCommand16(0xE201);      writeData16(0x09);
  writeCommand16(0xE202);      writeData16(0x0F);
  writeCommand16(0xE203);      writeData16(0x0E);
  writeCommand16(0xE204);      writeData16(0x07);
  writeCommand16(0xE205);      writeData16(0x10);
  writeCommand16(0xE206);      writeData16(0x0B);
  writeCommand16(0xE207);      writeData16(0x0A);
  writeCommand16(0xE208);      writeData16(0x04);
  writeCommand16(0xE209);      writeData16(0x07);
  writeCommand16(0xE20A);      writeData16(0x0B);
  writeCommand16(0xE20B);      writeData16(0x08);
  writeCommand16(0xE20C);      writeData16(0x0F);
  writeCommand16(0xE20D);      writeData16(0x10);
  writeCommand16(0xE20E);      writeData16(0x0A);
  writeCommand16(0xE20F);      writeData16(0x01);
  writeCommand16(0xD900);      writeData16(0x4E);    //vcom setting

  writeCommand16(0xc181);      writeData16(0x66);    //osc=65HZ

  writeCommand16(0xc1a1);      writeData16(0x08);
  writeCommand16(0xc592);      writeData16(0x01);    //power control

  writeCommand16(0xc595);      writeData16(0x34);    //power control

  writeCommand16(0xd800);      writeData16(0x79);    //GVDD / NGVDD setting

  writeCommand16(0xd801);      writeData16(0x79);    //GVDD / NGVDD setting

  writeCommand16(0xc594);      writeData16(0x33);    //power control

  writeCommand16(0xc0a3);      writeData16(0x1B);       //panel timing setting
  writeCommand16(0xc582);      writeData16(0x83);    //power control

  writeCommand16(0xc481);      writeData16(0x83);    //source driver setting

  writeCommand16(0xc1a1);      writeData16(0x0E);
  writeCommand16(0xb3a6);      writeData16(0x20);
  writeCommand16(0xb3a7);      writeData16(0x01);
  writeCommand16(0xce80);      writeData16(0x85);    // GOA VST

  writeCommand16(0xce81);      writeData16(0x01);  // GOA VST

  writeCommand16(0xce82);      writeData16(0x00);    // GOA VST

  writeCommand16(0xce83);      writeData16(0x84);    // GOA VST
  writeCommand16(0xce84);      writeData16(0x01);    // GOA VST
  writeCommand16(0xce85);      writeData16(0x00);    // GOA VST

  writeCommand16(0xcea0);      writeData16(0x18);    // GOA CLK
  writeCommand16(0xcea1);      writeData16(0x04);    // GOA CLK
  writeCommand16(0xcea2);      writeData16(0x03);    // GOA CLK
  writeCommand16(0xcea3);      writeData16(0x39);    // GOA CLK
  writeCommand16(0xcea4);      writeData16(0x00);    // GOA CLK
  writeCommand16(0xcea5);      writeData16(0x00);    // GOA CLK
  writeCommand16(0xcea6);      writeData16(0x00);    // GOA CLK
  writeCommand16(0xcea7);      writeData16(0x18);    // GOA CLK
  writeCommand16(0xcea8);      writeData16(0x03);    // GOA CLK
  writeCommand16(0xcea9);      writeData16(0x03);    // GOA CLK
  writeCommand16(0xceaa);      writeData16(0x3a);    // GOA CLK
  writeCommand16(0xceab);      writeData16(0x00);    // GOA CLK
  writeCommand16(0xceac);      writeData16(0x00);    // GOA CLK
  writeCommand16(0xcead);      writeData16(0x00);    // GOA CLK
  writeCommand16(0xceb0);      writeData16(0x18);    // GOA CLK
  writeCommand16(0xceb1);      writeData16(0x02);    // GOA CLK
  writeCommand16(0xceb2);      writeData16(0x03);    // GOA CLK
  writeCommand16(0xceb3);      writeData16(0x3b);    // GOA CLK
  writeCommand16(0xceb4);      writeData16(0x00);    // GOA CLK
  writeCommand16(0xceb5);      writeData16(0x00);    // GOA CLK
  writeCommand16(0xceb6);      writeData16(0x00);    // GOA CLK
  writeCommand16(0xceb7);      writeData16(0x18);    // GOA CLK
  writeCommand16(0xceb8);      writeData16(0x01);    // GOA CLK
  writeCommand16(0xceb9);      writeData16(0x03);    // GOA CLK
  writeCommand16(0xceba);      writeData16(0x3c);    // GOA CLK
  writeCommand16(0xcebb);      writeData16(0x00);    // GOA CLK
  writeCommand16(0xcebc);      writeData16(0x00);    // GOA CLK
  writeCommand16(0xcebd);      writeData16(0x00);    // GOA CLK
  writeCommand16(0xcfc0);      writeData16(0x01);    // GOA ECLK
  writeCommand16(0xcfc1);      writeData16(0x01);    // GOA ECLK
  writeCommand16(0xcfc2);      writeData16(0x20);    // GOA ECLK

  writeCommand16(0xcfc3);      writeData16(0x20);    // GOA ECLK

  writeCommand16(0xcfc4);      writeData16(0x00);    // GOA ECLK

  writeCommand16(0xcfc5);      writeData16(0x00);    // GOA ECLK

  writeCommand16(0xcfc6);      writeData16(0x01);    // GOA other options

  writeCommand16(0xcfc7);      writeData16(0x00);

  // GOA signal toggle option setting

  writeCommand16(0xcfc8);      writeData16(0x00);    //GOA signal toggle option setting
  writeCommand16(0xcfc9);      writeData16(0x00);

  //GOA signal toggle option setting

  writeCommand16(0xcfd0);      writeData16(0x00);
  writeCommand16(0xcb80);      writeData16(0x00);
  writeCommand16(0xcb81);      writeData16(0x00);
  writeCommand16(0xcb82);      writeData16(0x00);
  writeCommand16(0xcb83);      writeData16(0x00);
  writeCommand16(0xcb84);      writeData16(0x00);
  writeCommand16(0xcb85);      writeData16(0x00);
  writeCommand16(0xcb86);      writeData16(0x00);
  writeCommand16(0xcb87);      writeData16(0x00);
  writeCommand16(0xcb88);      writeData16(0x00);
  writeCommand16(0xcb89);      writeData16(0x00);
  writeCommand16(0xcb90);      writeData16(0x00);
  writeCommand16(0xcb91);      writeData16(0x00);
  writeCommand16(0xcb92);      writeData16(0x00);
  writeCommand16(0xcb93);      writeData16(0x00);
  writeCommand16(0xcb94);      writeData16(0x00);
  writeCommand16(0xcb95);      writeData16(0x00);
  writeCommand16(0xcb96);      writeData16(0x00);
  writeCommand16(0xcb97);      writeData16(0x00);
  writeCommand16(0xcb98);      writeData16(0x00);
  writeCommand16(0xcb99);      writeData16(0x00);
  writeCommand16(0xcb9a);      writeData16(0x00);
  writeCommand16(0xcb9b);      writeData16(0x00);
  writeCommand16(0xcb9c);      writeData16(0x00);
  writeCommand16(0xcb9d);      writeData16(0x00);
  writeCommand16(0xcb9e);      writeData16(0x00);
  writeCommand16(0xcba0);      writeData16(0x00);
  writeCommand16(0xcba1);      writeData16(0x00);
  writeCommand16(0xcba2);      writeData16(0x00);
  writeCommand16(0xcba3);      writeData16(0x00);
  writeCommand16(0xcba4);      writeData16(0x00);
  writeCommand16(0xcba5);      writeData16(0x00);
  writeCommand16(0xcba6);      writeData16(0x00);
  writeCommand16(0xcba7);      writeData16(0x00);
  writeCommand16(0xcba8);      writeData16(0x00);
  writeCommand16(0xcba9);      writeData16(0x00);
  writeCommand16(0xcbaa);      writeData16(0x00);
  writeCommand16(0xcbab);      writeData16(0x00);
  writeCommand16(0xcbac);      writeData16(0x00);
  writeCommand16(0xcbad);      writeData16(0x00);
  writeCommand16(0xcbae);      writeData16(0x00);
  writeCommand16(0xcbb0);      writeData16(0x00);
  writeCommand16(0xcbb1);      writeData16(0x00);
  writeCommand16(0xcbb2);      writeData16(0x00);
  writeCommand16(0xcbb3);      writeData16(0x00);
  writeCommand16(0xcbb4);      writeData16(0x00);
  writeCommand16(0xcbb5);      writeData16(0x00);
  writeCommand16(0xcbb6);      writeData16(0x00);
  writeCommand16(0xcbb7);      writeData16(0x00);
  writeCommand16(0xcbb8);      writeData16(0x00);
  writeCommand16(0xcbb9);      writeData16(0x00);
  writeCommand16(0xcbc0);      writeData16(0x00);
  writeCommand16(0xcbc1);      writeData16(0x04);
  writeCommand16(0xcbc2);      writeData16(0x04);
  writeCommand16(0xcbc3);      writeData16(0x04);
  writeCommand16(0xcbc4);      writeData16(0x04);
  writeCommand16(0xcbc5);      writeData16(0x04);
  writeCommand16(0xcbc6);      writeData16(0x00);
  writeCommand16(0xcbc7);      writeData16(0x00);
  writeCommand16(0xcbc8);      writeData16(0x00);
  writeCommand16(0xcbc9);      writeData16(0x00);
  writeCommand16(0xcbca);      writeData16(0x00);
  writeCommand16(0xcbcb);      writeData16(0x00);
  writeCommand16(0xcbcc);      writeData16(0x00);
  writeCommand16(0xcbcd);      writeData16(0x00);
  writeCommand16(0xcbce);      writeData16(0x00);
  writeCommand16(0xcbd0);      writeData16(0x00);
  writeCommand16(0xcbd1);      writeData16(0x00);
  writeCommand16(0xcbd2);      writeData16(0x00);
  writeCommand16(0xcbd3);      writeData16(0x00);
  writeCommand16(0xcbd4);      writeData16(0x00);
  writeCommand16(0xcbd5);      writeData16(0x00);
  writeCommand16(0xcbd6);      writeData16(0x04);
  writeCommand16(0xcbd7);      writeData16(0x04);
  writeCommand16(0xcbd8);      writeData16(0x04);
  writeCommand16(0xcbd9);      writeData16(0x04);
  writeCommand16(0xcbda);      writeData16(0x04);
  writeCommand16(0xcbdb);      writeData16(0x00);
  writeCommand16(0xcbdc);      writeData16(0x00);
  writeCommand16(0xcbdd);      writeData16(0x00);
  writeCommand16(0xcbde);      writeData16(0x00);
  writeCommand16(0xcbe0);      writeData16(0x00);
  writeCommand16(0xcbe1);      writeData16(0x00);
  writeCommand16(0xcbe2);      writeData16(0x00);
  writeCommand16(0xcbe3);      writeData16(0x00);
  writeCommand16(0xcbe4);      writeData16(0x00);
  writeCommand16(0xcbe5);      writeData16(0x00);
  writeCommand16(0xcbe6);      writeData16(0x00);
  writeCommand16(0xcbe7);      writeData16(0x00);
  writeCommand16(0xcbe8);      writeData16(0x00);
  writeCommand16(0xcbe9);      writeData16(0x00);
  writeCommand16(0xcbf0);      writeData16(0xFF);
  writeCommand16(0xcbf1);      writeData16(0xFF);
  writeCommand16(0xcbf2);      writeData16(0xFF);
  writeCommand16(0xcbf3);      writeData16(0xFF);
  writeCommand16(0xcbf4);      writeData16(0xFF);
  writeCommand16(0xcbf5);      writeData16(0xFF);
  writeCommand16(0xcbf6);      writeData16(0xFF);
  writeCommand16(0xcbf7);      writeData16(0xFF);
  writeCommand16(0xcbf8);      writeData16(0xFF);
  writeCommand16(0xcbf9);      writeData16(0xFF);
  writeCommand16(0xcc80);      writeData16(0x00);
  writeCommand16(0xcc81);      writeData16(0x26);
  writeCommand16(0xcc82);      writeData16(0x09);
  writeCommand16(0xcc83);      writeData16(0x0B);
  writeCommand16(0xcc84);      writeData16(0x01);
  writeCommand16(0xcc85);      writeData16(0x25);
  writeCommand16(0xcc86);      writeData16(0x00);
  writeCommand16(0xcc87);      writeData16(0x00);
  writeCommand16(0xcc88);      writeData16(0x00);
  writeCommand16(0xcc89);      writeData16(0x00);
  writeCommand16(0xcc90);      writeData16(0x00);
  writeCommand16(0xcc91);      writeData16(0x00);
  writeCommand16(0xcc92);      writeData16(0x00);
  writeCommand16(0xcc93);      writeData16(0x00);
  writeCommand16(0xcc94);      writeData16(0x00);
  writeCommand16(0xcc95);      writeData16(0x00);
  writeCommand16(0xcc96);      writeData16(0x00);
  writeCommand16(0xcc97);      writeData16(0x00);
  writeCommand16(0xcc98);      writeData16(0x00);
  writeCommand16(0xcc99);      writeData16(0x00);
  writeCommand16(0xcc9a);      writeData16(0x00);
  writeCommand16(0xcc9b);      writeData16(0x26);
  writeCommand16(0xcc9c);      writeData16(0x0A);
  writeCommand16(0xcc9d);      writeData16(0x0C);
  writeCommand16(0xcc9e);      writeData16(0x02);
  writeCommand16(0xcca0);      writeData16(0x25);
  writeCommand16(0xcca1);      writeData16(0x00);
  writeCommand16(0xcca2);      writeData16(0x00);
  writeCommand16(0xcca3);      writeData16(0x00);
  writeCommand16(0xcca4);      writeData16(0x00);
  writeCommand16(0xcca5);      writeData16(0x00);
  writeCommand16(0xcca6);      writeData16(0x00);
  writeCommand16(0xcca7);      writeData16(0x00);
  writeCommand16(0xcca8);      writeData16(0x00);
  writeCommand16(0xcca9);      writeData16(0x00);
  writeCommand16(0xccaa);      writeData16(0x00);
  writeCommand16(0xccab);      writeData16(0x00);
  writeCommand16(0xccac);      writeData16(0x00);
  writeCommand16(0xccad);      writeData16(0x00);
  writeCommand16(0xccae);      writeData16(0x00);
  writeCommand16(0xccb0);      writeData16(0x00);
  writeCommand16(0xccb1);      writeData16(0x25);
  writeCommand16(0xccb2);      writeData16(0x0C);
  writeCommand16(0xccb3);      writeData16(0x0A);
  writeCommand16(0xccb4);      writeData16(0x02);
  writeCommand16(0xccb5);      writeData16(0x26);
  writeCommand16(0xccb6);      writeData16(0x00);
  writeCommand16(0xccb7);      writeData16(0x00);
  writeCommand16(0xccb8);      writeData16(0x00);
  writeCommand16(0xccb9);      writeData16(0x00);
  writeCommand16(0xccc0);      writeData16(0x00);
  writeCommand16(0xccc1);      writeData16(0x00);
  writeCommand16(0xccc2);      writeData16(0x00);
  writeCommand16(0xccc3);      writeData16(0x00);
  writeCommand16(0xccc4);      writeData16(0x00);
  writeCommand16(0xccc5);      writeData16(0x00);
  writeCommand16(0xccc6);      writeData16(0x00);
  writeCommand16(0xccc7);      writeData16(0x00);
  writeCommand16(0xccc8);      writeData16(0x00);
  writeCommand16(0xccc9);      writeData16(0x00);
  writeCommand16(0xccca);      writeData16(0x00);
  writeCommand16(0xcccb);      writeData16(0x25);
  writeCommand16(0xcccc);      writeData16(0x0B);
  writeCommand16(0xcccd);      writeData16(0x09);
  writeCommand16(0xccce);      writeData16(0x01);
  writeCommand16(0xccd0);      writeData16(0x26);
  writeCommand16(0xccd1);      writeData16(0x00);
  writeCommand16(0xccd2);      writeData16(0x00);
  writeCommand16(0xccd3);      writeData16(0x00);
  writeCommand16(0xccd4);      writeData16(0x00);
  writeCommand16(0xccd5);      writeData16(0x00);
  writeCommand16(0xccd6);      writeData16(0x00);
  writeCommand16(0xccd7);      writeData16(0x00);
  writeCommand16(0xccd8);      writeData16(0x00);
  writeCommand16(0xccd9);      writeData16(0x00);
  writeCommand16(0xccda);      writeData16(0x00);
  writeCommand16(0xccdb);      writeData16(0x00);
  writeCommand16(0xccdc);      writeData16(0x00);
  writeCommand16(0xccdd);      writeData16(0x00);
  writeCommand16(0xccde);      writeData16(0x00);
  writeCommand16(0x3A00);      writeData16(0x55);

  writeCommand16(0x1100);
  delay(100);
  writeCommand16(0x2900);
  delay(50);
  writeCommand16(0x2C00);
  writeCommand16(0x2A00);     writeData16(0x00);
  writeCommand16(0x2A01);     writeData16(0x00);
  writeCommand16(0x2A02);     writeData16(0x01);
  writeCommand16(0x2A03);     writeData16(0xe0);
  writeCommand16(0x2B00);     writeData16(0x00);
  writeCommand16(0x2B01);     writeData16(0x00);
  writeCommand16(0x2B02);     writeData16(0x03);
  writeCommand16(0x2B03);     writeData16(0x20);
  endWrite();
  if (_bl_pin >= 0) pinMode(_bl_pin, OUTPUT);
  enableDisplay(true);
}

void GFX_OTM8009A::setRotation(uint8_t r)
{
  GFX_Root::setRotation(r);
  startWrite();
  writeCommand16(OTM8009A_MADCTL);
  switch (r & 3)
  {
    case 0:
      writeData(_bgr);
      break;
    case 1:
      writeData(OTM8009A_MADCTL_MX | OTM8009A_MADCTL_MV | _bgr);
      break;
    case 2:
      writeData(OTM8009A_MADCTL_MX | OTM8009A_MADCTL_MY | _bgr);
      break;
    case 3:
      writeData(OTM8009A_MADCTL_MY | OTM8009A_MADCTL_MV | _bgr);
      break;
  }
  endWrite();
}

void GFX_OTM8009A::invertDisplay(bool i)
{
  _bgr = i ? OTM8009A_MADCTL_BGR : 0x00;
  setRotation(rotation);
}


void GFX_OTM8009A::invert(bool i)
{
  invertDisplay(i);
}

void GFX_OTM8009A::enableDisplay(bool enable)
{
  //  startWrite();
  //  writeCommand(enable ? 0x29 : 0x28);  // Display ON / Display OFF
  //  endWrite();
  if (_bl_pin >= 0) digitalWrite(_bl_pin, enable ? HIGH : LOW);
  enableBacklight(enable); // for _plugin
}

void GFX_OTM8009A::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t xe = x + w - 1;
  uint16_t ye = y + h - 1;
  writeCommand16(0x2A00);
  writeData16(x >> 8);
  writeCommand16(0x2A01);
  writeData16(x & 0x00ff);
  writeCommand16(0x2A02);
  writeData16(xe >> 8);
  writeCommand16(0x2A03);
  writeData16(xe & 0x00ff);
  writeCommand16(0x2B00);
  writeData16(y >> 8);
  writeCommand16(0x2B01);
  writeData16(y & 0x00ff);
  writeCommand16(0x2B02);
  writeData16(ye >> 8);
  writeCommand16(0x2B03);
  writeData16(ye & 0x00ff);
  writeCommand16(0x2C00);
}
