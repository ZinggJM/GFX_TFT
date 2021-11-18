// created by Jean-Marc Zingg to be the GFX_STM32F407VxT6_FSMC io class for the GFX_TFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is the io class for STM32F407VxT6 board with FMSC TFT connector, e.g. for https://www.aliexpress.com/item/1005001806399987.html
// details available here: http://www.lcdwiki.com/STM32F407VxT6_Board
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

#ifndef _GFX_STM32F407VxT6_FSMC_H_
#define _GFX_STM32F407VxT6_FSMC_H_

#include "../GFX_TFT_IO_Plugin.h"

#if defined(ARDUINO_ARCH_STM32) && defined(STM32F407xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_GENERIC_F407VETX) || defined(ARDUINO_GENERIC_F407VGTX)

class GFX_STM32F407VxT6_FSMC : public GFX_TFT_IO_Plugin
{
  public:
    GFX_STM32F407VxT6_FSMC(bool bl_active_high = true);
    const char* name = "GFX_STM32F407VxT6_FSMC";
    void reset();
    void init();
    uint8_t readData();
    uint16_t readData16();
    uint32_t readRawData32(uint8_t part); // debug purpose
    void startWrite() {};
    void endWrite() {};
    void writeCommand(uint8_t cmd);
    void writeCommand16(uint16_t cmd);
    void writeData(uint8_t data);
    void writeData(uint8_t* data, uint32_t n);
    void writeData16(uint16_t data, uint32_t n);
    void writeData16(const uint16_t* data, uint32_t n);
    void enableBacklight(bool enable = true);
  private:
    int16_t _cs, _rs, _rst, _wr, _rd, _bl; // Control lines
    bool _bl_active_high;
};

#define GFX_TFT_IO_PLUGIN_CLASS GFX_STM32F407VxT6_FSMC

#endif
#endif

#endif
