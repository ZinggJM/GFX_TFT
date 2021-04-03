// created by Jean-Marc Zingg to be the GFX_STM32F103ZET6_FSMC io class for the GFX_TFT library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this is the io class for STM32F103ZE boards with FMSC TFT connector, e.g. for 
// https://www.aliexpress.com/item/32842677650.html
// for details of the board see https://stm32-base.org/boards/STM32F103ZET6-F103ZE-Board.html
//
// the above board has matched TFT connector for this display
// https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
//
// for pin information see the backside of the TFT, for the data pin to port pin mapping see FSMC pin table STM32F103Z doc.
//
// this io class can be used with or adapted to other STM32F103Z processor boards with FSMC TFT connector.
//
// this version is for use with Arduino package "STM32 Boards (select from submenu)" (STMicroelectronics), board "Generic STM32F1 series" part "Generic F103ZE".
// preferences Additional Boards Manager URLs https://raw.githubusercontent.com/stm32duino/BoardManagerFiles/master/STM32/package_stm_index.json

#ifndef _GxIO_STM32GENERIC_STM32F1_FSMC_H_
#define _GxIO_STM32GENERIC_STM32F1_FSMC_H_

#include "../GFX_TFT_IO_Plugin.h"

#if defined(ARDUINO_ARCH_STM32)&& defined(STM32F1xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_Generic_F103ZE)

class GFX_STM32F103ZET6_FSMC : public GFX_TFT_IO_Plugin
{
  public:
    GFX_STM32F103ZET6_FSMC();
    const char* name = "GFX_STM32F103ZET6_FSMC";
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
};

#define GFX_TFT_IO_PLUGIN_CLASS GFX_STM32F103ZET6_FSMC

#endif
#endif

#endif
