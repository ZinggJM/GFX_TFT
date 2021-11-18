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

#ifndef _GFX_STM32F103ZET6_H_
#define _GFX_STM32F103ZET6_H_

#include "GFX_TFT_IO_Plugin.h"

#if defined(ARDUINO_ARCH_STM32)&& defined(STM32F1xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_GENERIC_F103ZETX)

class GFX_STM32F103ZET6 : public GFX_TFT_IO_Plugin
{
  public:
    GFX_STM32F103ZET6();
    const char* name = "GFX_STM32F103ZET6";
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
};

#define GFX_TFT_IO_PLUGIN_CLASS GFX_STM32F103ZET6

#endif
#endif

#endif
