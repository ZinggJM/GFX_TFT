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

#ifndef _GFX_STM32F103C_H_
#define _GFX_STM32F103C_H_

#include "GFX_TFT_IO_Plugin.h"

#if defined(ARDUINO_ARCH_STM32)&& defined(STM32F1xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"

class GFX_STM32F103C : public GFX_TFT_IO_Plugin
{
  public:
    GFX_STM32F103C(bool bl_active_high = true);
    const char* name = "GFX_STM32F103C";
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
    void setDataPinsOutput();
    void setDataPinsInput();
    int16_t _cs, _rs, _rst, _wr, _rd, _bl; // Control lines
    bool _bl_active_high;
};

#define GFX_TFT_IO_PLUGIN_CLASS GFX_STM32F103C

#endif

#endif
