// created by Jean-Marc Zingg to be the GXF_TFT_IO class for the ZxTFT library
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//

#include "GXF_TFT_IO.h"

GXF_TFT_IO::GXF_TFT_IO(uint16_t w, uint16_t h, int8_t cs, int8_t dc, int8_t mosi, int8_t sck, int8_t rst, int8_t miso) :
  GFX_IO(w, h, cs, dc, mosi, sck, rst, miso)
{
}

GXF_TFT_IO::GXF_TFT_IO(uint16_t w, uint16_t h, int8_t cs, int8_t dc, int8_t rst) :
  GFX_IO(w, h, cs, dc, rst)
{
}

#if !defined(ESP8266)
GXF_TFT_IO::GXF_TFT_IO(uint16_t w, uint16_t h, SPIClass *spiClass, int8_t cs, int8_t dc, int8_t rst) :
  GFX_IO(w, h, spiClass, cs, dc, rst)
{
}
#endif // end !ESP8266

void GXF_TFT_IO::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
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

void GXF_TFT_IO::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
  fillRect(x, y, 1, h, color);
}

void GXF_TFT_IO::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
  fillRect(x, y, w, 1, color);
}

void GXF_TFT_IO::fillScreen(uint16_t color)
{
  fillRect(0, 0, WIDTH, HEIGHT, color);
}

void GXF_TFT_IO::_writeColor16(uint16_t data, uint32_t n)
{
  if (0 == connection) // TFT_HARD_SPI
  {
#if (defined (ESP8266) || defined(ESP32)) && true // fastest
    uint16_t swapped = ((data << 8) | (data >> 8));
    SPI.writePattern((uint8_t*)&swapped, 2, n);
#elif (defined (ESP8266) || defined(ESP32))
    while (n-- > 0)
    {
      SPI.write16(data);
    }
#else // wdt on ESP8266
    while (n-- > 0)
    {
      SPI_WRITE16(data);
    }
#endif
  }
  else
  {
    while (n-- > 0)
    {
      SPI_WRITE16(data);
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


void GXF_TFT_IO::_writeColor16(const uint16_t* data, uint32_t n)
{
  if (0 == connection) // TFT_HARD_SPI
  {
#if defined(SPI_WRITE_BYTES)
    static const uint16_t swap_buffer_size = 64; // optimal for ESP8266 SPI
    static const uint32_t max_chunk = swap_buffer_size / 2; // uint16_t's
    uint8_t swap_buffer[swap_buffer_size];
    const uint8_t* p1 = reinterpret_cast<const uint8_t*> (data);
    const uint8_t* p2 = p1 + 1;
    while (n > 0)
    {
      uint32_t chunk = min(max_chunk, n);
      n -= chunk;
      uint8_t* p3 = swap_buffer;
      uint8_t* p4 = p3 + 1;
      uint16_t ncopy = chunk;
      while (ncopy-- > 0)
      {
        *p3 = *p2; p3 += 2; p2 += 2;
        *p4 = *p1; p4 += 2; p1 += 2;
      }
      SPI_WRITE_BYTES(swap_buffer, 2 * chunk);
    }
#else
    while (n-- > 0)
    {
      SPI_WRITE16(*data++);
    }
#endif
  }
  else
  {
    while (n-- > 0)
    {
      SPI_WRITE16(*data++);
    }
  }
}
