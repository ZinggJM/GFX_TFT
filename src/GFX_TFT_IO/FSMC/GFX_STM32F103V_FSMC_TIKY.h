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

#ifndef _GxIO_STM32GENERIC_STM32F1_FSMC_H_
#define _GxIO_STM32GENERIC_STM32F1_FSMC_H_

#include "../GFX_TFT_IO_Plugin.h"

#if defined(ARDUINO_ARCH_STM32)&& defined(STM32F1xx) && !defined(STM32GENERIC) // "STM32 Boards (select from submenu)"
#if defined(ARDUINO_GENERIC_F103VCTX) || defined(ARDUINO_GENERIC_F103VDTX) || defined(ARDUINO_GENERIC_F103VETX) || defined(ARDUINO_GENERIC_F103VFTX) || defined(ARDUINO_GENERIC_F103VGTX)

class GFX_STM32F103V_FSMC_TIKY : public GFX_TFT_IO_Plugin
{
  public:
    GFX_STM32F103V_FSMC_TIKY(bool bl_active_high = true);
    const char* name = "GFX_STM32F103V_FSMC_TIKY";
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

#define GFX_TFT_IO_PLUGIN_CLASS GFX_STM32F103V_FSMC_TIKY

#endif
#endif

#endif
