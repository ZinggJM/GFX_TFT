// created by Jean-Marc Zingg to be the GFX_STM32F103C_TIKY io class for the GFX_TFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is a special wiring for
// https://www.aliexpress.com/item/5-0-inch-HD-IPS-TFT-LCD-module-resistance-touch-with-PCB-adapter-board-854-480/32666829945.html
//
// for STM32F103C8T6 Minimum System, also known as BluePill
// https://www.aliexpress.com/item/10pcs-lot-STM32F103C8T6-ARM-STM32-Minimum-System-Development-Board-Module-For-Arduino/32581770994.html
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), board "Generic STM32F1 series" part "BluePill F103C8".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#if defined(ARDUINO_ARCH_STM32)&& defined(STM32F1xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"

#include "GFX_STM32F103C_TIKY.h"
#include <SPI.h>

// reserved & unusable pins
// PA9  : BOARD_USART1_TX_PIN -> to ESP8266
// PA10 : BOARD_USART1_RX_PIN <- from ESP8266
// PA11 : USBDM : USB serial connection
// PA12 : USBDP : USB serial connection
// PA13 : SWDIO
// PA14 : SWCLK
// PA15 : JTDI, remap needed for use as GPIO
// PB2  : BOOT1, no pin
// PB3  : BOARD_JTDO_PIN, remap needed for use as GPIO
// PB4  : BOARD_JTDI_PIN, remap needed for use as GPIO
// PB8  : usable ?

// connector pins
// 11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40
// CS  RS  RW  RD  RST D0  D1  D2  D3  D4  D5  D6  D7  D8  D9  D10 D11 D12 D13 D14 D15 BSY TCS SCK MIS MOS INT BL  GND 3.3
// B11 B12 B10 B13 B1  B14 B0  B15 A7  A8  A6  A15 A5  B3  A4  B4  A3  B5  A2  B6  A1  B7      B8              B9

// A used : 15,..,..,..,..,..,.., 8, 7, 6, 5, 4, 3, 2, 1,.., // 9
// A data : 15,..,..,..,..,..,.., 8, 7, 6, 5, 4, 3, 2, 1,.., // 9
//         |           |           |           |           |
// B used : 15,14,13,12,11,10,..,.., 7, 6, 5, 4, 3,.., 1, 0, // 13
// B data : 15,14,..,..,..,..,..,..,.., 6, 5, 4, 3,..,.., 0, // 7

//                    |       |        | // Ruler on 8 & 16
#define PA_USED_BITS 0b1000000111111110  // 0x81FE
#define PA_CRH_MASK  0xF000000F          // all pin bits used 15..8
#define PA_CRH_CNMD  0x30000003          // 00 11 output PP, 50MHz
#define PA_CRL_MASK          0xFFFFFFF0  // all pin bits used 7..0
#define PA_CRL_CNMD          0x33333330  // 00 11 output PP, 50MHz
//                    |       |        | //
#define PB_USED_BITS 0b1111110011111011  //0xFCFB
#define PB_CRH_MASK  0xFFFFFF00          // all pin bits used 15..8
#define PB_CRH_CNMD  0x33333300          // 00 11 output PP, 50MHz
#define PB_CRL_MASK          0xFFFFF0FF  // all pin bits used 7..0
#define PB_CRL_CNMD          0x33333033  // 00 11 output PP, 50MHz
//                    |       |        | //
#define PA_DATA_BITS 0b1000000111111110  //0x81FE
#define PA_CRH_DATA  0xF000000F          // data bits used 15..8
#define PA_CRH_OUTP  0x30000003          // 00 11 output PP, 50MHz
#define PA_CRH_INP   0x40000004          // 01 00 input floating
#define PA_CRL_DATA          0xFFFFFFF0  // data bits used 7..0
#define PA_CRL_OUTP          0x33333330  // 00 11 output PP, 50MHz
#define PA_CRL_INP           0x44444440  // 01 00 input floating
//                    |       |        | //
#define PB_DATA_BITS 0b1100000001111001  // 0xC079
#define PB_CRH_DATA  0xFF000000          // data bits used 15..8
#define PB_CRH_OUTP  0x33000000          // 00 11 output PP, 50MHz
#define PB_CRH_INP   0x44000000          // 01 00 input floating
#define PB_CRL_DATA          0x0FFFF00F  // data bits used 7..0
#define PB_CRL_OUTP          0x03333003  // 00 11 output PP, 50MHz
#define PB_CRL_INP           0x04444004  // 01 00 input floating

GFX_STM32F103C_TIKY::GFX_STM32F103C_TIKY(bool bl_active_high)
{
  _cs   = PB11;
  _rs   = PB12;
  _wr   = PB10;
  _rd   = PB13;
  _rst  = PB1;
  _bl   = PB9;
  _bl_active_high = bl_active_high;
}

void GFX_STM32F103C_TIKY::reset()
{
  digitalWrite(_rst, LOW);
  delay(10);
  digitalWrite(_rst, HIGH);
  delay(10);
}

void GFX_STM32F103C_TIKY::init()
{
  RCC->APB2ENR |= 0x0000000C; // enable port A & B clocks
  uint32_t t = RCC->AHBENR; // delay
  uint32_t mapr = AFIO->MAPR;
  mapr &= ~AFIO_MAPR_SWJ_CFG;
  mapr |= AFIO_MAPR_SWJ_CFG_JTAGDISABLE;
  AFIO->MAPR = mapr; // remap JTAG pin A15 as GPIO
  digitalWrite(_cs, HIGH);
  digitalWrite(_rs, HIGH);
  digitalWrite(_wr, HIGH);
  digitalWrite(_rd, HIGH);
  digitalWrite(_rst, HIGH);
  digitalWrite(_bl, LOW);
  pinMode(_cs, OUTPUT);
  pinMode(_rs, OUTPUT);
  pinMode(_wr, OUTPUT);
  pinMode(_rd, OUTPUT);
  pinMode(_rst, OUTPUT);
  pinMode(_bl, OUTPUT);
  reset();
  GPIOA->BRR = PA_DATA_BITS; // Clear bits
  GPIOB->BRR = PB_DATA_BITS; // Clear bits
  setDataPinsOutput();
  enableBacklight(true);
}

void GFX_STM32F103C_TIKY::setDataPinsOutput()
{
  GPIOA->CRH &= ~PA_CRH_DATA;
  GPIOA->CRH |= PA_CRH_OUTP;
  GPIOA->CRL &= ~PA_CRL_DATA;
  GPIOA->CRL |= PA_CRL_OUTP;
  GPIOB->CRH &= ~PB_CRH_DATA;
  GPIOB->CRH |= PB_CRH_OUTP;
  GPIOB->CRL &= ~PB_CRL_DATA;
  GPIOB->CRL |= PB_CRL_OUTP;
}

void GFX_STM32F103C_TIKY::setDataPinsInput()
{
  GPIOA->CRH &= ~PA_CRH_DATA;
  GPIOA->CRH |= PA_CRH_INP;
  GPIOA->CRL &= ~PA_CRL_DATA;
  GPIOA->CRL |= PA_CRL_INP;
  GPIOB->CRH &= ~PB_CRH_DATA;
  GPIOB->CRH |= PB_CRH_INP;
  GPIOB->CRL &= ~PB_CRL_DATA;
  GPIOB->CRL |= PB_CRL_INP;
}

uint8_t GFX_STM32F103C_TIKY::readData()
{
  return readData16();
}

uint16_t GFX_STM32F103C_TIKY::readData16()
{
  setDataPinsInput();
  //  GPIOB->BRR = (0x1 << 13);  // B13 RD low pulse
  //  GPIOB->BSRR = (0x1 << 13); // B13 RD high
  GPIOB->BRR = (0x1 << 13);  // B13 RD low read
  uint16_t rv = 0;
  // The compiler efficiently codes this
  // so it is quite quick.                             Port.bit
  rv |= (GPIOA->IDR & (0x1 << 1)) << (15 - 1); // PA1
  rv |= (GPIOB->IDR & (0x1 << 6)) << (14 - 6); // PB6
  rv |= (GPIOA->IDR & (0x1 << 2)) << (13 - 2); // PA2
  rv |= (GPIOB->IDR & (0x1 << 5)) << (12 - 5); // PB5
  rv |= (GPIOA->IDR & (0x1 << 3)) << (11 - 3); // PA3
  rv |= (GPIOB->IDR & (0x1 << 4)) << (10 - 4); // PB4
  rv |= (GPIOA->IDR & (0x1 << 4)) << ( 9 - 4); // PA4
  rv |= (GPIOB->IDR & (0x1 << 3)) << ( 8 - 3); // PB3
  // so it is quite quick.                             Port.bit
  rv |= (GPIOA->IDR & (0x1 << 5)) << (7 - 5);  // PA5
  rv |= (GPIOA->IDR & (0x1 << 15)) >> -(6 - 15); // PA15
  rv |= (GPIOA->IDR & (0x1 << 6)) >> -(5 - 6); // PA6
  rv |= (GPIOA->IDR & (0x1 << 8)) >> -(4 - 8); // PA8
  rv |= (GPIOA->IDR & (0x1 << 7)) >> -(3 - 7); // PA7
  rv |= (GPIOB->IDR & (0x1 << 15)) >> -(2 - 15); // PB15
  rv |= (GPIOB->IDR & (0x1 << 0)) << (1 - 0);  // PB0
  rv |= (GPIOB->IDR & (0x1 << 14)) >> -(0 - 14); // PB14
  GPIOB->BSRR = (0x1 << 13); // B13 RD high
  setDataPinsOutput();
  return rv;
}

uint32_t GFX_STM32F103C_TIKY::readRawData32(uint8_t part)
{
  setDataPinsInput();
  GPIOB->BRR = (0x1 << 13);  // B13 RD low pulse
  GPIOB->BSRR = (0x1 << 13); // B13 RD high
  GPIOB->BRR = (0x1 << 13);  // B13 RD low read
  uint32_t rv = 0;
  if (part == 0) rv = GPIOA->IDR & PA_DATA_BITS;
  if (part == 1) rv = GPIOB->IDR & PB_DATA_BITS;
  GPIOB->BSRR = (0x1 << 13); // B13 RD high
  setDataPinsOutput();
  return rv;
}

void GFX_STM32F103C_TIKY::startWrite()
{
  GPIOB->BRR = (0x1 << 11);  // B11 CS low
}

void GFX_STM32F103C_TIKY::endWrite()
{
  GPIOB->BSRR = (0x1 << 11);  // B11 CS high
}

void GFX_STM32F103C_TIKY::writeCommand(uint8_t c)
{
  GPIOB->BRR = (0x1 << 12);  // B12 RS low
  writeData16(c);
  GPIOB->BSRR = (0x1 << 12);  // B12 RS high
}

void GFX_STM32F103C_TIKY::writeCommand16(uint16_t c)
{
  GPIOB->BRR = (0x1 << 12);  // B12 RS low
  writeData16(c);
  GPIOB->BSRR = (0x1 << 12);  // B12 RS high
}

void GFX_STM32F103C_TIKY::writeData(uint8_t d)
{
  writeData16(d);
}

void GFX_STM32F103C_TIKY::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    writeData16(*d);
    d++;
    num--;
  }
}

void GFX_STM32F103C_TIKY::writeData16(uint16_t d, uint32_t num)
{
  GPIOA->BRR = PA_DATA_BITS; // Clear bits
  GPIOB->BRR = PB_DATA_BITS; // Clear bits

  // The compiler efficiently codes this
  // so it is quite quick.                    Port.bit
  if (d & 0x8000) GPIOA->BSRR = 0x1 << 1; // PA1
  if (d & 0x4000) GPIOB->BSRR = 0x1 << 6; // PB6
  if (d & 0x2000) GPIOA->BSRR = 0x1 << 2; // PA2
  if (d & 0x1000) GPIOB->BSRR = 0x1 << 5; // PB5
  if (d & 0x0800) GPIOA->BSRR = 0x1 << 3; // PA3
  if (d & 0x0400) GPIOB->BSRR = 0x1 << 4; // PB4
  if (d & 0x0200) GPIOA->BSRR = 0x1 << 4; // PA4
  if (d & 0x0100) GPIOB->BSRR = 0x1 << 3; // PB3

  // so it is quite quick.                    Port.bit
  if (d & 0x0080) GPIOA->BSRR = 0x1 << 5; // PA5
  if (d & 0x0040) GPIOA->BSRR = 0x1 << 15; // PA15
  if (d & 0x0020) GPIOA->BSRR = 0x1 << 6; // PA6
  if (d & 0x0010) GPIOA->BSRR = 0x1 << 8; // PA8
  if (d & 0x0008) GPIOA->BSRR = 0x1 << 7; // PA7
  if (d & 0x0004) GPIOB->BSRR = 0x1 << 15; // PB15
  if (d & 0x0002) GPIOB->BSRR = 0x1 << 0; // PB0
  if (d & 0x0001) GPIOB->BSRR = 0x1 << 14; // PB14
  while (num > 0)
  {
    GPIOB->BRR = (0x1 << 10);  // B10 WR low
    GPIOB->BRR = (0x1 << 10);  // B10 WR low
    GPIOB->BSRR = (0x1 << 10);  // B10 WR high
    num--;
  }
}

void GFX_STM32F103C_TIKY::writeData16(const uint16_t* data, uint32_t n)
{
  while (n > 0)
  {
    writeData16(*data, 1);
    data++;
    n--;
  }
}

//void GFX_STM32F103C_TIKY::selectRegister(bool rs_low)
//{
//  digitalWrite(_rs, (rs_low ? LOW : HIGH));
//}

void GFX_STM32F103C_TIKY::enableBacklight(bool enable)
{
  digitalWrite(_bl, (enable == _bl_active_high));
}

#endif
