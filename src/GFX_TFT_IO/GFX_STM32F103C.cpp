// created by Jean-Marc Zingg to be the GFX_STM32F103C io class for the GFX_TFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is a special wiring for
// https://www.aliexpress.com/item/New-7-inch-TFT-LCD-module-800x480-SSD1963-Touch-PWM-For-Arduino-AVR-STM32-ARM/32667404985.html
//
// for STM32F103C8T6 Minimum System, also known as BluePill
// https://www.aliexpress.com/item/10pcs-lot-STM32F103C8T6-ARM-STM32-Minimum-System-Development-Board-Module-For-Arduino/32581770994.html
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), board "Generic STM32F1 series" part "BluePill F103C8".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#if defined(ARDUINO_ARCH_STM32)&& defined(STM32F1xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"

#include "GFX_STM32F103C.h"

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
// 01  02  03  04  05  06  07  08  09  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30  31  32  33  34  35  36  37  38  39  40
// GND D0  3.3 D1  NC  D2  RS  D3  WR  D4  RD  D5  D8  D6  D9  D7  D10 TCK D11 TCS D12 TDI D13 NC  D14 TDO D15 TIR CS  SDO FCS SCK RST SDI 5V  SCS BL  NC  NC  NC
// GND B12 3.3 B13 NC  B14 B11 B15 B10 A8  B1  A15 B0  B3  A7  B4  A6  NC  A5  NC  A4  NC  A3  NC  A2  NC  A1  NC  A0  NC  C15 NC  C14 NC  5V  NC  B9

// A used : 15,..,..,..,..,..,.., 8, 7, 6, 5, 4, 3, 2, 1, 0, // 10
// A data : 15,..,..,..,..,..,.., 8, 7, 6, 5, 4, 3, 2, 1,.., // 9
//         |           |           |           |           |
// B used : 15,14,13,12,11,10, 9,..,..,..,.., 4, 3,.., 1, 0, // 13
// B data : 15,14,13,12,..,..,..,..,..,..,.., 4, 3,..,.., 0, // 7

//                    |       |        | //
#define PA_DATA_BITS 0b1000000111111110  //
#define PA_CRH_DATA  0xF000000F          // data bits used 15..8
#define PA_CRH_OUTP  0x30000003          // 00 11 output PP, 50MHz
#define PA_CRH_INP   0x40000004          // 01 00 input floating
#define PA_CRL_DATA          0xFFFFFFF0  // data bits used 7..0
#define PA_CRL_OUTP          0x33333330  // 00 11 output PP, 50MHz
#define PA_CRL_INP           0x44444440  // 01 00 input floating
//                    |       |        | //
#define PB_DATA_BITS 0b1111000000011001  //
#define PB_CRH_DATA  0xFFFF0000          // data bits used 15..8
#define PB_CRH_OUTP  0x33330000          // 00 11 output PP, 50MHz
#define PB_CRH_INP   0x44440000          // 01 00 input floating
#define PB_CRL_DATA          0x000FF00F  // data bits used 7..0
#define PB_CRL_OUTP          0x00033003  // 00 11 output PP, 50MHz
#define PB_CRL_INP           0x00044004  // 01 00 input floating

GFX_STM32F103C::GFX_STM32F103C(bool bl_active_high)
{
  _cs   = PA0;
  _rs   = PB11;
  _wr   = PB10;
  _rd   = PB1;
  _rst  = PC14;
  _bl   = PB9;
  _bl_active_high = bl_active_high;
}

void GFX_STM32F103C::reset()
{
  digitalWrite(_rst, LOW);
  delay(10);
  digitalWrite(_rst, HIGH);
  delay(10);
}

void GFX_STM32F103C::init()
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

void GFX_STM32F103C::setDataPinsOutput()
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

void GFX_STM32F103C::setDataPinsInput()
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

uint8_t GFX_STM32F103C::readData()
{
  return readData16();
}

// GND D0  3.3 D1  NC  D2  RS  D3  WR  D4  RD  D5  D8  D6  D9  D7  D10 TCK D11 TCS D12 TDI D13 NC  D14 TDO D15 TIR CS  SDO FCS SCK RST SDI 5V  SCS BL  NC  NC  NC
// GND B12 3.3 B13 NC  B14 B11 B15 B10 A8  B1  A15 B0  B3  A7  B4  A6  NC  A5  NC  A4  NC  A3  NC  A2  NC  A1  NC  A0  NC  C15 NC  C14 NC  5V  NC  B9

uint16_t GFX_STM32F103C::readData16()
{
  setDataPinsInput();
  GPIOB->BRR = (0x1 << 1);  // B1 RD low read
  uint16_t rv = 0;
  // The compiler efficiently codes this
  // so it is quite quick.                             Port.bit
  rv |= (GPIOA->IDR & (0x1 << 1)) << (15 - 1); // PA1
  rv |= (GPIOA->IDR & (0x1 << 2)) << (14 - 2); // PA2
  rv |= (GPIOA->IDR & (0x1 << 3)) << (13 - 3); // PA3
  rv |= (GPIOA->IDR & (0x1 << 4)) << (12 - 4); // PA4
  rv |= (GPIOA->IDR & (0x1 << 5)) << (11 - 5); // PA5
  rv |= (GPIOA->IDR & (0x1 << 6)) << (10 - 6); // PA6
  rv |= (GPIOA->IDR & (0x1 << 7)) << ( 9 - 7); // PA7
  rv |= (GPIOB->IDR & (0x1 << 0)) << ( 8 - 0); // PB0
  // so it is quite quick.                             Port.bit
  rv |= (GPIOB->IDR & (0x1 << 4)) << (7 - 5);  // PB4
  rv |= (GPIOB->IDR & (0x1 << 4)) << (6 - 3); // PB3
  rv |= (GPIOA->IDR & (0x1 << 15)) >> -(5 - 15); // PA15
  rv |= (GPIOA->IDR & (0x1 << 8)) >> -(4 - 8); // PA8
  rv |= (GPIOB->IDR & (0x1 << 15)) >> -(3 - 15); // PB15
  rv |= (GPIOB->IDR & (0x1 << 14)) >> -(2 - 14); // PB14
  rv |= (GPIOB->IDR & (0x1 << 13)) >> -(1 - 13);  // PB13
  rv |= (GPIOB->IDR & (0x1 << 12)) >> -(0 - 12); // PB12
  GPIOB->BSRR = (0x1 << 1); // B1 RD high
  setDataPinsOutput();
  return rv;
}

uint32_t GFX_STM32F103C::readRawData32(uint8_t part)
{
  setDataPinsInput();
  GPIOB->BRR = (0x1 << 1);  // B1 RD low read
  uint32_t rv = 0;
  if (part == 0) rv = GPIOA->IDR & PA_DATA_BITS;
  if (part == 1) rv = GPIOB->IDR & PB_DATA_BITS;
  GPIOB->BSRR = (0x1 << 1); // B1 RD high
  setDataPinsOutput();
  return rv;
}

void GFX_STM32F103C::startWrite()
{
  GPIOA->BRR = (0x1 << 0);  // A0 CS low
}

void GFX_STM32F103C::endWrite()
{
  GPIOA->BSRR = (0x1 << 0);  // A0 CS high
}

void GFX_STM32F103C::writeCommand(uint8_t c)
{
  GPIOB->BRR = (0x1 << 11);  // B11 RS low
  writeData16(c);
  GPIOB->BSRR = (0x1 << 11);  // B11 RS high
}

void GFX_STM32F103C::writeCommand16(uint16_t c)
{
  GPIOB->BRR = (0x1 << 11);  // B11 RS low
  writeData16(c);
  GPIOB->BSRR = (0x1 << 11);  // B11 RS high
}

void GFX_STM32F103C::writeData(uint8_t d)
{
  writeData16(d);
}

void GFX_STM32F103C::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    writeData16(*d);
    d++;
    num--;
  }
}

// GND D0  3.3 D1  NC  D2  RS  D3  WR  D4  RD  D5  D8  D6  D9  D7  D10 TCK D11 TCS D12 TDI D13 NC  D14 TDO D15 TIR CS  SDO FCS SCK RST SDI 5V  SCS BL  NC  NC  NC
// GND B12 3.3 B13 NC  B14 B11 B15 B10 A8  B1  A15 B0  B3  A7  B4  A6  NC  A5  NC  A4  NC  A3  NC  A2  NC  A1  NC  A0  NC  C15 NC  C14 NC  5V  NC  B9

void GFX_STM32F103C::writeData16(uint16_t d, uint32_t num)
{
  GPIOA->BRR = PA_DATA_BITS; // Clear bits
  GPIOB->BRR = PB_DATA_BITS; // Clear bits

  // The compiler efficiently codes this
  // so it is quite quick.                    Port.bit
  if (d & 0x8000) GPIOA->BSRR = 0x1 << 1; // PA1
  if (d & 0x4000) GPIOA->BSRR = 0x1 << 2; // PA2
  if (d & 0x2000) GPIOA->BSRR = 0x1 << 3; // PA3
  if (d & 0x1000) GPIOA->BSRR = 0x1 << 4; // PA4
  if (d & 0x0800) GPIOA->BSRR = 0x1 << 5; // PA5
  if (d & 0x0400) GPIOA->BSRR = 0x1 << 6; // PA6
  if (d & 0x0200) GPIOA->BSRR = 0x1 << 6; // PA7
  if (d & 0x0100) GPIOB->BSRR = 0x1 << 0; // PB0

  // so it is quite quick.                    Port.bit
  if (d & 0x0080) GPIOB->BSRR = 0x1 << 4; // PB4
  if (d & 0x0040) GPIOB->BSRR = 0x1 << 3; // PB3
  if (d & 0x0020) GPIOA->BSRR = 0x1 << 15; // PA15
  if (d & 0x0010) GPIOA->BSRR = 0x1 << 8; // PA8
  if (d & 0x0008) GPIOB->BSRR = 0x1 << 15; // PB15
  if (d & 0x0004) GPIOB->BSRR = 0x1 << 14; // PB14
  if (d & 0x0002) GPIOB->BSRR = 0x1 << 13; // PB13
  if (d & 0x0001) GPIOB->BSRR = 0x1 << 12; // PB12
  while (num > 0)
  {
    GPIOB->BRR = (0x1 << 10);  // B10 WR low
    GPIOB->BRR = (0x1 << 10);  // B10 WR low
    GPIOB->BRR = (0x1 << 10);  // B10 WR low
    GPIOB->BRR = (0x1 << 10);  // B10 WR low
    GPIOB->BSRR = (0x1 << 10);  // B10 WR high
    num--;
  }
}

void GFX_STM32F103C::writeData16(const uint16_t* data, uint32_t n)
{
  while (n > 0)
  {
    writeData16(*data, 1);
    data++;
    n--;
  }
}

//void GFX_STM32F103C::selectRegister(bool rs_low)
//{
//  digitalWrite(_rs, (rs_low ? LOW : HIGH));
//}

void GFX_STM32F103C::enableBacklight(bool enable)
{
  digitalWrite(_bl, (enable == _bl_active_high));
}

#endif
