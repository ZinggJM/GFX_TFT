// created by Jean-Marc Zingg to be the GFX_ILI9341 class for the GFX_TFT library
// code extracts taken from https://github.com/Bodmer/TFT_HX8357
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//

#include "GFX_ILI9341.h"

#define SPI_SPEED 40000000

// These are the ILI9341 control registers
#define ILI9341_NOP     0x00
#define ILI9341_SWRESET 0x01
#define ILI9341_RDDID   0x04
#define ILI9341_RDDST   0x09

#define ILI9341_SLPIN   0x10
#define ILI9341_SLPOUT  0x11
#define ILI9341_PTLON   0x12
#define ILI9341_NORON   0x13

#define ILI9341_RDMODE  0x0A
#define ILI9341_RDMADCTL  0x0B
#define ILI9341_RDPIXFMT  0x0C
#define ILI9341_RDIMGFMT  0x0A
#define ILI9341_RDSELFDIAG  0x0F

#define ILI9341_INVOFF  0x20
#define ILI9341_INVON   0x21
#define ILI9341_GAMMASET 0x26
#define ILI9341_DISPOFF 0x28
#define ILI9341_DISPON  0x29

#define ILI9341_CASET   0x2A
#define ILI9341_PASET   0x2B
#define ILI9341_RAMWR   0x2C
#define ILI9341_RAMRD   0x2E

#define ILI9341_PTLAR   0x30
#define ILI9341_VSCRDEF 0x33
#define ILI9341_MADCTL  0x36
#define ILI9341_VSCRSADD 0x37
#define ILI9341_PIXFMT  0x3A

#define ILI9341_FRMCTR1 0xB1
#define ILI9341_FRMCTR2 0xB2
#define ILI9341_FRMCTR3 0xB3
#define ILI9341_INVCTR  0xB4
#define ILI9341_DFUNCTR 0xB6

#define ILI9341_PWCTR1  0xC0
#define ILI9341_PWCTR2  0xC1
#define ILI9341_PWCTR3  0xC2
#define ILI9341_PWCTR4  0xC3
#define ILI9341_PWCTR5  0xC4
#define ILI9341_VMCTR1  0xC5
#define ILI9341_VMCTR2  0xC7

#define ILI9341_RDID1   0xDA
#define ILI9341_RDID2   0xDB
#define ILI9341_RDID3   0xDC
#define ILI9341_RDID4   0xDD

#define ILI9341_GMCTRP1 0xE0
#define ILI9341_GMCTRN1 0xE1

#define ILI9341_MADCTL_MY  0x80
#define ILI9341_MADCTL_MX  0x40
#define ILI9341_MADCTL_MV  0x20
#define ILI9341_MADCTL_ML  0x10
#define ILI9341_MADCTL_RGB 0x00
#define ILI9341_MADCTL_BGR 0x08
#define ILI9341_MADCTL_MH  0x04

GFX_ILI9341::GFX_ILI9341(int8_t cs_pin, int8_t dc_pin, int8_t rst_pin, int8_t bl_pin) :
#if defined(ESP8266)
  GFX_TFT_IO(240, 320, cs_pin, dc_pin, rst_pin)
#else
  GFX_TFT_IO(240, 320, &SPI, cs_pin, dc_pin, rst_pin)
#endif
{
  _bl_pin = bl_pin;
  _bgr = ILI9341_MADCTL_BGR;
}

GFX_ILI9341::GFX_ILI9341(int8_t cs_pin, int8_t dc_pin, int8_t mosi_pin, int8_t sclk_pin, int8_t rst_pin, int8_t bl_pin) :
  GFX_TFT_IO(240, 320, cs_pin, dc_pin, mosi_pin, sclk_pin, rst_pin, -1)
{
  _bl_pin = bl_pin;
  _bgr = ILI9341_MADCTL_BGR;
}

GFX_ILI9341::GFX_ILI9341(SPIClass *spi, int8_t cs_pin, int8_t dc_pin, int8_t rst_pin, int8_t bl_pin) :
#if defined(ESP8266)
  GFX_TFT_IO(240, 320, cs_pin, dc_pin, rst_pin)
#else
  GFX_TFT_IO(240, 320, spi, cs_pin, dc_pin, rst_pin)
#endif
{
  (void) spi;
  _bl_pin = bl_pin;
  _bgr = ILI9341_MADCTL_BGR;
}

GFX_ILI9341::GFX_ILI9341(GFX_TFT_IO_Plugin& plugin) : GFX_TFT_IO(240, 320, &plugin)
{
  _bl_pin = -1;
  _bgr = ILI9341_MADCTL_BGR;
}

void GFX_ILI9341::begin(uint32_t freq)
{
  init(freq);
}

void GFX_ILI9341::init(uint32_t freq)
{
  Serial.println("init");
  if (!freq) freq = SPI_SPEED;
  initSPI(freq);
  startWrite();
  writeCommand(0xEF);
  writeData(0x03);
  writeData(0x80);
  writeData(0x02);

  writeCommand(0xCF);
  writeData(0x00);
  writeData(0XC1);
  writeData(0X30);

  writeCommand(0xED);
  writeData(0x64);
  writeData(0x03);
  writeData(0X12);
  writeData(0X81);

  writeCommand(0xE8);
  writeData(0x85);
  writeData(0x00);
  writeData(0x78);

  writeCommand(0xCB);
  writeData(0x39);
  writeData(0x2C);
  writeData(0x00);
  writeData(0x34);
  writeData(0x02);

  writeCommand(0xF7);
  writeData(0x20);

  writeCommand(0xEA);
  writeData(0x00);
  writeData(0x00);

  writeCommand(ILI9341_PWCTR1);    //Power control
  writeData(0x23);   //VRH[5:0]

  writeCommand(ILI9341_PWCTR2);    //Power control
  writeData(0x10);   //SAP[2:0];BT[3:0]

  writeCommand(ILI9341_VMCTR1);    //VCM control
  writeData(0x3e);
  writeData(0x28);

  writeCommand(ILI9341_VMCTR2);    //VCM control2
  writeData(0x86);  //--

  writeCommand(ILI9341_MADCTL);    // Memory Access Control
  writeData(0x48);

  writeCommand(ILI9341_PIXFMT);
  writeData(0x55);

  writeCommand(ILI9341_FRMCTR1);
  writeData(0x00);
  writeData(0x18);

  writeCommand(ILI9341_DFUNCTR);    // Display Function Control
  writeData(0x08);
  writeData(0x82);
  writeData(0x27);

  writeCommand(0xF2);    // 3Gamma Function Disable
  writeData(0x00);

  writeCommand(ILI9341_GAMMASET);    //Gamma curve selected
  writeData(0x01);

  writeCommand(ILI9341_GMCTRP1);    //Set Gamma
  writeData(0x0F);
  writeData(0x31);
  writeData(0x2B);
  writeData(0x0C);
  writeData(0x0E);
  writeData(0x08);
  writeData(0x4E);
  writeData(0xF1);
  writeData(0x37);
  writeData(0x07);
  writeData(0x10);
  writeData(0x03);
  writeData(0x0E);
  writeData(0x09);
  writeData(0x00);

  writeCommand(ILI9341_GMCTRN1);    //Set Gamma
  writeData(0x00);
  writeData(0x0E);
  writeData(0x14);
  writeData(0x03);
  writeData(0x11);
  writeData(0x07);
  writeData(0x31);
  writeData(0xC1);
  writeData(0x48);
  writeData(0x08);
  writeData(0x0F);
  writeData(0x0C);
  writeData(0x31);
  writeData(0x36);
  writeData(0x0F);

  writeCommand(ILI9341_SLPOUT);    //Exit Sleep
  endWrite();
  delay(120);
  startWrite();
  writeCommand(ILI9341_DISPON);    //Display on
  endWrite();
  if (_bl_pin >= 0) pinMode(_bl_pin, OUTPUT);
  enableDisplay(true);
}

void GFX_ILI9341::setRotation(uint8_t r)
{
  GFX_Root::setRotation(r);
  startWrite();
  writeCommand(ILI9341_MADCTL);
  switch (r & 3)
  {
    case 0: // Portrait
      writeData(ILI9341_MADCTL_MX | _bgr);
      break;
    case 1: // Landscape (Portrait + 90)
      writeData(ILI9341_MADCTL_MV | _bgr);
      break;
    case 2: // Inverter portrait
      writeData(ILI9341_MADCTL_MY | _bgr);
      break;
    case 3: // Inverted landscape
      writeData(ILI9341_MADCTL_MV | ILI9341_MADCTL_MX | ILI9341_MADCTL_MY | _bgr);
      break;
  }
  endWrite();
}

void GFX_ILI9341::invertDisplay(bool i)
{
  _bgr = i ? 0x00 : ILI9341_MADCTL_BGR;
  setRotation(rotation);
}


void GFX_ILI9341::invert(bool i)
{
  invertDisplay(i);
}

void GFX_ILI9341::enableDisplay(bool enable)
{
  startWrite();
  writeCommand(enable ? 0x29 : 0x28);  // Display ON / Display OFF
  endWrite();
  if (_bl_pin >= 0) digitalWrite(_bl_pin, enable ? HIGH : LOW);
  enableBacklight(enable); // for _plugin
}

void GFX_ILI9341::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t xe = x + w - 1;
  uint16_t ye = y + h - 1;
  writeCommand(ILI9341_CASET);  // Column addr set
  writeData(x >> 8);
  writeData(x); // XSTART
  writeData(xe >> 8);
  writeData(xe); // XEND
  writeCommand(ILI9341_PASET);  // Row addr set
  writeData(y >> 8);
  writeData(y);        // YSTART
  writeData(ye >> 8);
  writeData(ye);        // YEND
  writeCommand(ILI9341_RAMWR);  // write to RAM
}
