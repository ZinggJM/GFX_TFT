// created by Jean-Marc Zingg to be the GFX_STM32F103ZET6 io class for the GFX_TFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is a wiring for the TFT connector, e.g. of
// https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
//
// for the STM32F103ZET6 board with matching TFT connector (FSMC pins)
// https://www.aliexpress.com/item/32842677650.html
//
// for pin information see the backside of the TFT, for the data pin to port pin mapping see FSMC pin table STM32F103V doc.
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), board "Generic STM32F1 series" part "Generic F103ZE".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#if defined(ARDUINO_ARCH_STM32)&& defined(STM32F1xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_GENERIC_F103ZETX)

#include "GFX_STM32F103ZET6.h"

// TFT connector uses FSMC pins
// D0   D1   D2  D3  D4  D5  D6  D7   D8   D9   D10  D11  D12  D13 D14 D15
// PD14 PD15 PD0 PD1 PE7 PE8 PE9 PE10 PE11 PE12 PE13 PE14 PE15 PD8 PD9 PD10

// connector pins
// 01  02  03  04  05  06  07  08  09  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32
// GND RST D15 D14 D13 D12 D11 D10 D9  D8  D7  D6  D5  D4  D3  D2  D1  D0  RD  WR  RS  CS  SCK SCS SI  SO  INT BLK SET GND 3.3 GND
//         D10 D9  D8  E15 E14 E13 E12 E11 E10 E9  E8  E7  D1  D0  D15 D14 D4  D5  G0  G12                     -

//                    |       |        | // Ruler on 8 & 16
#define PD_DATA_BITS 0b1100011100000011;
//                    |       |        |
#define PD_CRH_DATA  0xFF000FFF          // data bits used 15..8
#define PD_CRH_OUTP  0x33000333          // 00 11 output PP, 50MHz
#define PD_CRH_INP   0x44000444          // 01 00 input floating
#define PD_CRL_DATA          0x000000FF  // data bits used 7..0
#define PD_CRL_OUTP          0x00000033  // 00 11 output PP, 50MHz
#define PD_CRL_INP           0x00000044  // 01 00 input floating
//                    |       |        |
#define PE_DATA_BITS 0b1111111110000000;
//                    |       |        |
#define PE_CRH_DATA  0xFFFFFFFF          // data bits used 15..8
#define PE_CRH_OUTP  0x33333333          // 00 11 output PP, 50MHz
#define PE_CRH_INP   0x44444444          // 01 00 input floating
#define PE_CRL_DATA          0xF0000000  // data bits used 7..0
#define PE_CRL_OUTP          0x30000000  // 00 11 output PP, 50MHz
#define PE_CRL_INP           0x40000000  // 01 00 input floating
//                    |       |        |

GFX_STM32F103ZET6::GFX_STM32F103ZET6()
{
  _cs   = PG12; // FSMC_NE4
  _rs   = PG0;  // FSMC_A10
  _rst  = 0; // not available, driven from NRST
  _wr   = PD5;  // FSMC_NWE
  _rd   = PD4;  // FSMC_NOE
  _bl   = 0; // not available
}

void GFX_STM32F103ZET6::reset()
{
  // _rst pin not available
}

void GFX_STM32F103ZET6::init()
{
  RCC->APB2ENR |= 0x000001E0; // // enable GPIOD, GPIOE, GPIOF and GPIOG interface clock
  uint32_t t = RCC->AHBENR; // delay
  (void)(t);
  digitalWrite(_rs, HIGH);
  digitalWrite(_cs, HIGH);
  digitalWrite(_wr, HIGH);
  digitalWrite(_rd, HIGH);
  pinMode(_rs, OUTPUT);
  pinMode(_cs, OUTPUT);
  pinMode(_wr, OUTPUT);
  pinMode(_rd, OUTPUT);
  reset();
  setDataPinsOutput();
}

void GFX_STM32F103ZET6::setDataPinsOutput()
{
  GPIOD->CRH &= ~PD_CRH_DATA;
  GPIOD->CRH |= PD_CRH_OUTP;
  GPIOD->CRL &= ~PD_CRL_DATA;
  GPIOD->CRL |= PD_CRL_OUTP;
  GPIOE->CRH &= ~PE_CRH_DATA;
  GPIOE->CRH |= PE_CRH_OUTP;
  GPIOE->CRL &= ~PE_CRL_DATA;
  GPIOE->CRL |= PE_CRL_OUTP;
}

void GFX_STM32F103ZET6::setDataPinsInput()
{
  GPIOD->CRH &= ~PD_CRH_DATA;
  GPIOD->CRH |= PD_CRH_INP;
  GPIOD->CRL &= ~PD_CRL_DATA;
  GPIOD->CRL |= PD_CRL_INP;
  GPIOE->CRH &= ~PE_CRH_DATA;
  GPIOE->CRH |= PE_CRH_INP;
  GPIOE->CRL &= ~PE_CRL_DATA;
  GPIOE->CRL |= PE_CRL_INP;
}

uint8_t GFX_STM32F103ZET6::readData()
{
  return readData16();
}

uint16_t GFX_STM32F103ZET6::readData16()
{
  setDataPinsInput();
//  GPIOD->BRR = (0x1 << 4);  // PD4 RD low pulse
//  GPIOD->BSRR = (0x1 << 4); // PD4 RD high
  GPIOD->BRR = (0x1 << 4);  // PD4 RD low read
  uint16_t rv = 0;
  // The compiler efficiently codes this
  // so it is quite quick.                    Port.bit
  rv |= (GPIOD->IDR & (0x1 << 10)) << (15 - 10); // PD10
  rv |= (GPIOD->IDR & (0x1 << 9)) << (14 - 9); // PD9
  rv |= (GPIOD->IDR & (0x1 << 8)) << (13 - 8); // PD8
  rv |= (GPIOE->IDR & (0x1 << 15)) >> -(12 - 15); // PE15
  rv |= (GPIOE->IDR & (0x1 << 14)) >> -(11 - 14); // PE14
  rv |= (GPIOE->IDR & (0x1 << 13)) >> -(10 - 13); // PE13
  rv |= (GPIOE->IDR & (0x1 << 12)) >> -(9 - 12); // PE12
  rv |= (GPIOE->IDR & (0x1 << 11)) >> -(8 - 11); // PE11
  // so it is quite quick.                    Port.bit
  rv |= (GPIOE->IDR & (0x1 << 10)) >> -(7 - 10); // PE10
  rv |= (GPIOE->IDR & (0x1 << 9)) >> -(6 - 9); // PE9
  rv |= (GPIOE->IDR & (0x1 << 8)) >> -(5 - 8); // PE8
  rv |= (GPIOE->IDR & (0x1 << 7)) >> -(4 - 7); // PE7
  rv |= (GPIOD->IDR & (0x1 << 1)) << (3 - 1); // PD1
  rv |= (GPIOD->IDR & (0x1 << 0)) << (2 - 0); // PD0
  rv |= (GPIOD->IDR & (0x1 << 15)) >> -(1 - 15); // PD15
  rv |= (GPIOD->IDR & (0x1 << 14)) >> -(0 - 14); // PD14
  GPIOD->BSRR = (0x1 << 4); // PD4 RD high
  setDataPinsOutput();
  return rv;
}

uint32_t GFX_STM32F103ZET6::readRawData32(uint8_t part)
{
  setDataPinsInput();
  GPIOD->BRR = (0x1 << 4);  // PD4 RD low pulse
  GPIOD->BSRR = (0x1 << 4); // PD4 RD high
  GPIOD->BRR = (0x1 << 4);  // PD4 RD low read
  uint32_t rv = 0;
  if (part == 0) rv = GPIOD->IDR & PD_DATA_BITS;
  if (part == 1) rv = GPIOE->IDR & PE_DATA_BITS;
  GPIOD->BSRR = (0x1 << 4); // PD4 RD high
  setDataPinsOutput();
  return rv;
}

void GFX_STM32F103ZET6::startWrite()
{
  GPIOG->BRR = (0x1 << 12);  // PG12 CS low
}

void GFX_STM32F103ZET6::endWrite()
{
  GPIOG->BSRR = (0x1 << 12);  // PG12 CS high
}

void GFX_STM32F103ZET6::writeCommand(uint8_t c)
{
  GPIOG->BRR = (0x1 << 0);   // PG0  RS low
  writeData16(c);
  GPIOG->BSRR = (0x1 << 0);   // PG0  RS high
}

void GFX_STM32F103ZET6::writeCommand16(uint16_t c)
{
  GPIOG->BRR = (0x1 << 0);   // PG0  RS low
  writeData16(c);
  GPIOG->BSRR = (0x1 << 0);   // PG0  RS high
}

void GFX_STM32F103ZET6::writeData(uint8_t d)
{
  writeData16(d);
}

void GFX_STM32F103ZET6::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    writeData16(*d);
    d++;
    num--;
  }
}

void GFX_STM32F103ZET6::writeData16(uint16_t d, uint32_t num)
{
  //                 |       |        | // Ruler on 8 & 16 (hopefully)
  GPIOD->BRR = 0b1100011100000011; // Clear bits
  //                 |       |        |
  GPIOE->BRR = 0b1111111110000000; // Clear bits

  // The compiler efficiently codes this
  // so it is quite quick.                         Port.bit
  if (d & 0x8000) GPIOD->BSRR = 0x1 << 10; // PD10
  if (d & 0x4000) GPIOD->BSRR = 0x1 << 9;  // PD9
  if (d & 0x2000) GPIOD->BSRR = 0x1 << 8;  // PD8
  if (d & 0x1000) GPIOE->BSRR = 0x1 << 15; // PE15
  if (d & 0x0800) GPIOE->BSRR = 0x1 << 14; // PE14
  if (d & 0x0400) GPIOE->BSRR = 0x1 << 13; // PE13
  if (d & 0x0200) GPIOE->BSRR = 0x1 << 12; // PE12
  if (d & 0x0100) GPIOE->BSRR = 0x1 << 11; // PE11

  // so it is quite quick.                         Port.bit
  if (d & 0x0080) GPIOE->BSRR = 0x1 << 10; // PE10
  if (d & 0x0040) GPIOE->BSRR = 0x1 << 9;  // PE9
  if (d & 0x0020) GPIOE->BSRR = 0x1 << 8;  // PE8
  if (d & 0x0010) GPIOE->BSRR = 0x1 << 7;  // PE7
  if (d & 0x0008) GPIOD->BSRR = 0x1 << 1;  // PD1
  if (d & 0x0004) GPIOD->BSRR = 0x1 << 0;  // PD0
  if (d & 0x0002) GPIOD->BSRR = 0x1 << 15; // PD15
  if (d & 0x0001) GPIOD->BSRR = 0x1 << 14; // PD14
  while (num > 0)
  {
    GPIOD->BRR = (0x1 << 5);  // PD5 WR low
    GPIOD->BRR = (0x1 << 5);  // PD5 WR low
    GPIOD->BSRR = (0x1 << 5); // PD5 WR high
    num--;
  }
}

void GFX_STM32F103ZET6::writeData16(const uint16_t* data, uint32_t n)
{
  while (n > 0)
  {
    writeData16(*data, 1);
    data++;
    n--;
  }
}

void GFX_STM32F103ZET6::enableBacklight(bool enable)
{
  //digitalWrite(_bl, (enable == _bl_active_high));
}

#endif
#endif
