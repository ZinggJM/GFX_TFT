// created by Jean-Marc Zingg to be the GFX_SPI_RA8875 class for the GFX_TFT library
// code extracts taken from https://github.com/adafruit/Adafruit_RA8875
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// This controller class is to be used with SPI
// It uses SPI calls that include the RS selection prefix
//

#ifndef _GFX_SPI_RA8875_H_
#define _GFX_SPI_RA8875_H_

//#include <GFX_Extensions.h>
#include <GFX_IO.h>
#include "GFX_TFT_IO/GFX_TFT_IO.h"

class GFX_SPI_RA8875 : public GFX_TFT_IO
{
  public:
    GFX_SPI_RA8875(int16_t cs_pin, int16_t dc_pin, int16_t rst_pin = -1, int16_t bl_pin = -1);
    GFX_SPI_RA8875(int16_t cs_pin, int16_t dc_pin, int16_t mosi_pin, int16_t sclk_pin, int16_t rst_pin, int16_t bl_pin = -1);
    GFX_SPI_RA8875(SPIClass *spi, int16_t cs_pin, int16_t dc_pin, int16_t rst_pin = -1, int16_t bl_pin = -1);
    void begin(uint32_t freq);
    void init(uint32_t freq = 0);
    void setRotation(uint8_t r);
    void invertDisplay(bool i);
    void invert(bool i);
    void enableDisplay(bool enable);
    void setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    // optimization for this controller
    void writePixel(int16_t x, int16_t y, uint16_t color);
    void writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void drawPixel(int16_t x, int16_t y, uint16_t color);
    void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
    void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
    void fillScreen(uint16_t color);
    void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
    void drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    void fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
    void drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color);
    void fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color);
    void drawEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color);
    void fillEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color);
    void drawCurve(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color);
    void fillCurve(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color);
  private:
    void _clearWindowAddress(); // reset clipping by controller
    void _rotatePoint(int16_t& x, int16_t& y);
    void _rotatePoint(uint16_t& x, uint16_t& y);
    void _rotateWindow(int16_t& x0, int16_t& y0, int16_t& x1, int16_t& y1);
    void _rotateWindow(uint16_t& x0, uint16_t& y0, uint16_t& x1, uint16_t& y1);
    void _writeColor24(uint16_t color);
    void _rectHelper  (int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, bool filled);
    void _circleHelper(int16_t x0, int16_t y0, int16_t r, uint16_t color, bool filled);
    void _triangleHelper(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, bool filled);
    void _roundRectHelper(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color, bool filled);
    void _ellipseHelper(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color, bool filled);
    void _ellipseHelper(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color, bool filled);
    bool _waitPoll(uint8_t r, uint8_t f);
    void    _writeReg(uint8_t reg, uint8_t val);
    void    _writeReg16(uint8_t reg, uint16_t val);
    uint8_t _readReg(uint8_t reg);
    void    _writeData(uint8_t d);
    uint8_t _readData(void);
    void    _writeCommand(uint8_t d);
    uint8_t _readStatus(void);
  private:
    uint8_t _rotation;
    uint16_t _tft_width, _tft_height; // physical
    bool _is_clipping;
    int16_t _cs_pin, _bl_pin;
};

#define GFX_TFT_CLASS GFX_SPI_RA8875

#endif
