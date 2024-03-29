// created by Jean-Marc Zingg to be the GFX_STM32F407VxT6 io class for the GxTFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is the io class for STM32F407VxT6 board with FMSC TFT connector, e.g. for https://www.aliexpress.com/item/1005001806399987.html
// details available here: http://www.lcdwiki.com/STM32F407VxT6_Board
// this is a wiring for the 34pin female TFT connector of this STM32F407VxT6 board
//
// and for several direct matching displays
// e.g. https://www.aliexpress.com/item/Smart-Electronics-3-5-inch-TFT-Touch-Screen-LCD-Module-Display-320-480-ILI9486-with-PCB/32586941686.html
// e.g. "maithoga IPS 3.97 inch 34P 16.7M HD TFT LCD Screen with Adapter Board (Touch/No Touch) NT35510 Drive IC 480(RGB)*800" https://www.aliexpress.com/item/32676929794.html
//
// for pin information see the backside of the TFT, for the data pin to port pin mapping see FSMC pin table STM32F407V doc.
//
// this io class can be used with or adapted to other STM32F407V/Z processor boards with FSMC TFT connector.
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), board "Generic STM32F4 series" part "Generic F407VE" or "Generic F407VG".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#if defined(ARDUINO_ARCH_STM32) && defined(STM32F407xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_GENERIC_F407VETX) || defined(ARDUINO_GENERIC_F407VGTX)

#include "GFX_STM32F407VxT6.h"

// TFT connector uses FSMC pins
// D0   D1   D2  D3  D4  D5  D6  D7   D8   D9   D10  D11  D12  D13 D14 D15
// PD14 PD15 PD0 PD1 PE7 PE8 PE9 PE10 PE11 PE12 PE13 PE14 PE15 PD8 PD9 PD10

// connector pins
// 01  02  03  04  05  06  07  08  09  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29   30   31   32  33  34
// CS  RS  WR  RD  RST D0  D1  D2  D3  D4  D5  D6  D7  D8  D9  D10 D11 D12 D13 D14 D15 NC  LIG 3.3 3.3 GND GND NC  MISO MOSI TINT NC  TCS SCK
// D7  D11 D5  D4  RST D14 D15 D0  D1  E7  E8  E9  E10 E11 E12 E13 E14 E15 D8  D9  D10 GND B15 3.3 3.3 GND GND 5V  B2   C4   B1   NC  C13 B0

// D used : 15,14,..,..,11,10, 9, 8, 7,.., 5, 4,..,.., 1, 0, // 11
// D data : 15,14,..,..,..,10, 9, 8,..,..,..,..,..,.., 1, 0, // 7
//         |           |           |           |           |
// E used : 15,14,13,12,11,10, 9, 8, 7,..,..,..,..,..,..,.., // 9
// E data : 15,14,13,12,11,10, 9, 8, 7,..,..,..,..,..,..,.., // 9

#define PD_USED_BITS 0xCFB3
#define PD_DATA_BITS 0xC703

#define PE_USED_BITS 0xFF80
#define PE_DATA_BITS 0xFF80

// PD_MODE_MASK      0q3300333330330033 // quaternary notation, 2bit per digit
// PD_MODE_DATA      0q3300033300000033 // quaternary notation, 2bit per digit
#define PD_MODE_MASK 0xF0FFCF0F // all used bits
#define PD_MODE_BITS 0x50558505 // 01 : general purpose output mode
#define PE_MODE_MASK 0xFFFFC000 // all used bits
#define PE_MODE_BITS 0x55554000 // 01 : general purpose output mode

#define PD_MODE_DATA 0xF03F000F // all data bits
#define PD_MODE_OUTP 0x50150005 // 01 : general purpose output mode
#define PD_MODE_INP  0x00000000 // 00 : input floating mode
#define PE_MODE_DATA 0xFFFFC000 // all data bits
#define PE_MODE_OUTP 0x55554000 // 01 : general purpose output mode
#define PE_MODE_INP  0x00000000 // 00 : input floating mode

GFX_STM32F407VxT6::GFX_STM32F407VxT6(bool bl_active_high)
{
  _cs   = PD7;
  _rs   = PD11;
  _rst  = 0;    // not available, driven from NRST
  _wr   = PD5;
  _rd   = PD4;
  _bl   = PB15;
  _bl_active_high = bl_active_high;
}

void GFX_STM32F407VxT6::reset()
{
  // not available, driven from NRST
  delay(20); // delay anyway, some displays need it after reset
}

void GFX_STM32F407VxT6::init()
{
  RCC->AHB1ENR |= 0x00000078; // enable GPIOD, GPIOE, GPIOF and GPIOG interface clock
  READ_BIT(RCC->AHB1ENR, RCC_AHB1ENR_GPIODEN); // delay after an RCC peripheral clock enabling
  GPIOD->BSRR = PD_USED_BITS; // preset all output high
  GPIOD->MODER &= ~PD_MODE_MASK;
  GPIOD->MODER |= PD_MODE_BITS;
  GPIOD->OTYPER &= ~PD_USED_BITS; // 0 : output push-pull
  GPIOD->OSPEEDR &= ~PD_MODE_MASK; // 0 : low speed
  GPIOD->PUPDR &= ~PD_MODE_MASK; // 0 : no pull-up, no pull-down

  GPIOE->BSRR = PE_USED_BITS; // preset all output high
  GPIOE->MODER &= ~PE_MODE_MASK;
  GPIOE->MODER |= PE_MODE_BITS;
  GPIOE->OTYPER &= ~PE_USED_BITS; // 0 : output push-pull
  GPIOE->OSPEEDR &= ~PE_MODE_MASK; // 0 : low speed
  GPIOE->PUPDR &= ~PE_MODE_MASK; // 0 : no pull-up, no pull-down

  digitalWrite(_cs, HIGH);
  pinMode(_cs, OUTPUT);
  digitalWrite(_rs, HIGH);
  pinMode(_rs, OUTPUT);
  digitalWrite(_bl, LOW);
  pinMode(_bl, OUTPUT);

  reset();
}

uint8_t GFX_STM32F407VxT6::readData()
{
  return readData16();
}

uint16_t GFX_STM32F407VxT6::readData16()
{
  volatile static uint32_t ttt = 1; // compiler can't optimize.
  // Set direction input
  GPIOD->MODER &= ~PD_MODE_DATA;
  GPIOE->MODER &= ~PE_MODE_DATA;
  // RD needs to be active long enough before first read
  if (ttt++) GPIOD->BSRR = (0x1 << (4 + 16)); // PD4 RD low read
  if (ttt++) GPIOD->BSRR = (0x1 << (4 + 16)); // PD4 RD low read
  if (ttt++) GPIOD->BSRR = (0x1 << (4 + 16)); // PD4 RD low read
  if (ttt++) GPIOD->BSRR = (0x1 << (4 + 16)); // PD4 RD low read
  ttt = 1; // don't miss once in 2**32
  uint16_t rv = 0;
  // The compiler efficiently codes this  so it is quite quick.
  rv |= (GPIOD->IDR & (0x1 << 10)) << (15 - 10); // PD10
  rv |= (GPIOD->IDR & (0x1 << 9)) << (14 - 9); // PD9
  rv |= (GPIOD->IDR & (0x1 << 8)) << (13 - 8); // PD8
  rv |= (GPIOE->IDR & (0x1 << 15)) >> -(12 - 15); // PE15
  rv |= (GPIOE->IDR & (0x1 << 14)) >> -(11 - 14); // PE14
  rv |= (GPIOE->IDR & (0x1 << 13)) >> -(10 - 13); // PE13
  rv |= (GPIOE->IDR & (0x1 << 12)) >> -(9 - 12); // PE12
  rv |= (GPIOE->IDR & (0x1 << 11)) >> -(8 - 11); // PE11
  rv |= (GPIOE->IDR & (0x1 << 10)) >> -(7 - 10); // PE10
  rv |= (GPIOE->IDR & (0x1 << 9)) >> -(6 - 9); // PE9
  rv |= (GPIOE->IDR & (0x1 << 8)) >> -(5 - 8); // PE8
  rv |= (GPIOE->IDR & (0x1 << 7)) >> -(4 - 7); // PE7
  rv |= (GPIOD->IDR & (0x1 << 1)) << (3 - 1); // PD1
  rv |= (GPIOD->IDR & (0x1 << 0)) << (2 - 0); // PD0
  rv |= (GPIOD->IDR & (0x1 << 15)) >> -(1 - 15); // PD15
  rv |= (GPIOD->IDR & (0x1 << 14)) >> -(0 - 14); // PD14
  GPIOD->BSRR = (0x1 << 4); // PD4 RD high
  // Set direction output again
  GPIOD->MODER &= ~PD_MODE_DATA;
  GPIOD->MODER |= PD_MODE_OUTP;
  GPIOE->MODER &= ~PD_MODE_DATA;
  GPIOE->MODER |= PE_MODE_OUTP;
  return rv;
}

uint32_t GFX_STM32F407VxT6::readRawData32(uint8_t part)
{
  // Set direction input
  GPIOD->MODER &= ~PD_MODE_DATA;
  GPIOE->MODER &= ~PE_MODE_DATA;
  GPIOD->BSRR = (0x1 << (4 + 16)); // PD4 RD low pulse
  GPIOD->BSRR = (0x1 << 4);        // PD4 RD high
  GPIOD->BSRR = (0x1 << (4 + 16)); // PD4 RD low read
  uint32_t rv = 0;
  if (part == 0) rv = GPIOD->IDR & PD_DATA_BITS;
  if (part == 1) rv = GPIOE->IDR & PE_DATA_BITS;
  GPIOD->BSRR = (0x1 << 4); // PD4 RD high
  // Set direction output again
  GPIOD->MODER &= ~PD_MODE_DATA;
  GPIOD->MODER |= PD_MODE_OUTP;
  GPIOE->MODER &= ~PD_MODE_DATA;
  GPIOE->MODER |= PE_MODE_OUTP;
  return rv;
}

void GFX_STM32F407VxT6::startWrite()
{
  GPIOD->BSRR = (0x1 << (7 + 16));  // PD7 CS low
}

void GFX_STM32F407VxT6::endWrite()
{
  GPIOD->BSRR = (0x1 << 7);  // PD7 CS high
}

void GFX_STM32F407VxT6::writeCommand(uint8_t c)
{
  GPIOD->BSRR = (0x1 << (11 + 16)); // PD11  RS low
  writeData16(c);
  GPIOD->BSRR = (0x1 << 11);        // PD11  RS high
}

void GFX_STM32F407VxT6::writeCommand16(uint16_t c)
{
  GPIOD->BSRR = (0x1 << (11 + 16)); // PD11  RS low
  writeData16(c);
  GPIOD->BSRR = (0x1 << 11);        // PD11  RS high
}

void GFX_STM32F407VxT6::writeData(uint8_t d)
{
  writeData16(d);
}

void GFX_STM32F407VxT6::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    writeData16(*d);
    d++;
    num--;
  }
}

void GFX_STM32F407VxT6::writeData16(uint16_t d, uint32_t num)
{
  volatile static uint32_t ttt = 1; // compiler can't optimize.
  GPIOD->BSRR = PD_DATA_BITS << 16; // clear bits
  GPIOE->BSRR = PE_DATA_BITS << 16; // clear bits

  // TFT connector uses FSMC pins
  // D0   D1   D2  D3  D4  D5  D6  D7   D8   D9   D10  D11  D12  D13 D14 D15
  // PD14 PD15 PD0 PD1 PE7 PE8 PE9 PE10 PE11 PE12 PE13 PE14 PE15 PD8 PD9 PD10

  // The compiler efficiently codes this  so it is quite quick.
  if (d & 0x8000) GPIOD->BSRR = 0x1 << 10; // PD10
  if (d & 0x4000) GPIOD->BSRR = 0x1 << 9; // PD9
  if (d & 0x2000) GPIOD->BSRR = 0x1 << 8; // PD8
  if (d & 0x1000) GPIOE->BSRR = 0x1 << 15; // PE15
  if (d & 0x0800) GPIOE->BSRR = 0x1 << 14; // PE14
  if (d & 0x0400) GPIOE->BSRR = 0x1 << 13; // PE13
  if (d & 0x0200) GPIOE->BSRR = 0x1 << 12; // PE12
  if (d & 0x0100) GPIOE->BSRR = 0x1 << 11; // PE11

  if (d & 0x0080) GPIOE->BSRR = 0x1 << 10; // PE10
  if (d & 0x0040) GPIOE->BSRR = 0x1 << 9; // PE9
  if (d & 0x0020) GPIOE->BSRR = 0x1 << 8; // PE8
  if (d & 0x0010) GPIOE->BSRR = 0x1 << 7; // PE7
  if (d & 0x0008) GPIOD->BSRR = 0x1 << 1; // PD1
  if (d & 0x0004) GPIOD->BSRR = 0x1 << 0; // PD0
  if (d & 0x0002) GPIOD->BSRR = 0x1 << 15; // PD15
  if (d & 0x0001) GPIOD->BSRR = 0x1 << 14; // PD14
  while (num)
  {
    if (ttt++) GPIOD->BSRR = (0x1 << (5 + 16)); // PD5 WR low
    if (ttt++) GPIOD->BSRR = (0x1 << (5 + 16)); // PD5 WR low
    if (ttt++) GPIOD->BSRR = (0x1 << (5 + 16)); // PD5 WR low
    if (ttt++) GPIOD->BSRR = (0x1 << (5 + 16)); // PD5 WR low
    ttt = 1; // don't miss once in 2**32
    GPIOD->BSRR = (0x1 << 5);        // PD5 WR high
    num--;
  }
}

void GFX_STM32F407VxT6::writeData16(const uint16_t* data, uint32_t n)
{
  while (n > 0)
  {
    writeData16(*data, 1);
    data++;
    n--;
  }
}

void GFX_STM32F407VxT6::enableBacklight(bool enable)
{
  digitalWrite(_bl, (enable == _bl_active_high));
}

#endif
#endif
