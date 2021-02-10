// created by Jean-Marc Zingg to be the GFX_STM32F407VET6 io class for the GFX_TFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is a special wiring for
// https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
//
// for the black STM32F407VET6 board with FMSC TFT connector, e.g.
// https://www.aliexpress.com/item/Free-shipping-STM32F407VET6-development-board-Cortex-M4-STM32-minimum-system-learning-board-ARM-core-board/32618222721.html
// for details of the board see https://github.com/mcauser/BLACK_F407VE
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), 
// board "Generic STM32F4 series" part "BLACK F407VE", or "BLACK F407VG", or "BLACK F407ZE", or "BLACK F407ZG".

#ifndef _GFX_STM32F407VET6_H_
#define _GFX_STM32F407VET6_H_

#include "GFX_TFT_IO_Plugin.h"

#if defined(ARDUINO_ARCH_STM32) && defined(STM32F407xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_BLACK_F407VE) || defined(ARDUINO_BLACK_F407VG) || defined(ARDUINO_BLACK_F407ZE) || defined(ARDUINO_BLACK_F407ZG)

class GFX_STM32F407VET6 : public GFX_TFT_IO_Plugin
{
  public:
    GFX_STM32F407VET6(bool bl_active_high = true);
    const char* name = "GFX_STM32F407VET6";
    void reset();
    void init();
    uint8_t readData();
    uint16_t readData16();
    uint32_t readRawData32(uint8_t part); // debug purpose
    void startWrite();
    void endWrite();
    void writeCommand(uint8_t cmd);
    void writeCommand16(uint16_t cmd);
    void writeData(uint8_t data);
    void writeData(uint8_t* data, uint32_t n);
    void writeData16(uint16_t data, uint32_t n = 1);
    void writeData16(const uint16_t* data, uint32_t n);
    void enableBacklight(bool enable = true);
  private:
    int8_t _cs, _rs, _rst, _wr, _rd, _bl; // Control lines
    bool _bl_active_high;
};

#define GFX_TFT_IO_PLUGIN_CLASS GFX_STM32F407VET6

#endif
#endif

#endif
