// created by Jean-Marc Zingg to be the GFX_SSD1963 class for the GFX_TFT library
// code extracts taken from http://www.rinkydinkelectronics.com/download.php?f=UTFT.zip
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE

#include "GFX_SSD1963.h"

#define SPI_SPEED 40000000
#define MADCTL     0x36
#define MADCTL_BGR 0x08
#define RGB_or_BGR 0x00 // RGB

GFX_SSD1963::GFX_SSD1963(int8_t cs_pin, int8_t dc_pin, int8_t rst_pin, int8_t bl_pin) :
#if defined(ESP8266)
  GFX_TFT_IO(480, 800, cs_pin, dc_pin, rst_pin)
#else
  GFX_TFT_IO(480, 800, &SPI, cs_pin, dc_pin, rst_pin)
#endif
{
  _bl_pin = bl_pin;
  _bgr = RGB_or_BGR;
}

GFX_SSD1963::GFX_SSD1963(int8_t cs_pin, int8_t dc_pin, int8_t mosi_pin, int8_t sclk_pin, int8_t rst_pin, int8_t bl_pin) :
  GFX_TFT_IO(480, 800, cs_pin, dc_pin, mosi_pin, sclk_pin, rst_pin, -1)
{
  _bl_pin = bl_pin;
  _bgr = RGB_or_BGR;
}

GFX_SSD1963::GFX_SSD1963(SPIClass *spi, int8_t cs_pin, int8_t dc_pin, int8_t rst_pin, int8_t bl_pin) :
#if defined(ESP8266)
  GFX_TFT_IO(480, 800, cs_pin, dc_pin, rst_pin)
#else
  GFX_TFT_IO(480, 800, spi, cs_pin, dc_pin, rst_pin)
#endif
{
  (void) spi;
  _bl_pin = bl_pin;
  _bgr = RGB_or_BGR;
}

GFX_SSD1963::GFX_SSD1963(GFX_TFT_IO_Plugin& plugin) : GFX_TFT_IO(480, 800, &plugin)
{
  _bl_pin = -1;
  _bgr = RGB_or_BGR;
}

void GFX_SSD1963::begin(uint32_t freq)
{
  init(freq);
}

void GFX_SSD1963::init(uint32_t freq)
{
  Serial.println("init");
  if (!freq) freq = SPI_SPEED;
  initSPI(freq);
  startWrite();
  writeCommand(0xE2);    //PLL multiplier, set PLL clock to 120M
  writeData(0x23);     //N=0x36 for 6.5M, 0x23 for 10M crystal
  writeData(0x02);
  writeData(0x04); // original SSD1963_800ALT
  writeCommand(0xE0);    // PLL enable
  writeData(0x01);
  delay(10);
  writeCommand(0xE0);
  writeData(0x03);
  delay(10);
  writeCommand(0x01);    // software reset
  delay(100);
  writeCommand(0xE6);    //PLL setting for PCLK, depends on resolution
  writeData(0x04);
  writeData(0x93);
  writeData(0xE0);

  writeCommand(0xB0);    //LCD SPECIFICATION
  writeData(0x00); // 0x24
  writeData(0x00);
  writeData(0x03);   //Set HDP 799
  writeData(0x1F);
  writeData(0x01);   //Set VDP 479
  writeData(0xDF);
  writeData(0x00);

  writeCommand(0xB4);    //HSYNC
  writeData(0x03);   //Set HT  928
  writeData(0xA0);
  writeData(0x00);   //Set HPS 46
  writeData(0x2E);
  writeData(0x30);   //Set HPW 48
  writeData(0x00);   //Set LPS 15
  writeData(0x0F);
  writeData(0x00);

  writeCommand(0xB6);    //VSYNC
  writeData(0x02);   //Set VT  525
  writeData(0x0D);
  writeData(0x00);   //Set VPS 16
  writeData(0x10);
  writeData(0x10);   //Set VPW 16
  writeData(0x00);   //Set FPS 8
  writeData(0x08);

  writeCommand(0xBA);
  //writeData(0x05);   //GPIO[3:0] out 1
  writeData(0x07);   //GPIO[3:0] out 1

  writeCommand(0xB8);
  writeData(0x07);     //GPIO3=input, GPIO[2:0]=output
  writeData(0x01);   //GPIO0 normal

  writeCommand(MADCTL);    //rotation
  writeData(0x20);   // -- Set to 0x20 to rotate 180 degrees

  writeCommand(0xF0);    //pixel data interface
  writeData(0x03);
  endWrite();

  delay(10);

  startWrite();
  // SetWindow, physical addresses, even if default rotation is changed
  writeCommand(0x2a);
  writeData(0x00);
  writeData(0x00);
  writeData(0x03);
  writeData(0x1F);
  writeCommand(0x2b);
  writeData(0x00);
  writeData(0x00);
  writeData(0x01);
  writeData(0xDF);

  writeCommand(0x29); // display on

  writeCommand(0xBE); // set PWM for B/L (set_pwm_conf)
  writeData(0x06); // PWMF (frequency)
  writeData(0xF0); // PWM  (duty cycle 240/256 94%)
  writeData(0x01); // PWM conf (PWM enable, controlled by host)
  writeData(0xF0); // DBC maximum brightness (94%)
  writeData(0x00); // DBC minimum brighness (0%)
  writeData(0x00); // Brightness prescaler (off)

  writeCommand(0xD0); // set_dbc_conf
  writeData(0x0D); // DBC enable, agressive mode

  writeCommand(0x2C);
  endWrite();
  enableDisplay(true);
}

void GFX_SSD1963::setRotation(uint8_t r)
{
  GFX_Root::setRotation(r);
  startWrite();
  writeCommand(MADCTL);
  switch (r & 3)
  {
    case 0:
      writeData(0x33 | _bgr);
      break;
    case 1:
      writeData(0x2 | _bgr);
      break;
    case 2:
      writeData(0x20 | _bgr);
      break;
    case 3:
      writeData(0x11 | _bgr);
      break;
  }
  endWrite();
}

void GFX_SSD1963::invertDisplay(bool i)
{
  _bgr = i ? 0x00 : MADCTL_BGR;
  setRotation(rotation);
}


void GFX_SSD1963::invert(bool i)
{
  invertDisplay(i);
}

void GFX_SSD1963::enableDisplay(bool enable)
{
  //  startWrite();
  //  writeCommand(enable ? 0x29 : 0x28);  // Display ON / Display OFF
  //  endWrite();
  if (_bl_pin >= 0) digitalWrite(_bl_pin, enable ? HIGH : LOW);
  enableBacklight(enable); // for _plugin
}

void GFX_SSD1963::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t xe = x + w - 1;
  uint16_t ye = y + h - 1;
  if (rotation & 1) // landscape
  {
    writeCommand(0x2a);
    writeData(x >> 8);
    writeData(x); // XSTART
    writeData(xe >> 8);
    writeData(xe); // XEND
    writeCommand(0x2b);
    writeData(y >> 8);
    writeData(y);        // YSTART
    writeData(ye >> 8);
    writeData(ye);        // YEND
    writeCommand(0x2c);
  }
  else // portrait
  {
    // transform to physical addresses
    writeCommand(0x2b); // by switching address
    writeData(x >> 8);
    writeData(x); // XSTART
    writeData(xe >> 8);
    writeData(xe); // XEND
    writeCommand(0x2a); // by switching address
    writeData(y >> 8);
    writeData(y);        // YSTART
    writeData(ye >> 8);
    writeData(ye);        // YEND
    writeCommand(0x2c);
  }
  writeCommand(0x2c);
}
