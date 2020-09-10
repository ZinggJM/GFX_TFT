// created by Jean-Marc Zingg to be the GFX_SPI_RA8875 class for the GFX_TFT library
// code extracts taken from https://github.com/adafruit/Adafruit_RA8875
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// This controller class is to be used with SPI
// It uses SPI calls that include the RS selection prefix
//

#include "GFX_SPI_RA8875.h"

#define SPI_SPEED 16000000

// Command/Data pins for SPI
#define RA8875_DATAWRITE        0x00
#define RA8875_DATAREAD         0x40
#define RA8875_CMDWRITE         0x80
#define RA8875_CMDREAD          0xC0

// Registers & bits
#define RA8875_PWRR             0x01
#define RA8875_PWRR_DISPON      0x80
#define RA8875_PWRR_DISPOFF     0x00
#define RA8875_PWRR_SLEEP       0x02
#define RA8875_PWRR_NORMAL      0x00
#define RA8875_PWRR_SOFTRESET   0x01

#define RA8875_MRWC             0x02

#define RA8875_GPIOX            0xC7

#define RA8875_PLLC1            0x88
#define RA8875_PLLC1_PLLDIV2    0x80
#define RA8875_PLLC1_PLLDIV1    0x00

#define RA8875_PLLC2            0x89
#define RA8875_PLLC2_DIV1       0x00
#define RA8875_PLLC2_DIV2       0x01
#define RA8875_PLLC2_DIV4       0x02
#define RA8875_PLLC2_DIV8       0x03
#define RA8875_PLLC2_DIV16      0x04
#define RA8875_PLLC2_DIV32      0x05
#define RA8875_PLLC2_DIV64      0x06
#define RA8875_PLLC2_DIV128     0x07

#define RA8875_SYSR             0x10
#define RA8875_SYSR_8BPP        0x00
#define RA8875_SYSR_16BPP       0x0C
#define RA8875_SYSR_MCU8        0x00
#define RA8875_SYSR_MCU16       0x03

#define RA8875_PCSR             0x04
#define RA8875_PCSR_PDATR       0x00
#define RA8875_PCSR_PDATL       0x80
#define RA8875_PCSR_CLK         0x00
#define RA8875_PCSR_2CLK        0x01
#define RA8875_PCSR_4CLK        0x02
#define RA8875_PCSR_8CLK        0x03

#define RA8875_HDWR             0x14

#define RA8875_HNDFTR           0x15
#define RA8875_HNDFTR_DE_HIGH   0x00
#define RA8875_HNDFTR_DE_LOW    0x80

#define RA8875_HNDR             0x16
#define RA8875_HSTR             0x17
#define RA8875_HPWR             0x18
#define RA8875_HPWR_LOW         0x00
#define RA8875_HPWR_HIGH        0x80

#define RA8875_VDHR0            0x19
#define RA8875_VDHR1            0x1A
#define RA8875_VNDR0            0x1B
#define RA8875_VNDR1            0x1C
#define RA8875_VSTR0            0x1D
#define RA8875_VSTR1            0x1E
#define RA8875_VPWR             0x1F
#define RA8875_VPWR_LOW         0x00
#define RA8875_VPWR_HIGH        0x80

#define RA8875_DPCR             0x20 // Display Configuration Register
#define RA8875_FNCR1            0x22 //Font Control Register 1

#define RA8875_HSAW0            0x30
#define RA8875_HSAW1            0x31
#define RA8875_VSAW0            0x32
#define RA8875_VSAW1            0x33

#define RA8875_HEAW0            0x34
#define RA8875_HEAW1            0x35
#define RA8875_VEAW0            0x36
#define RA8875_VEAW1            0x37

#define RA8875_MCLR             0x8E
#define RA8875_MCLR_START       0x80
#define RA8875_MCLR_STOP        0x00
#define RA8875_MCLR_READSTATUS  0x80
#define RA8875_MCLR_FULL        0x00
#define RA8875_MCLR_ACTIVE      0x40

#define RA8875_DCR                    0x90
#define RA8875_DCR_LINESQUTRI_START   0x80
#define RA8875_DCR_LINESQUTRI_STOP    0x00
#define RA8875_DCR_LINESQUTRI_STATUS  0x80
#define RA8875_DCR_CIRCLE_START       0x40
#define RA8875_DCR_CIRCLE_STATUS      0x40
#define RA8875_DCR_CIRCLE_STOP        0x00
#define RA8875_DCR_FILL               0x20
#define RA8875_DCR_NOFILL             0x00
#define RA8875_DCR_DRAWLINE           0x00
#define RA8875_DCR_DRAWTRIANGLE       0x01
#define RA8875_DCR_DRAWSQUARE         0x10


#define RA8875_ELLIPSE                0xA0
#define RA8875_ELLIPSE_STATUS         0x80

#define RA8875_MWCR0            0x40
#define RA8875_MWCR0_GFXMODE    0x00
#define RA8875_MWCR0_TXTMODE    0x80

#define RA8875_CURH0            0x46
#define RA8875_CURH1            0x47
#define RA8875_CURV0            0x48
#define RA8875_CURV1            0x49

#define RA8875_MRCD              0x45
#define RA8875_RCURH0            0x4A
#define RA8875_RCURH1            0x4B
#define RA8875_RCURV0            0x4C
#define RA8875_RCURV1            0x4D

#define RA8875_P1CR             0x8A
#define RA8875_P1CR_ENABLE      0x80
#define RA8875_P1CR_DISABLE     0x00
#define RA8875_P1CR_CLKOUT      0x10
#define RA8875_P1CR_PWMOUT      0x00

#define RA8875_P1DCR            0x8B

#define RA8875_P2CR             0x8C
#define RA8875_P2CR_ENABLE      0x80
#define RA8875_P2CR_DISABLE     0x00
#define RA8875_P2CR_CLKOUT      0x10
#define RA8875_P2CR_PWMOUT      0x00

#define RA8875_P2DCR            0x8D

#define RA8875_PWM_CLK_DIV1     0x00
#define RA8875_PWM_CLK_DIV2     0x01
#define RA8875_PWM_CLK_DIV4     0x02
#define RA8875_PWM_CLK_DIV8     0x03
#define RA8875_PWM_CLK_DIV16    0x04
#define RA8875_PWM_CLK_DIV32    0x05
#define RA8875_PWM_CLK_DIV64    0x06
#define RA8875_PWM_CLK_DIV128   0x07
#define RA8875_PWM_CLK_DIV256   0x08
#define RA8875_PWM_CLK_DIV512   0x09
#define RA8875_PWM_CLK_DIV1024  0x0A
#define RA8875_PWM_CLK_DIV2048  0x0B
#define RA8875_PWM_CLK_DIV4096  0x0C
#define RA8875_PWM_CLK_DIV8192  0x0D
#define RA8875_PWM_CLK_DIV16384 0x0E
#define RA8875_PWM_CLK_DIV32768 0x0F

#define RA8875_TPCR0                  0x70
#define RA8875_TPCR0_ENABLE           0x80
#define RA8875_TPCR0_DISABLE          0x00
#define RA8875_TPCR0_WAIT_512CLK      0x00
#define RA8875_TPCR0_WAIT_1024CLK     0x10
#define RA8875_TPCR0_WAIT_2048CLK     0x20
#define RA8875_TPCR0_WAIT_4096CLK     0x30
#define RA8875_TPCR0_WAIT_8192CLK     0x40
#define RA8875_TPCR0_WAIT_16384CLK    0x50
#define RA8875_TPCR0_WAIT_32768CLK    0x60
#define RA8875_TPCR0_WAIT_65536CLK    0x70
#define RA8875_TPCR0_WAKEENABLE       0x08
#define RA8875_TPCR0_WAKEDISABLE      0x00
#define RA8875_TPCR0_ADCCLK_DIV1      0x00
#define RA8875_TPCR0_ADCCLK_DIV2      0x01
#define RA8875_TPCR0_ADCCLK_DIV4      0x02
#define RA8875_TPCR0_ADCCLK_DIV8      0x03
#define RA8875_TPCR0_ADCCLK_DIV16     0x04
#define RA8875_TPCR0_ADCCLK_DIV32     0x05
#define RA8875_TPCR0_ADCCLK_DIV64     0x06
#define RA8875_TPCR0_ADCCLK_DIV128    0x07

#define RA8875_TPCR1            0x71
#define RA8875_TPCR1_AUTO       0x00
#define RA8875_TPCR1_MANUAL     0x40
#define RA8875_TPCR1_VREFINT    0x00
#define RA8875_TPCR1_VREFEXT    0x20
#define RA8875_TPCR1_DEBOUNCE   0x04
#define RA8875_TPCR1_NODEBOUNCE 0x00
#define RA8875_TPCR1_IDLE       0x00
#define RA8875_TPCR1_WAIT       0x01
#define RA8875_TPCR1_LATCHX     0x02
#define RA8875_TPCR1_LATCHY     0x03

#define RA8875_TPXH             0x72
#define RA8875_TPYH             0x73
#define RA8875_TPXYL            0x74

#define RA8875_INTC1            0xF0
#define RA8875_INTC1_KEY        0x10
#define RA8875_INTC1_DMA        0x08
#define RA8875_INTC1_TP         0x04
#define RA8875_INTC1_BTE        0x02

#define RA8875_INTC2            0xF1
#define RA8875_INTC2_KEY        0x10
#define RA8875_INTC2_DMA        0x08
#define RA8875_INTC2_TP         0x04
#define RA8875_INTC2_BTE        0x02

GFX_SPI_RA8875::GFX_SPI_RA8875(int8_t cs_pin, int8_t dc_pin, int8_t rst_pin, int8_t bl_pin) :
#if defined(ESP8266)
  GFX_TFT_IO(480, 800, cs_pin, dc_pin, rst_pin)
#else
  GFX_TFT_IO(480, 800, &SPI, cs_pin, dc_pin, rst_pin)
#endif
{
  _cs_pin = cs_pin;
  _bl_pin = bl_pin;
}

GFX_SPI_RA8875::GFX_SPI_RA8875(int8_t cs_pin, int8_t dc_pin, int8_t mosi_pin, int8_t sclk_pin, int8_t rst_pin, int8_t bl_pin) :
  GFX_TFT_IO(480, 800, cs_pin, dc_pin, mosi_pin, sclk_pin, rst_pin, -1)
{
  _cs_pin = cs_pin;
  _bl_pin = bl_pin;
}

GFX_SPI_RA8875::GFX_SPI_RA8875(SPIClass *spi, int8_t cs_pin, int8_t dc_pin, int8_t rst_pin, int8_t bl_pin) :
#if defined(ESP8266)
  GFX_TFT_IO(480, 800, cs_pin, dc_pin, rst_pin)
#else
  GFX_TFT_IO(480, 800, spi, cs_pin, dc_pin, rst_pin)
#endif
{
  (void) spi;
  _cs_pin = cs_pin;
  _bl_pin = bl_pin;
}

void GFX_SPI_RA8875::begin(uint32_t freq)
{
  init(freq);
}

void GFX_SPI_RA8875::init(uint32_t freq)
{
  if (!freq) freq = SPI_SPEED;
  initSPI(freq);
  _tft_width = 800; // physical
  _tft_height = 480; // physical
  _rotation = 0;
  setSPISpeed(4000000); // needs lower speed for setup
  startWrite();
  _writeReg(RA8875_P1CR, 0x40);
  _writeReg(RA8875_PLLC1, RA8875_PLLC1_PLLDIV1 + 10);
  delay(1);
  _writeReg(RA8875_PLLC2, RA8875_PLLC2_DIV4);
  delay(1);
  _writeReg(RA8875_SYSR, RA8875_SYSR_16BPP | RA8875_SYSR_MCU8);

  /* Timing values */
  uint8_t pixclk;
  uint8_t hsync_start;
  uint8_t hsync_pw;
  uint8_t hsync_finetune;
  uint8_t hsync_nondisp;
  uint8_t vsync_pw;
  uint16_t vsync_nondisp;
  uint16_t vsync_start;

  /* Set the correct values for the display being used */
  pixclk          = RA8875_PCSR_PDATL | RA8875_PCSR_2CLK;
  hsync_nondisp   = 26;
  hsync_start     = 32;
  hsync_pw        = 96;
  hsync_finetune  = 0;
  vsync_nondisp   = 32;
  vsync_start     = 23;
  vsync_pw        = 2;

  _writeReg(RA8875_PCSR, pixclk);
  delay(1);

  /* Horizontal settings registers */
  _writeReg(RA8875_HDWR, (_tft_width / 8) - 1);                          // H width: (HDWR + 1) * 8 = 480
  _writeReg(RA8875_HNDFTR, RA8875_HNDFTR_DE_HIGH + hsync_finetune);
  _writeReg(RA8875_HNDR, (hsync_nondisp - hsync_finetune - 2) / 8);  // H non-display: HNDR * 8 + HNDFTR + 2 = 10
  _writeReg(RA8875_HSTR, hsync_start / 8 - 1);                       // Hsync start: (HSTR + 1)*8
  _writeReg(RA8875_HPWR, RA8875_HPWR_LOW + (hsync_pw / 8 - 1));      // HSync pulse width = (HPWR+1) * 8

  /* Vertical settings registers */
  _writeReg(RA8875_VDHR0, (uint16_t)(_tft_height - 1) & 0xFF);
  _writeReg(RA8875_VDHR1, (uint16_t)(_tft_height - 1) >> 8);
  _writeReg(RA8875_VNDR0, vsync_nondisp - 1);                        // V non-display period = VNDR + 1
  _writeReg(RA8875_VNDR1, vsync_nondisp >> 8);
  _writeReg(RA8875_VSTR0, vsync_start - 1);                          // Vsync start position = VSTR + 1
  _writeReg(RA8875_VSTR1, vsync_start >> 8);
  _writeReg(RA8875_VPWR, RA8875_VPWR_LOW + vsync_pw - 1);            // Vsync pulse width = VPWR + 1

  /* Set active window X */
  _writeReg(RA8875_HSAW0, 0);                                        // horizontal start point
  _writeReg(RA8875_HSAW1, 0);
  _writeReg(RA8875_HEAW0, (uint16_t)(_tft_width - 1) & 0xFF);            // horizontal end point
  _writeReg(RA8875_HEAW1, (uint16_t)(_tft_width - 1) >> 8);

  /* Set active window Y */
  _writeReg(RA8875_VSAW0, 0);                                        // vertical start point
  _writeReg(RA8875_VSAW1, 0);
  _writeReg(RA8875_VEAW0, (uint16_t)(_tft_height - 1) & 0xFF);           // horizontal end point
  _writeReg(RA8875_VEAW1, (uint16_t)(_tft_height - 1) >> 8);

  /* ToDo: Setup touch panel? */

  /* Clear the entire window */
  _writeReg(RA8875_MCLR, RA8875_MCLR_START | RA8875_MCLR_FULL);
  delay(500);
  _writeReg(RA8875_PWRR, RA8875_PWRR_NORMAL | RA8875_PWRR_DISPON);
  _writeReg(RA8875_GPIOX, 1);
  _writeReg(RA8875_P1CR, RA8875_P1CR_ENABLE | (RA8875_PWM_CLK_DIV1024 & 0xF));
  _writeReg(RA8875_P1DCR, 255);
  endWrite();
  setSPISpeed(SPI_SPEED);
}

void GFX_SPI_RA8875::setRotation(uint8_t r)
{
  GFX_Root::setRotation(r);
  _rotation = r;
  return;
  // did not work, or don't know how to
  switch (r & 3)
  {
    case 0:
      _writeReg(RA8875_MWCR0, 0x0C);
      //_writeReg(RA8875_DPCR, 0);
      break;
    case 1:
      _writeReg(RA8875_MWCR0, 0);
      //_writeReg(RA8875_DPCR, 4);
      break;
    case 2:
      _writeReg(RA8875_MWCR0, 0);
      //_writeReg(RA8875_DPCR, 6);
      break;
    case 3:
      _writeReg(RA8875_MWCR0, 0);
      //_writeReg(RA8875_DPCR, 8);
      break;
  }
  return;
}

void GFX_SPI_RA8875::invertDisplay(bool i)
{
}


void GFX_SPI_RA8875::invert(bool i)
{
  invertDisplay(i);
}

void GFX_SPI_RA8875::enableDisplay(bool enable)
{
  if (_bl_pin >= 0) digitalWrite(_bl_pin, enable ? HIGH : LOW);
  enableBacklight(enable); // for _plugin
}

void GFX_SPI_RA8875::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t xe = x + w - 1;
  uint16_t ye = y + h - 1;
  _is_clipping = (x != 0) || (y != 0) || ( xe != _tft_width - 1) || (ye != _tft_height - 1);
  _rotateWindow(x, y, xe, ye);
  _writeReg16(RA8875_HSAW0, x); // horizontal start point
  _writeReg16(RA8875_HEAW0, xe); // horizontal end point
  _writeReg16(RA8875_VSAW0, y); // vertical start point
  _writeReg16(RA8875_VEAW0, ye); // vertical end point
  _writeReg16(RA8875_CURH0, x); // horizontal input cursor
  _writeReg16(RA8875_CURV0, y); // vertical input cursor
  if (_cs_pin >= 0) digitalWrite(_cs_pin, LOW);
  writeData(RA8875_CMDWRITE);
  writeData(RA8875_MRWC);
  writeData(RA8875_DATAWRITE);
}

void GFX_SPI_RA8875::_clearWindowAddress()
{
  _is_clipping = false;
  _writeReg16(RA8875_HSAW0, 0); // horizontal start point
  _writeReg16(RA8875_HEAW0, _tft_width - 1); // horizontal end point
  _writeReg16(RA8875_VSAW0, 0); // vertical start point
  _writeReg16(RA8875_VEAW0, _tft_height - 1); // vertical end point
  _writeReg16(RA8875_CURH0, 0); // horizontal input cursor
  _writeReg16(RA8875_CURV0, 0); // vertical input cursor
}


void GFX_SPI_RA8875::_rotatePoint(int16_t& x, int16_t& y)
{
  int16_t x0 = x;
  switch (_rotation)
  {
    case 0:
      x = y;
      y = _tft_height - x0 - 1;
      break;
    case 1: break;
    case 2:
      x = _tft_width - y - 1;
      y = x0;
      break;
    case 3:
      x = _tft_width - x - 1;
      y = _tft_height - y - 1;
      break;
  }
}

void GFX_SPI_RA8875::_rotatePoint(uint16_t& x, uint16_t& y)
{
  uint16_t x0 = x;
  switch (_rotation)
  {
    case 0:
      x = y;
      y = _tft_height - x0 - 1;
      break;
    case 1: break;
    case 2:
      x = _tft_width - y - 1;
      y = x0;
      break;
    case 3:
      x = _tft_width - x - 1;
      y = _tft_height - y - 1;
      break;
  }
}

void GFX_SPI_RA8875::_rotateWindow(int16_t& x0, int16_t& y0, int16_t& x1, int16_t& y1)
{
  switch (_rotation)
  {
    case 0:
      {
        int16_t tx0 = x0;
        int16_t tx1 = x1;
        x0 = y0;
        x1 = y1;
        y0 = _tft_height - tx1 - 1;
        y1 = _tft_height - tx0 - 1;
      }
      break;
    case 1:
      break;
    case 2:
      {
        int16_t tx0 = x0;
        int16_t tx1 = x1;
        x0 = _tft_width - y1 - 1;
        x1 = _tft_width - y0 - 1;
        y0 = tx0;
        y1 = tx1;
      }
      break;
    case 3:
      {
        int16_t tx0 = x0;
        int16_t ty0 = y0;
        x0 = _tft_width - x1 - 1;
        x1 = _tft_width - tx0 - 1;
        y0 = _tft_height - y1 - 1;
        y1 = _tft_height - ty0 - 1;
      }
      break;
  }
}

void GFX_SPI_RA8875::_rotateWindow(uint16_t& x0, uint16_t& y0, uint16_t& x1, uint16_t& y1)
{
  switch (_rotation)
  {
    case 0:
      {
        uint16_t tx0 = x0;
        uint16_t tx1 = x1;
        x0 = y0;
        x1 = y1;
        y0 = _tft_height - tx1 - 1;
        y1 = _tft_height - tx0 - 1;
      }
      break;
    case 1:
      break;
    case 2:
      {
        uint16_t tx0 = x0;
        uint16_t tx1 = x1;
        x0 = _tft_width - y1 - 1;
        x1 = _tft_width - y0 - 1;
        y0 = tx0;
        y1 = tx1;
      }
      break;
    case 3:
      {
        uint16_t tx0 = x0;
        uint16_t ty0 = y0;
        x0 = _tft_width - x1 - 1;
        x1 = _tft_width - tx0 - 1;
        y0 = _tft_height - y1 - 1;
        y1 = _tft_height - ty0 - 1;
      }
      break;
  }
}

void GFX_SPI_RA8875::_writeColor24(uint16_t color)
{
  _writeReg(0x63, (color & 0xf800) >> 11);
  _writeReg(0x64, (color & 0x07e0) >> 5);
  _writeReg(0x65, (color & 0x001f));
}

void GFX_SPI_RA8875::writePixel(int16_t x, int16_t y, uint16_t color)
{
  // Clip first...
  if ((x >= 0) && (x < _width) && (y >= 0) && (y < _height))
  {
    _rotatePoint(x, y);
    _writeReg16(RA8875_CURH0, x);
    _writeReg16(RA8875_CURV0, y);
    _writeCommand(RA8875_MRWC);
    if (_cs_pin >= 0) digitalWrite(_cs_pin, LOW);
    writeData(RA8875_DATAWRITE);
    writeData(color >> 8);
    writeData(color);
    if (_cs_pin >= 0) digitalWrite(_cs_pin, HIGH);
  }
}

void GFX_SPI_RA8875::writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  // a correct clipping is the goal. try to achieve this
  if (x < 0) w += x, x = 0;
  if (y < 0) h += y, y = 0;
  if (x + w > _width) w = _width - x;
  if (y + h > _height) h = _height - y;
  if ((w < 1) || (h < 1)) return;
  if (_cs_pin >= 0) digitalWrite(_cs_pin, LOW);
  setAddrWindow(x, y, w, h);
  writeData16(color, uint32_t(w) * uint32_t(h));
  if (_cs_pin >= 0) digitalWrite(_cs_pin, HIGH);
}

void GFX_SPI_RA8875::drawPixel(int16_t x, int16_t y, uint16_t color)
{
  startWrite();
  _rotatePoint(x, y);
  _writeReg16(RA8875_CURH0, x);
  _writeReg16(RA8875_CURV0, y);
  _writeCommand(RA8875_MRWC);
  if (_cs_pin >= 0) digitalWrite(_cs_pin, LOW);
  writeData(RA8875_DATAWRITE);
  writeData(color >> 8);
  writeData(color);
  endWrite();
}

void GFX_SPI_RA8875::drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
  startWrite();
  if (_is_clipping) _clearWindowAddress();
  _rotatePoint(x0, y0);
  _rotatePoint(x1, y1);
  _writeReg16(0x91, x0); // Xs
  _writeReg16(0x93, y0); // Ys
  _writeReg16(0x95, x1); // Xe
  _writeReg16(0x97, y1); // Ye
  _writeColor24(color);
  _writeReg(RA8875_DCR, 0x80); // draw
  /* Wait for the command to finish */
  _waitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS);
  endWrite();
}

void GFX_SPI_RA8875::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  _rectHelper(x, y, w, h, color, false);
}

void GFX_SPI_RA8875::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
  _rectHelper(x, y, w, h, color, true);
}

void GFX_SPI_RA8875::fillScreen(uint16_t color)
{
  fillRect(0, 0, _width, _height, color); // uses _rotation!
}

void GFX_SPI_RA8875::drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  _circleHelper(x0, y0, r, color, false);
}

void GFX_SPI_RA8875::fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
  _circleHelper(x0, y0, r, color, true);
}

void GFX_SPI_RA8875::drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
  _triangleHelper(x0, y0, x1, y1, x2, y2, color, false);
}

void GFX_SPI_RA8875::fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color)
{
  _triangleHelper(x0, y0, x1, y1, x2, y2, color, true);
}

void GFX_SPI_RA8875::drawRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color)
{
  _roundRectHelper(x, y, w, h, radius, color, false);
}

void GFX_SPI_RA8875::fillRoundRect(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color)
{
  _roundRectHelper(x, y, w, h, radius, color, false);
}

void GFX_SPI_RA8875::drawEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color)
{
  _ellipseHelper(xCenter, yCenter, longAxis, shortAxis, color, false);
}

void GFX_SPI_RA8875::fillEllipse(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color)
{
  _ellipseHelper(xCenter, yCenter, longAxis, shortAxis, color, true);
}

void GFX_SPI_RA8875::drawCurve(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color)
{
  _ellipseHelper(xCenter, yCenter, longAxis, shortAxis, curvePart, color, false);
}

void GFX_SPI_RA8875::fillCurve(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color)
{
  _ellipseHelper(xCenter, yCenter, longAxis, shortAxis, curvePart, color, true);
}

void GFX_SPI_RA8875::_rectHelper(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color, bool filled)
{
  int16_t xe = x + w - 1;
  int16_t ye = y + h - 1;
  startWrite();
  if (_is_clipping) _clearWindowAddress();
  //Serial.print("_rectHelper("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.print(w); Serial.print(", "); Serial.print(h); Serial.println(")");
  _rotateWindow(x, y, xe, ye);
  //Serial.print("_rectHelper("); Serial.print(x); Serial.print(", "); Serial.print(y); Serial.print(", "); Serial.print(xe); Serial.print(", "); Serial.print(ye); Serial.print(") "); Serial.println(_rotation);
  _writeReg16(0x91, x); // Xs
  _writeReg16(0x93, y); // Ys
  _writeReg16(0x95, xe); // Xe
  _writeReg16(0x97, ye); // Ye
  _writeColor24(color);
  _writeReg(RA8875_DCR, filled ? 0xB0 : 0x90); // draw
  /* Wait for the command to finish */
  _waitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS);
  endWrite();
}

void GFX_SPI_RA8875::_circleHelper(int16_t x0, int16_t y0, int16_t r, uint16_t color, bool filled)
{
  startWrite();
  if (_is_clipping) _clearWindowAddress();
  _rotatePoint(x0, y0);
  _writeReg16(0x99, x0);
  _writeReg16(0x9b, y0);
  _writeReg(0x9d, r);
  _writeColor24(color);
  _writeReg(RA8875_DCR, filled ? (RA8875_DCR_CIRCLE_START | RA8875_DCR_FILL) : (RA8875_DCR_CIRCLE_START | RA8875_DCR_NOFILL)); // draw
  endWrite();
  /* Wait for the command to finish */
  setSPISpeed(4000000);  // slow down for read, SUMOTOY's trick
  startWrite();
  _waitPoll(RA8875_DCR, RA8875_DCR_CIRCLE_STATUS);
  endWrite();
  setSPISpeed(16000000);
}

void GFX_SPI_RA8875::_triangleHelper(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, bool filled)
{
  startWrite();
  if (_is_clipping) _clearWindowAddress();
  _rotatePoint(x0, y0);
  _rotatePoint(x1, y1);
  _rotatePoint(x2, y2);
  _writeReg16(0x91, x0); // X0
  _writeReg16(0x93, y0); // Y0
  _writeReg16(0x95, x1); // X1
  _writeReg16(0x97, y1); // Y1
  _writeReg16(0xA9, x2); // X2
  _writeReg16(0xAB, y2); // Y2
  _writeColor24(color);
  _writeReg(RA8875_DCR, filled ? 0xA1 : 0x81); // draw
  /* Wait for the command to finish */
  _waitPoll(RA8875_DCR, RA8875_DCR_LINESQUTRI_STATUS);
  endWrite();
}

void GFX_SPI_RA8875::_roundRectHelper(int16_t x, int16_t y, int16_t w, int16_t h, int16_t radius, uint16_t color, bool filled)
{
  startWrite();
  if (_is_clipping) _clearWindowAddress();
  int16_t xe = x + w - 1;
  int16_t ye = y + h - 1;
  _rotateWindow(x, y, xe, ye);
  _writeReg16(0x91, x); // Xs
  _writeReg16(0x93, y); // Ys
  _writeReg16(0x95, xe); // Xe
  _writeReg16(0x97, ye); // Ye
  _writeReg16(0xA1, radius);
  _writeReg16(0xA3, radius);
  _writeColor24(color);
  _writeReg(RA8875_ELLIPSE, filled ? 0xE0 : 0xA0); // draw
  /* Wait for the command to finish */
  _waitPoll(RA8875_ELLIPSE, RA8875_DCR_LINESQUTRI_STATUS);
  endWrite();
}

void GFX_SPI_RA8875::_ellipseHelper(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint16_t color, bool filled)
{
  startWrite();
  if (_is_clipping) _clearWindowAddress();
  _rotatePoint(xCenter, yCenter);
  _writeReg16(0xA5, xCenter);
  _writeReg16(0xA7, yCenter);
  _writeReg16(0xA1, longAxis);
  _writeReg16(0xA3, shortAxis);
  _writeColor24(color);
  _writeReg(RA8875_ELLIPSE, filled ? 0xC0 : 0x80); // draw
  /* Wait for the command to finish */
  _waitPoll(RA8875_ELLIPSE, RA8875_ELLIPSE_STATUS);
  endWrite();
}

void GFX_SPI_RA8875::_ellipseHelper(int16_t xCenter, int16_t yCenter, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color, bool filled)
{
  startWrite();
  if (_is_clipping) _clearWindowAddress();
  _rotatePoint(xCenter, yCenter);
  _writeReg16(0xA5, xCenter);
  _writeReg16(0xA7, yCenter);
  _writeReg16(0xA1, longAxis);
  _writeReg16(0xA3, shortAxis);
  _writeColor24(color);
  _writeReg(RA8875_ELLIPSE, filled ? (0xD0 | (curvePart & 0x03)) : (0x90 | (curvePart & 0x03))); // draw
  /* Wait for the command to finish */
  _waitPoll(RA8875_ELLIPSE, RA8875_ELLIPSE_STATUS);
  endWrite();
}

bool GFX_SPI_RA8875::_waitPoll(uint8_t regname, uint8_t waitflag)
{
  /* Wait for the command to finish */
  unsigned long timeout = millis();
  while (1)
  {
    uint8_t temp = _readReg(regname);
    if (!(temp & waitflag)) return true;
    if ((millis() - timeout) > 20) return false;
  }
  return false;
}

void GFX_SPI_RA8875::_writeReg(uint8_t reg, uint8_t val)
{
  _writeCommand(reg);
  _writeData(val);
}

void GFX_SPI_RA8875::_writeReg16(uint8_t reg, uint16_t val)
{
  _writeCommand(reg);
  _writeData(val);
  _writeCommand(reg + 1);
  _writeData(val >> 8);
}

uint8_t GFX_SPI_RA8875::_readReg(uint8_t reg)
{
  _writeCommand(reg);
  return _readData();
}

void GFX_SPI_RA8875::_writeData(uint8_t d)
{
  if (_cs_pin >= 0) digitalWrite(_cs_pin, LOW);
  writeData(RA8875_DATAWRITE);
  writeData(d);
  if (_cs_pin >= 0) digitalWrite(_cs_pin, HIGH);
}

uint8_t  GFX_SPI_RA8875::_readData(void)
{
  if (_cs_pin >= 0) digitalWrite(_cs_pin, LOW);
  writeData(RA8875_DATAREAD);
  uint8_t x = spiRead();
  if (_cs_pin >= 0) digitalWrite(_cs_pin, HIGH);
  return x;
}

void  GFX_SPI_RA8875::_writeCommand(uint8_t d)
{
  if (_cs_pin >= 0) digitalWrite(_cs_pin, LOW);
  writeData(RA8875_CMDWRITE);
  writeData(d);
  if (_cs_pin >= 0) digitalWrite(_cs_pin, HIGH);
}

uint8_t  GFX_SPI_RA8875::_readStatus(void)
{
  if (_cs_pin >= 0) digitalWrite(_cs_pin, LOW);
  writeData(RA8875_CMDREAD);
  uint8_t x = spiRead();
  if (_cs_pin >= 0) digitalWrite(_cs_pin, HIGH);
  return x;
}
