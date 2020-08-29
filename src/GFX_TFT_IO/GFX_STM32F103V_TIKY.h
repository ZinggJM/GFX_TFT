// created by Jean-Marc Zingg to be the GFX_STM32F103V_TIKY io class for the GFX_TFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is a special wiring for
// https://www.aliexpress.com/item/5-0-inch-HD-IPS-TFT-LCD-module-resistance-touch-with-PCB-adapter-board-854-480/32666829945.html
//
// for the STM32F103V board with matching TFT connector (FSMC pins)
// https://www.aliexpress.com/item/STM32-core-development-TFT-LCD-screen-evaluation-board-with-high-speed-FSMC-SDIO-interface/32667841009.html
//
// for pin information see the backside of the TFT, for the data pin to port pin mapping see FSMC pin table STM32F103V doc.
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), board "Generic STM32F1 series" part "GENERIC F103VC".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#ifndef _GFX_STM32F103V_TIKY_H_
#define _GFX_STM32F103V_TIKY_H_

#include "GFX_TFT_IO_Plugin.h"

#if defined(ARDUINO_ARCH_STM32)&& defined(STM32F1xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_GENERIC_F103VC) || defined(ARDUINO_GENERIC_F103VD) || defined(ARDUINO_GENERIC_F103VE) || defined(ARDUINO_GENERIC_F103VF)

class GFX_STM32F103V_TIKY : public GFX_TFT_IO_Plugin
{
  public:
    GFX_STM32F103V_TIKY(bool bl_active_high = true);
    const char* name = "GFX_STM32F103V_TIKY";
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
    int8_t _cs, _rs, _rst, _wr, _rd, _bl; // Control lines
    bool _bl_active_high;
};

#define GFX_TFT_IO_PLUGIN_CLASS GFX_STM32F103V_TIKY

#endif
#endif

#endif
