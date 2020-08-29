// created by Jean-Marc Zingg to be the GFX_ILI9806 class for the GFX_TFT library
// code extracts taken from code and documentation from Ruijia Industry (Tiky_LCD.h, Tiky_LCD.c)
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// note: readRect does not work correctly with my only ILI9806 display (pixel sequence)
//       workaround added

#include "GFX_ILI9806.h"

#define SPI_SPEED 40000000

#define CASET 0x2A
#define PASET 0x2B
#define RAMWR 0x2C
#define RAMRD 0x2E
#define MADCTL 0x36
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08
#define MADCTL_SS  0x02
#define MADCTL_GS  0x01

#define RGB_or_BGR 0x00 // RGB

GFX_ILI9806::GFX_ILI9806(int8_t cs_pin, int8_t dc_pin, int8_t mosi_pin, int8_t sclk_pin, int8_t rst_pin, int8_t bl_pin) :
  GFX_TFT_IO(480, 854, cs_pin, dc_pin, mosi_pin, sclk_pin, rst_pin, -1)
{
  _bl_pin = bl_pin;
  _bgr = RGB_or_BGR;
}

GFX_ILI9806::GFX_ILI9806(SPIClass *spi, int8_t cs_pin, int8_t dc_pin, int8_t rst_pin, int8_t bl_pin) :
#if defined(ESP8266)
  GFX_TFT_IO(480, 854, cs_pin, dc_pin, rst_pin)
#else
  GFX_TFT_IO(480, 854, spi, cs_pin, dc_pin, rst_pin)
#endif
{
  (void) spi;
  _bl_pin = bl_pin;
  _bgr = RGB_or_BGR;
}

GFX_ILI9806::GFX_ILI9806(GFX_TFT_IO_Plugin& plugin) : GFX_TFT_IO(480, 854, &plugin)
{
  _bl_pin = -1;
  _bgr = RGB_or_BGR;
}

void GFX_ILI9806::begin(uint32_t freq)
{
  init(freq);
}

void GFX_ILI9806::init(uint32_t freq)
{
  Serial.println("init");
  if (!freq) freq = SPI_SPEED;
  initSPI(freq);
  // Configure ILI9806 display
  //************* Start Initial Sequence **********//
  _writeCommandTransaction(0xFF); // EXTC Command Set enable register
  _writeDataTransaction(0xFF);
  _writeDataTransaction(0x98);
  _writeDataTransaction(0x06);

  _writeCommandTransaction(0xBA); // SPI Interface Setting
  _writeDataTransaction(0xE0);

  _writeCommandTransaction(0xBC); // GIP 1
  _writeDataTransaction(0x03);
  _writeDataTransaction(0x0F);
  _writeDataTransaction(0x63);
  _writeDataTransaction(0x69);
  _writeDataTransaction(0x01);
  _writeDataTransaction(0x01);
  _writeDataTransaction(0x1B);
  _writeDataTransaction(0x11);
  _writeDataTransaction(0x70);
  _writeDataTransaction(0x73);
  _writeDataTransaction(0xFF);
  _writeDataTransaction(0xFF);
  _writeDataTransaction(0x08);
  _writeDataTransaction(0x09);
  _writeDataTransaction(0x05);
  _writeDataTransaction(0x00);
  _writeDataTransaction(0xEE);
  _writeDataTransaction(0xE2);
  _writeDataTransaction(0x01);
  _writeDataTransaction(0x00);
  _writeDataTransaction(0xC1);

  _writeCommandTransaction(0xBD); // GIP 2
  _writeDataTransaction(0x01);
  _writeDataTransaction(0x23);
  _writeDataTransaction(0x45);
  _writeDataTransaction(0x67);
  _writeDataTransaction(0x01);
  _writeDataTransaction(0x23);
  _writeDataTransaction(0x45);
  _writeDataTransaction(0x67);

  _writeCommandTransaction(0xBE); // GIP 3
  _writeDataTransaction(0x00);
  _writeDataTransaction(0x22);
  _writeDataTransaction(0x27);
  _writeDataTransaction(0x6A);
  _writeDataTransaction(0xBC);
  _writeDataTransaction(0xD8);
  _writeDataTransaction(0x92);
  _writeDataTransaction(0x22);
  _writeDataTransaction(0x22);

  _writeCommandTransaction(0xC7); // Vcom
  _writeDataTransaction(0x1E);

  _writeCommandTransaction(0xED); // EN_volt_reg
  _writeDataTransaction(0x7F);
  _writeDataTransaction(0x0F);
  _writeDataTransaction(0x00);

  _writeCommandTransaction(0xC0); // Power Control 1
  _writeDataTransaction(0xE3);
  _writeDataTransaction(0x0B);
  _writeDataTransaction(0x00);

  _writeCommandTransaction(0xFC);
  _writeDataTransaction(0x08);

  _writeCommandTransaction(0xDF); // Engineering Setting
  _writeDataTransaction(0x00);
  _writeDataTransaction(0x00);
  _writeDataTransaction(0x00);
  _writeDataTransaction(0x00);
  _writeDataTransaction(0x00);
  _writeDataTransaction(0x02);

  _writeCommandTransaction(0xF3); // DVDD Voltage Setting
  _writeDataTransaction(0x74);

  _writeCommandTransaction(0xB4); // Display Inversion Control
  _writeDataTransaction(0x00);
  _writeDataTransaction(0x00);
  _writeDataTransaction(0x00);

  _writeCommandTransaction(0xF7); // 480x854
  _writeDataTransaction(0x81);

  _writeCommandTransaction(0xB1); // Frame Rate
  _writeDataTransaction(0x00);
  _writeDataTransaction(0x10);
  _writeDataTransaction(0x14);

  _writeCommandTransaction(0xF1); // Panel Timing Control
  _writeDataTransaction(0x29);
  _writeDataTransaction(0x8A);
  _writeDataTransaction(0x07);

  _writeCommandTransaction(0xF2); //Panel Timing Control
  _writeDataTransaction(0x40);
  _writeDataTransaction(0xD2);
  _writeDataTransaction(0x50);
  _writeDataTransaction(0x28);

  _writeCommandTransaction(0xC1); // Power Control 2
  _writeDataTransaction(0x17);
  _writeDataTransaction(0X85);
  _writeDataTransaction(0x85);
  _writeDataTransaction(0x20);

  _writeCommandTransaction(0xE0);
  _writeDataTransaction(0x00); //P1
  _writeDataTransaction(0x0C); //P2
  _writeDataTransaction(0x15); //P3
  _writeDataTransaction(0x0D); //P4
  _writeDataTransaction(0x0F); //P5
  _writeDataTransaction(0x0C); //P6
  _writeDataTransaction(0x07); //P7
  _writeDataTransaction(0x05); //P8
  _writeDataTransaction(0x07); //P9
  _writeDataTransaction(0x0B); //P10
  _writeDataTransaction(0x10); //P11
  _writeDataTransaction(0x10); //P12
  _writeDataTransaction(0x0D); //P13
  _writeDataTransaction(0x17); //P14
  _writeDataTransaction(0x0F); //P15
  _writeDataTransaction(0x00); //P16

  _writeCommandTransaction(0xE1);
  _writeDataTransaction(0x00); //P1
  _writeDataTransaction(0x0D); //P2
  _writeDataTransaction(0x15); //P3
  _writeDataTransaction(0x0E); //P4
  _writeDataTransaction(0x10); //P5
  _writeDataTransaction(0x0D); //P6
  _writeDataTransaction(0x08); //P7
  _writeDataTransaction(0x06); //P8
  _writeDataTransaction(0x07); //P9
  _writeDataTransaction(0x0C); //P10
  _writeDataTransaction(0x11); //P11
  _writeDataTransaction(0x11); //P12
  _writeDataTransaction(0x0E); //P13
  _writeDataTransaction(0x17); //P14
  _writeDataTransaction(0x0F); //P15
  _writeDataTransaction(0x00); //P16

  _writeCommandTransaction(0x35); //Tearing Effect ON
  _writeDataTransaction(0x00);

  _writeCommandTransaction(0x36);
  _writeDataTransaction(0x60);

  _writeCommandTransaction(0x3A);
  _writeDataTransaction(0x55);

  _writeCommandTransaction(0x11); //Exit Sleep
  delay(120);
  _writeCommandTransaction(0x29); // Display On
  delay(10);

  _writeCommandTransaction(0x3A);
  _writeDataTransaction(0x55);
  _writeCommandTransaction(0x36);
  _writeDataTransaction(0x0 | RGB_or_BGR); // portrait, RGB_or_BGR
  enableDisplay(true);
}

void GFX_ILI9806::setRotation(uint8_t r)
{
  GFX_Root::setRotation(r);
  startWrite();
  writeCommand(MADCTL);
  switch (r & 3)
  {
    case 0:
      writeData(0 | _bgr);
      break;
    case 1:
      writeData(MADCTL_MX | MADCTL_MV | _bgr);
      break;
    case 2:
      writeData(MADCTL_MX | MADCTL_MY | _bgr);
      break;
    case 3:
      writeData(MADCTL_MY | MADCTL_MV | _bgr);
      break;
  }
  endWrite();
}

void GFX_ILI9806::invertDisplay(bool i)
{
  _bgr = i ? 0x00 : MADCTL_BGR;
  setRotation(rotation);
}


void GFX_ILI9806::invert(bool i)
{
  invertDisplay(i);
}

void GFX_ILI9806::enableDisplay(bool enable)
{
  startWrite();
  writeCommand(enable ? 0x29 : 0x28);  // Display ON / Display OFF
  endWrite();
  if (_bl_pin >= 0) digitalWrite(_bl_pin, enable ? HIGH : LOW);
  enableBacklight(enable); // for _plugin
}

void GFX_ILI9806::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t xe = x + w - 1;
  uint16_t ye = y + h - 1;
  writeCommand(CASET);  // Column addr set
  writeData(x >> 8);
  writeData(x & 0xFF); // XSTART
  writeData(xe >> 8);
  writeData(xe & 0xFF); // XEND
  writeCommand(PASET);  // Row addr set
  writeData(y >> 8);
  writeData(y);        // YSTART
  writeData(ye >> 8);
  writeData(ye);        // YEND
  writeCommand(RAMWR);  // write to RAM
}

void GFX_ILI9806::_writeCommandTransaction(uint8_t cmd)
{
  startWrite();
  writeCommand(cmd);
  endWrite();
}

void GFX_ILI9806::_writeDataTransaction(uint8_t data)
{
  startWrite();
  writeData(data);
  endWrite();
}
