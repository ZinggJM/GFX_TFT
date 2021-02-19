// created by Jean-Marc Zingg to be the GFX_STM32Nucleo144_FSMC io class for the GFX_TFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is the io class for the TFT connector on my Nucleo-144 proto board for e.g.
// https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), 
// board "Nucleo-144" part "Nucleo F767ZI", or "Nucleo F746ZG".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#ifndef _GFX_STM32Nucleo144_FSMC_H_
#define _GFX_STM32Nucleo144_FSMC_H_

#include "../GFX_TFT_IO_Plugin.h"

#if defined(ARDUINO_ARCH_STM32) && defined(STM32F7xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_NUCLEO_F767ZI) || defined(ARDUINO_NUCLEO_F746ZG)

class GFX_STM32Nucleo144_FSMC : public GFX_TFT_IO_Plugin
{
  public:
    GFX_STM32Nucleo144_FSMC(bool bl_active_high = false);
    const char* name = "GFX_STM32Nucleo144_FSMC";
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
    int8_t _cs, _rs, _rst, _wr, _rd, _bl; // Control lines
    bool _bl_active_high;
};

#define GFX_TFT_IO_PLUGIN_CLASS GFX_STM32Nucleo144_FSMC

#endif
#endif

#endif
