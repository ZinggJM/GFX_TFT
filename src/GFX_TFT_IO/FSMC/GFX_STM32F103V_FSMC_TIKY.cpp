// created by Jean-Marc Zingg to be the GFX_STM32F103V_FSMC_TIKY io class for the GFX_TFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is the io class for STM32F103VC boards with FMSC TFT connector, e.g. for 
// STM32 board no longer available. New board available with different processor.
//
// the above board has matched TFT connector for this display (aka Tiky)
// https://www.aliexpress.com/item/5-0-inch-HD-IPS-TFT-LCD-module-resistance-touch-with-PCB-adapter-board-854-480/32666829945.html
//
// for pin information see the backside of the TFT, for the data pin to port pin mapping see FSMC pin table STM32F103V doc.
//
// this io class can be used with or adapted to other STM32F103V processor boards with FSMC TFT connector.
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), board "Generic STM32F1 series" part "GENERIC F103VC".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#if defined(ARDUINO_ARCH_STM32)&& defined(STM32F1xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_GENERIC_F103VC) || defined(ARDUINO_GENERIC_F103VD) || defined(ARDUINO_GENERIC_F103VE) || defined(ARDUINO_GENERIC_F103VF)

#include "GFX_STM32F103V_FSMC_TIKY.h"

//                    |       |        | // Ruler on 8 & 16
#define PD_DATA_BITS 0b1100011100000011
#define PD_CTRL_BITS 0b0000100010110000  // PD11(FSMC_A16),PD7(FSMC_NE1),PD5(FSMC_NWE),PD4(FSMC_NOE)
#define PD_CRH_MASK  0xFF00FFFF
#define PD_CRH_FSMC  0xBB00BBBB          // AF PP 10MHz
#define PD_CRL_MASK          0xF0FF00FF
#define PD_CRL_FSMC          0xB0BB00BB  // AF PP 10MHz
//                    |       |        |
#define PE_DATA_BITS 0b1111111110000000
//                    |       |        |
#define PE_CRH_MASK  0xFFFFFFFF          // data bits used 15..8
#define PE_CRH_FSMC  0xBBBBBBBB          // AF PP 10MHz
#define PE_CRL_MASK          0xF0000000  // data bits used 7..0
#define PE_CRL_FSMC          0xB0000000  // AF PP 10MHz
//                    |       |        |

// 80 MHz processor clock
#define ADDSET 7 // 100ns (ADDSET+1)*12.5ns = CS to RW
#define DATAST 3 // 50ns  (DATAST+1)*12.5ns = RW length

#define CommandAccess FSMC_BANK1
#define DataAccess (FSMC_BANK1 + 0x20000)

GFX_STM32F103V_FSMC_TIKY::GFX_STM32F103V_FSMC_TIKY(bool bl_active_high)
{
  _cs   = PD7;  // FSMC_NE1
  _rs   = PD11; // FSMC_A16
  _rst  = PD13;
  _wr   = PD5;  // FSMC_NWE
  _rd   = PD4;  // FSMC_NOE
  _bl   = PA1;
  _bl_active_high = bl_active_high;
}

void GFX_STM32F103V_FSMC_TIKY::reset()
{
  digitalWrite(_rst, LOW);
  delay(10);
  digitalWrite(_rst, HIGH);
  delay(10);
}

void GFX_STM32F103V_FSMC_TIKY::init()
{
  RCC->AHBENR |= 0x00000100; // enable FSMC clock
  uint32_t t = RCC->AHBENR; // delay
  RCC->APB2ENR |= 0x00000060; // enable port D & E clocks
  t = RCC->APB2ENR; // delay
  (void)(t);
  GPIOD->CRH = (GPIOD->CRH & ~PD_CRH_MASK) | PD_CRH_FSMC;
  GPIOD->CRL = (GPIOD->CRL & ~PD_CRL_MASK) | PD_CRL_FSMC;
  GPIOE->CRH = (GPIOE->CRH & ~PE_CRH_MASK) | PE_CRH_FSMC;
  GPIOE->CRL = (GPIOE->CRL & ~PE_CRL_MASK) | PE_CRL_FSMC;
  FSMC_Bank1->BTCR[0] = 0x000010D9;
  FSMC_Bank1->BTCR[1] = (DATAST << 8) | ADDSET;
  digitalWrite(_rst, HIGH);
  digitalWrite(_bl, LOW);
  pinMode(_rst, OUTPUT);
  pinMode(_bl, OUTPUT);
  reset();
}

uint8_t GFX_STM32F103V_FSMC_TIKY::readData()
{
  return *(uint8_t*)DataAccess;
}

uint16_t GFX_STM32F103V_FSMC_TIKY::readData16()
{
  return *(uint16_t*)DataAccess;
}

uint32_t GFX_STM32F103V_FSMC_TIKY::readRawData32(uint8_t part)
{
  return *(uint16_t*)DataAccess;
}

void GFX_STM32F103V_FSMC_TIKY::writeCommand(uint8_t c)
{
  *(uint8_t*)CommandAccess = c;
}

void GFX_STM32F103V_FSMC_TIKY::writeCommand16(uint16_t c)
{
  *(uint16_t*)CommandAccess = c;
}

void GFX_STM32F103V_FSMC_TIKY::writeData(uint8_t d)
{
  *(uint8_t*)DataAccess = d;
}

void GFX_STM32F103V_FSMC_TIKY::writeData(uint8_t* d, uint32_t num)
{
  while (num > 0)
  {
    *(uint8_t*)DataAccess = *d;
    d++;
    num--;
  }
}

void GFX_STM32F103V_FSMC_TIKY::writeData16(uint16_t d, uint32_t num)
{
  while (num > 0)
  {
    *(uint16_t*)DataAccess = d;
    num--;
  }
}

void GFX_STM32F103V_FSMC_TIKY::writeData16(const uint16_t* data, uint32_t n)
{
  while (n > 0)
  {
    *(uint16_t*)DataAccess = *data;
    data++;
    n--;
  }
}

//void GFX_STM32F103V_FSMC_TIKY::selectRegister(bool rs_low)
//{
//}

void GFX_STM32F103V_FSMC_TIKY::enableBacklight(bool enable)
{
  digitalWrite(_bl, (enable == _bl_active_high));
}

#endif
#endif
