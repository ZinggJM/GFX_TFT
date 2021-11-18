// created by Jean-Marc Zingg to be the GFX_SPI_ILI9486 class for the GFX_TFT library
// code extracts taken from https://github.com/Bodmer/TFT_HX8357
// spi kludge handling solution found in https://github.com/Bodmer/TFT_eSPI
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//

#include "GFX_SPI_ILI9486.h"

#define SPI_SPEED 20000000ul // max reliable speed is 20Mhz for RPi SPI kludge
//#define SPI_SPEED 4000000

#define ILI9486_CASET 0x2A
#define ILI9486_PASET 0x2B
#define ILI9486_RAMWR 0x2C
#define ILI9486_RAMRD   0x2E
#define ILI9486_MADCTL 0x36
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_BGR 0x08
#define MADCTL_MH  0x04

GFX_SPI_ILI9486::GFX_SPI_ILI9486(int16_t cs_pin, int16_t dc_pin, int16_t rst_pin) :
#if defined(ESP8266)
  SPI_GFX_Class(320, 480, cs_pin, dc_pin, rst_pin)
#else
  SPI_GFX_Class(320, 480, &SPI, cs_pin, dc_pin, rst_pin)
#endif
{
  _spi16_mode = true;
  _bgr = MADCTL_BGR;
}

GFX_SPI_ILI9486::GFX_SPI_ILI9486(int16_t cs_pin, int16_t dc_pin, int16_t mosi_pin, int16_t sclk_pin, int16_t rst_pin) :
  SPI_GFX_Class(320, 480, cs_pin, dc_pin, mosi_pin, sclk_pin, rst_pin, -1)
{
  _spi16_mode = true;
  _bgr = MADCTL_BGR;
}

GFX_SPI_ILI9486::GFX_SPI_ILI9486(uint16_t width, uint16_t height,
                                 SPIClass *spi, int16_t cs_pin, int16_t dc_pin, int16_t rst_pin) :
#if defined(ESP8266)
  SPI_GFX_Class(width, height, cs_pin, dc_pin, rst_pin)
#else
  SPI_GFX_Class(width, height, spi, cs_pin, dc_pin, rst_pin)
#endif
{
  (void) spi;
  _spi16_mode = true;
  _bgr = MADCTL_BGR;
}

GFX_SPI_ILI9486::GFX_SPI_ILI9486(uint16_t width, uint16_t height,
                                 int16_t cs_pin, int16_t dc_pin, int16_t mosi_pin, int16_t sclk_pin, int16_t rst_pin) :
  SPI_GFX_Class(width, height, cs_pin, dc_pin, mosi_pin, sclk_pin, rst_pin, -1)
{
  _spi16_mode = true;
  _bgr = MADCTL_BGR;
}

void GFX_SPI_ILI9486::setSpiKludge(bool rpi_spi16_mode)
{
  _spi16_mode = rpi_spi16_mode;
}

void GFX_SPI_ILI9486::begin(uint32_t freq)
{
  init(freq);
}

void GFX_SPI_ILI9486::init(uint32_t freq)
{
  Serial.println("init");
  if (!freq) freq = SPI_SPEED;
  initSPI(freq);
  startWrite();
  if (_spi16_mode)
  {
    writeCommand16(0x3A);
    SPI_WRITE16(0x55);  // use 16 bits per pixel color
    writeCommand16(0x36);
    SPI_WRITE16(0x48);  // MX, BGR == rotation 0
    // PGAMCTRL(Positive Gamma Control)
    writeCommand16(0xE0);
    SPI_WRITE16(0x0F);
    SPI_WRITE16(0x1F);
    SPI_WRITE16(0x1C);
    SPI_WRITE16(0x0C);
    SPI_WRITE16(0x0F);
    SPI_WRITE16(0x08);
    SPI_WRITE16(0x48);
    SPI_WRITE16(0x98);
    SPI_WRITE16(0x37);
    SPI_WRITE16(0x0A);
    SPI_WRITE16(0x13);
    SPI_WRITE16(0x04);
    SPI_WRITE16(0x11);
    SPI_WRITE16(0x0D);
    SPI_WRITE16(0x00);
    // NGAMCTRL(Negative Gamma Control)
    writeCommand16(0xE1);
    SPI_WRITE16(0x0F);
    SPI_WRITE16(0x32);
    SPI_WRITE16(0x2E);
    SPI_WRITE16(0x0B);
    SPI_WRITE16(0x0D);
    SPI_WRITE16(0x05);
    SPI_WRITE16(0x47);
    SPI_WRITE16(0x75);
    SPI_WRITE16(0x37);
    SPI_WRITE16(0x06);
    SPI_WRITE16(0x10);
    SPI_WRITE16(0x03);
    SPI_WRITE16(0x24);
    SPI_WRITE16(0x20);
    SPI_WRITE16(0x00);
    // Digital Gamma Control 1
    writeCommand16(0xE2);
    SPI_WRITE16(0x0F);
    SPI_WRITE16(0x32);
    SPI_WRITE16(0x2E);
    SPI_WRITE16(0x0B);
    SPI_WRITE16(0x0D);
    SPI_WRITE16(0x05);
    SPI_WRITE16(0x47);
    SPI_WRITE16(0x75);
    SPI_WRITE16(0x37);
    SPI_WRITE16(0x06);
    SPI_WRITE16(0x10);
    SPI_WRITE16(0x03);
    SPI_WRITE16(0x24);
    SPI_WRITE16(0x20);
    SPI_WRITE16(0x00);
    writeCommand16(0x11);  // Sleep OUT
    delay(150);   // wait some time
    writeCommand16(0x29);  // Display ON
  }
  else
  {
    writeCommand(0x3A);
    spiWrite(0x66);  // 18 bit colour for native SPI
    writeCommand(0x36);
    spiWrite(0x48);  // MX, BGR == rotation 0
    // PGAMCTRL(Positive Gamma Control)
    writeCommand(0xE0);
    spiWrite(0x0F);
    spiWrite(0x1F);
    spiWrite(0x1C);
    spiWrite(0x0C);
    spiWrite(0x0F);
    spiWrite(0x08);
    spiWrite(0x48);
    spiWrite(0x98);
    spiWrite(0x37);
    spiWrite(0x0A);
    spiWrite(0x13);
    spiWrite(0x04);
    spiWrite(0x11);
    spiWrite(0x0D);
    spiWrite(0x00);
    // NGAMCTRL(Negative Gamma Control)
    writeCommand(0xE1);
    spiWrite(0x0F);
    spiWrite(0x32);
    spiWrite(0x2E);
    spiWrite(0x0B);
    spiWrite(0x0D);
    spiWrite(0x05);
    spiWrite(0x47);
    spiWrite(0x75);
    spiWrite(0x37);
    spiWrite(0x06);
    spiWrite(0x10);
    spiWrite(0x03);
    spiWrite(0x24);
    spiWrite(0x20);
    spiWrite(0x00);
    // Digital Gamma Control 1
    writeCommand(0xE2);
    spiWrite(0x0F);
    spiWrite(0x32);
    spiWrite(0x2E);
    spiWrite(0x0B);
    spiWrite(0x0D);
    spiWrite(0x05);
    spiWrite(0x47);
    spiWrite(0x75);
    spiWrite(0x37);
    spiWrite(0x06);
    spiWrite(0x10);
    spiWrite(0x03);
    spiWrite(0x24);
    spiWrite(0x20);
    spiWrite(0x00);
    writeCommand(0x11);  // Sleep OUT
    delay(150);   // wait some time
    writeCommand(0x29);  // Display ON
  }
  endWrite();
}

void GFX_SPI_ILI9486::setRotation(uint8_t r)
{
  GFX_Root::setRotation(r);
  startWrite();
  if (_spi16_mode)
  {
    writeCommand16(ILI9486_MADCTL);
    switch (r & 3)
    {
      case 0:
        SPI_WRITE16(MADCTL_MX | _bgr);
        break;
      case 1:
        SPI_WRITE16(MADCTL_MV | _bgr);
        break;
      case 2:
        SPI_WRITE16(MADCTL_MY | _bgr);
        break;
      case 3:
        SPI_WRITE16(MADCTL_MX | MADCTL_MY | MADCTL_MV | _bgr);
        break;
    }
  }
  else
  {
    writeCommand(ILI9486_MADCTL);
    switch (r & 3)
    {
      case 0:
        spiWrite(MADCTL_MX | _bgr);
        break;
      case 1:
        spiWrite(MADCTL_MV | _bgr);
        break;
      case 2:
        spiWrite(MADCTL_MY | _bgr);
        break;
      case 3:
        spiWrite(MADCTL_MX | MADCTL_MY | MADCTL_MV | _bgr);
        break;
    }
  }
  endWrite();
}

void GFX_SPI_ILI9486::invertDisplay(bool i)
{
  _bgr = i ? MADCTL_BGR : 0x00;
  setRotation(rotation);
}


void GFX_SPI_ILI9486::invert(bool i)
{
  invertDisplay(i);
}

void GFX_SPI_ILI9486::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t xe = x + w - 1;
  uint16_t ye = y + h - 1;
  if (_spi16_mode)
  {
#if defined(_GFX_TFT_IO_H_)
    writeCommand16(ILI9486_CASET);
    uint16_t columns[] = {uint16_t(x >> 8), uint16_t(x & 0xFF), uint16_t(xe >> 8), uint16_t(xe & 0xFF)};
    writeData16(columns, 4);
    writeCommand16(ILI9486_PASET);
    uint16_t rows[] = {uint16_t(y >> 8), uint16_t(y & 0xFF), uint16_t(ye >> 8), uint16_t(ye & 0xFF)};
    writeData16(rows, 4);
    writeCommand16(ILI9486_RAMWR);
#else
    writeCommand16(ILI9486_CASET);
    SPI_WRITE16(x >> 8);
    SPI_WRITE16(x & 0xFF);
    SPI_WRITE16(xe >> 8);
    SPI_WRITE16(xe & 0xFF);
    writeCommand16(ILI9486_PASET);
    SPI_WRITE16(y >> 8);
    SPI_WRITE16(y & 0xFF);
    SPI_WRITE16(ye >> 8);
    SPI_WRITE16(ye & 0xFF);
    writeCommand16(ILI9486_RAMWR);
#endif
  }
  else
  {
    writeCommand(ILI9486_CASET);
    SPI_WRITE16(x);
    SPI_WRITE16(xe);
    writeCommand(ILI9486_PASET);
    SPI_WRITE16(y);
    SPI_WRITE16(ye);
    writeCommand(ILI9486_RAMWR);
  }
}

void GFX_SPI_ILI9486::enableDisplay(bool enable)
{
  startWrite();
  if (_spi16_mode) writeCommand16(enable ? 0x29 : 0x28);  // Display ON / Display OFF
  else  writeCommand(enable ? 0x29 : 0x28);  // Display ON / Display OFF
  endWrite();
}

void GFX_SPI_ILI9486::writePixel(int16_t x, int16_t y, uint16_t color)
{
  // Clip first...
  if ((x >= 0) && (x < _width) && (y >= 0) && (y < _height))
  {
    // THEN set up transaction (if needed) and draw...
    setAddrWindow(x, y, 1, 1);
    _writeColor16(color, 1);
  }
  //Serial.print(".");
}

void GFX_SPI_ILI9486::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  //  if ((x < 0) || (y < 0) || (w < 1) || (h < 1) || (x + w > _width) || (y + h > _height))
  //  {
  //    Serial.print("writeFillRect("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(") oops? "); delay(1);
  //  }
  // a correct clipping is the goal. try to achieve this
  if (x < 0) w += x, x = 0;
  if (y < 0) h += y, y = 0;
  if (x + w > _width) w = _width - x;
  if (y + h > _height) h = _height - y;
  if ((w < 1) || (h < 1)) return;
  setAddrWindow(x, y, w, h);
  _writeColor16(color, uint32_t(w) * uint32_t(h));
}

void GFX_SPI_ILI9486::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  // Clip first...
  if ((x >= 0) && (x < _width) && (y >= 0) && (y < _height))
  {
    // THEN set up transaction (if needed) and draw...
    startWrite();
    setAddrWindow(x, y, 1, 1);
    _writeColor16(color, 1);
    endWrite();
  }
  //Serial.print(".");
}

void GFX_SPI_ILI9486::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  //  if ((x < 0) || (y < 0) || (w < 1) || (h < 1) || (x + w > _width) || (y + h > _height))
  //  {
  //    Serial.print("fillRect("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(") oops? "); delay(1);
  //  }
  // a correct clipping is the goal. try to achieve this
  if (x < 0) w += x, x = 0;
  if (y < 0) h += y, y = 0;
  if (x + w > _width) w = _width - x;
  if (y + h > _height) h = _height - y;
  if ((w < 1) || (h < 1)) return;
  startWrite();
  setAddrWindow(x, y, w, h);
  _writeColor16(color, uint32_t(w) * uint32_t(h));
  endWrite();
}

void GFX_SPI_ILI9486::writeColor(uint16_t color, uint32_t len)
{
  _writeColor16(color, len);
}

void GFX_SPI_ILI9486::writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  writeFillRect(x, y, w, 1, color);
}

void GFX_SPI_ILI9486::writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  writeFillRect(x, y, 1, h, color);
}

void GFX_SPI_ILI9486::_writeColor16(uint16_t data, uint32_t n)
{
  if (_spi16_mode) return writeData16(data, n);
  if (0 == connection) // TFT_HARD_SPI
  {
#if (defined (ESP8266) || defined(ESP32))
    uint8_t rgb888[] = {uint8_t((data & 0xF800) >> 8), uint8_t((data & 0x07E0) >> 3), uint8_t((data & 0x001F) << 3)};
    SPI.writePattern(rgb888, 3, n);
#else // wdt on ESP8266
    while (n-- > 0)
    {
      spiWrite(uint8_t((data & 0xF800) >> 8));
      spiWrite(uint8_t((data & 0x07E0) >> 3));
      spiWrite(uint8_t((data & 0x001F) << 3));
    }
#endif
  }
  else
  {
    while (n-- > 0)
    {
      spiWrite(uint8_t((data & 0xF800) >> 8));
      spiWrite(uint8_t((data & 0x07E0) >> 3));
      spiWrite(uint8_t((data & 0x001F) << 3));
    }
  }
}

#if (defined (ESP8266) || defined(ESP32))
#define SPI_WRITE_BYTES(data, n) SPI.transferBytes(data, 0, n)
#elif defined(ARDUINO_ARCH_SAM)
#define SPI_WRITE_BYTES(data, n) SPI.transfer(SS, data, n)
#elif (TEENSYDUINO == 147)
#define SPI_WRITE_BYTES(data, n) SPI.transfer(data, 0, n)
#elif defined(ARDUINO_ARCH_STM32F1) || defined(ARDUINO_ARCH_STM32F4)
#define SPI_WRITE_BYTES(data, n) SPI.write(data, n)
#else
// valid for all other platforms? else comment out next line
#define SPI_WRITE_BYTES(data, n) SPI.transfer(data, n)
#endif


void GFX_SPI_ILI9486::_writeColor16(const uint16_t* data, uint32_t n)
{
  if (_spi16_mode) return writeData16(data, n);
  if (0 == connection) // TFT_HARD_SPI
  {
#if defined(SPI_WRITE_BYTES)
    static const uint16_t rgb888_buffer_size = 60; // 64 optimal for ESP8266 SPI
    static const uint32_t max_chunk = rgb888_buffer_size / 3; // rgb888
    uint8_t rgb888_buffer[rgb888_buffer_size];
    while (n > 0)
    {
      uint32_t chunk = min(max_chunk, n);
      n -= chunk;
      uint8_t* p = rgb888_buffer;
      uint16_t ncopy = chunk;
      while (ncopy-- > 0)
      {
        *p++ = uint8_t((*data & 0xF800) >> 8);
        *p++ = uint8_t((*data & 0x07E0) >> 3);
        *p++ = uint8_t((*data & 0x001F) << 3);
        data++;
      }
      SPI_WRITE_BYTES(rgb888_buffer, 3 * chunk);
    }
#else
    while (n-- > 0)
    {
      spiWrite(uint8_t((*data & 0xF800) >> 8));
      spiWrite(uint8_t((*data & 0x07E0) >> 3));
      spiWrite(uint8_t((*data & 0x001F) << 3));
    }
#endif
  }
  else
  {
    while (n-- > 0)
    {
      spiWrite(uint8_t((*data & 0xF800) >> 8));
      spiWrite(uint8_t((*data & 0x07E0) >> 3));
      spiWrite(uint8_t((*data & 0x001F) << 3));
    }
  }
}
