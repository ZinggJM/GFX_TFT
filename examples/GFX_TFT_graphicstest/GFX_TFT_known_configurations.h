// created by Jean-Marc Zingg to be the GFX_TFT_known_configurations.h io include file for the GFX_TFT library
//
// can be used to select a configuration by uncommenting the corresponding line
//
//#define USE_3i2_TFT_320x240_ILI9341_STM32F103ZET6        // "Generic STM32F1 series", "Generic F103ZE" // ok
//#define USE_3i2_TFT_320x240_ILI9341_STM32F103ZET6_FSMC   // "Generic STM32F1 series", "Generic F103ZE" // ok
//#define USE_3i2_TFT_320x240_ILI9341_STM32F103ZET6M3      // "Generic STM32F1 series", "Generic F103ZE" // ok
//#define USE_3i2_TFT_320x240_ILI9341_STM32F103ZET6M3_FSMC // "Generic STM32F1 series", "Generic F103ZE" // ok
//#define USE_3i2_TFT_320x240_ILI9341_STM32F407VET6        // "Generic STM32F4 series", "Generic F407VE" // ok
//#define USE_3i2_TFT_320x240_ILI9341_STM32F407VET6_FSMC   // "Generic STM32F4 series", "Generic F407VE" // ok
//#define USE_3i2_TFT_320x240_ILI9341_STM32F407VxT6        // "Generic STM32F4 series", "Generic F407VE" // ok
//#define USE_3i2_TFT_320x240_ILI9341_STM32F407VxT6_FSMC   // "Generic STM32F4 series", "Generic F407VE" // ok
//#define USE_3i2_TFT_320x240_ILI9341_STM32F407ZET6        // "Generic STM32F4 series", "Black F407ZE" // ok
//#define USE_3i2_TFT_320x240_ILI9341_STM32F407ZET6_FSMC   // "Generic STM32F4 series", "Black F407ZE" // ok
//#define USE_3i2_TFT_320x240_ILI9341_STM32F407ZGM4        // "Generic STM32F4 series", "Black F407ZG" // ok
//#define USE_3i2_TFT_320x240_ILI9341_STM32F407ZGM4_FSMC   // "Generic STM32F4 series", "Black F407ZG" // ok
//#define USE_3i2_TFT_320x240_ILI9341_Nucleo144            // "Nucleo-144", "Nucleo F767ZI" or "Nucleo F746ZG" // ok
//#define USE_3i2_TFT_320x240_ILI9341_Nucleo144_FSMC       // "Nucleo-144", "Nucleo F767ZI" or "Nucleo F746ZG" // ok
//
//#define USE_3i5_TFT_480x320_ILI9486_STM32F103ZET6M3      // "Generic STM32F1 series", "Generic F103ZE" // ok
//#define USE_3i5_TFT_480x320_ILI9486_STM32F103ZET6M3_FSMC // "Generic STM32F1 series", "Generic F103ZE" // ok
//#define USE_3i5_TFT_480x320_ILI9486_STM32F407VxT6        // "Generic STM32F4 series", "Generic F407VE" // ok
//#define USE_3i5_TFT_480x320_ILI9486_STM32F407VxT6_FSMC   // "Generic STM32F4 series", "Generic F407VE"
//#define USE_3i5_TFT_480x320_ILI9486_STM32F407ZGM4        // "Generic STM32F4 series", "Black F407ZG" // ok
//#define USE_3i5_TFT_480x320_ILI9486_STM32F407ZGM4_FSMC   // "Generic STM32F4 series", "Black F407ZG" // ok
//#define USE_3i97_800x480_NT35510_STM32F407VxT6           // "Generic STM32F4 series", "Generic F407VE" // ok
//#define USE_3i97_800x480_NT35510_STM32F407VxT6_FSMC      // "Generic STM32F4 series", "Generic F407VE" // ok
//#define USE_3i97_800x480_OTM8009A_STM32F407VxT6          // "Generic STM32F4 series", "Generic F407VE" // LDO version needs 5V on pin 24, ok
//#define USE_3i97_800x480_OTM8009A_STM32F407VxT6_FSMC     // "Generic STM32F4 series", "Generic F407VE" // LDO version needs 5V on pin 24, ok
//#define USE_3i97_800x480_OTM8009A_STM32F407ZGM4          // "Generic STM32F4 series", "Black F407ZG" // ok
//#define USE_3i97_800x480_OTM8009A_STM32F407ZGM4_FSMC     // "Generic STM32F4 series", "Black F407ZG" // ok
//#define USE_4i0_480x320_ST7796S_STM32F407VxT6            // "Generic STM32F4 series", "Generic F407VE" // ok
//#define USE_4i0_480x320_ST7796S_STM32F407VxT6_FSMC       // "Generic STM32F4 series", "Generic F407VE" // ok

#ifdef USE_3i2_TFT_320x240_ILI9341_STM32F103ZET6
// tft https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
// on  https://www.aliexpress.com/item/32842677650.html
#include <GFX_ILI9341.h>
#include <GFX_TFT_IO/GFX_STM32F103ZET6.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i2_TFT_320x240_ILI9341_STM32F103ZET6_FSMC
// tft https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
// on  https://www.aliexpress.com/item/32842677650.html
#include <GFX_ILI9341.h>
#include <GFX_TFT_IO/FSMC/GFX_STM32F103ZET6_FSMC.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i2_TFT_320x240_ILI9341_STM32F103ZET6M3
// tft https://www.aliexpress.com/item/32812961761.html
// on  https://www.aliexpress.com/item/32720494327.html
#include <GFX_ILI9341.h>
#include <GFX_TFT_IO/GFX_STM32F103ZET6M3.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i2_TFT_320x240_ILI9341_STM32F103ZET6M3_FSMC
// tft https://www.aliexpress.com/item/32812961761.html
// on  https://www.aliexpress.com/item/32720494327.html
#include <GFX_ILI9341.h>
#include <GFX_TFT_IO/FSMC/GFX_STM32F103ZET6M3_FSMC.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i2_TFT_320x240_ILI9341_STM32F407VET6
// tft https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
// on  https://www.aliexpress.com/item/Free-shipping-STM32F407VET6-development-board-Cortex-M4-STM32-minimum-system-learning-board-ARM-core-board/32618222721.html
#include <GFX_ILI9341.h>
#include <GFX_TFT_IO/GFX_STM32F407VET6.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i2_TFT_320x240_ILI9341_STM32F407VET6_FSMC
// tft https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
// on  https://www.aliexpress.com/item/Free-shipping-STM32F407VET6-development-board-Cortex-M4-STM32-minimum-system-learning-board-ARM-core-board/32618222721.html
#include <GFX_ILI9341.h>
#include <GFX_TFT_IO/FSMC/GFX_STM32F407VET6_FSMC.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i2_TFT_320x240_ILI9341_STM32F407VxT6
// tft https://www.aliexpress.com/item/32812961761.html
// on  https://www.aliexpress.com/item/1005001806399987.html
#include <GFX_ILI9341.h>
#include <GFX_TFT_IO/GFX_STM32F407VxT6.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i2_TFT_320x240_ILI9341_STM32F407VxT6_FSMC
// tft https://www.aliexpress.com/item/32812961761.html
// on  https://www.aliexpress.com/item/1005001806399987.html
#include <GFX_ILI9341.h>
#include <GFX_TFT_IO/FSMC/GFX_STM32F407VxT6_FSMC.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i2_TFT_320x240_ILI9341_STM32F407ZET6
// tft https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
// on  https://www.aliexpress.com/item/32689262341.html
#include <GFX_ILI9341.h>
#include <GFX_TFT_IO/GFX_STM32F407ZET6.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i2_TFT_320x240_ILI9341_STM32F407ZET6_FSMC
// tft https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
// on  https://www.aliexpress.com/item/32689262341.html
#include <GFX_ILI9341.h>
#include <GFX_TFT_IO/FSMC/GFX_STM32F407ZET6_FSMC.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i2_TFT_320x240_ILI9341_STM32F407ZGM4
// tft https://www.aliexpress.com/item/32812961761.html
// on  https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
#include <GFX_ILI9341.h>
#include <GFX_TFT_IO/GFX_STM32F407ZGM4.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i2_TFT_320x240_ILI9341_STM32F407ZGM4_FSMC
// tft https://www.aliexpress.com/item/32812961761.html
// on  https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
#include <GFX_ILI9341.h>
#include <GFX_TFT_IO/FSMC/GFX_STM32F407ZGM4_FSMC.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i2_TFT_320x240_ILI9341_Nucleo144
// tft https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
// on my Nucleo 144 proto board
#include <GFX_ILI9341.h>
#include <GFX_TFT_IO/GFX_STM32Nucleo144.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i2_TFT_320x240_ILI9341_Nucleo144_FSMC
// tft https://www.aliexpress.com/item/3-2-inch-TFT-LCD-screen-with-resistive-touch-screens-ILI9341-display-module/32662835059.html
// on my Nucleo 144 proto board
#include <GFX_ILI9341.h>
#include <GFX_TFT_IO/FSMC/GFX_STM32Nucleo144_FSMC.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i5_TFT_480x320_ILI9486_STM32F103ZET6M3
// tft https://www.aliexpress.com/item/Smart-Electronics-3-5-inch-TFT-Touch-Screen-LCD-Module-Display-320-480-ILI9486-with-PCB/32586941686.html
// on  https://www.aliexpress.com/item/32720494327.html
#include <GFX_ILI9486.h>
#include <GFX_TFT_IO/GFX_STM32F103ZET6M3.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i5_TFT_480x320_ILI9486_STM32F103ZET6M3_FSMC
// tft https://www.aliexpress.com/item/Smart-Electronics-3-5-inch-TFT-Touch-Screen-LCD-Module-Display-320-480-ILI9486-with-PCB/32586941686.html
// on  https://www.aliexpress.com/item/32720494327.html
#include <GFX_ILI9486.h>
#include <GFX_TFT_IO/FSMC/GFX_STM32F103ZET6M3_FSMC.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i5_TFT_480x320_ILI9486_STM32F407VxT6
// tft https://www.aliexpress.com/item/Smart-Electronics-3-5-inch-TFT-Touch-Screen-LCD-Module-Display-320-480-ILI9486-with-PCB/32586941686.html
// on  https://www.aliexpress.com/item/1005001806399987.html
#include <GFX_ILI9486.h>
#include <GFX_TFT_IO/GFX_STM32F407VxT6.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i5_TFT_480x320_ILI9486_STM32F407VxT6_FSMC
// tft https://www.aliexpress.com/item/Smart-Electronics-3-5-inch-TFT-Touch-Screen-LCD-Module-Display-320-480-ILI9486-with-PCB/32586941686.html
// on  https://www.aliexpress.com/item/1005001806399987.html
#include <GFX_ILI9486.h>
#include <GFX_TFT_IO/FSMC/GFX_STM32F407VxT6_FSMC.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i5_TFT_480x320_ILI9486_STM32F407ZGM4
// tft https://www.aliexpress.com/item/Smart-Electronics-3-5-inch-TFT-Touch-Screen-LCD-Module-Display-320-480-ILI9486-with-PCB/32586941686.html
// on  https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
#include <GFX_ILI9486.h>
#include <GFX_TFT_IO/GFX_STM32F407ZGM4.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i5_TFT_480x320_ILI9486_STM32F407ZGM4_FSMC
// tft https://www.aliexpress.com/item/Smart-Electronics-3-5-inch-TFT-Touch-Screen-LCD-Module-Display-320-480-ILI9486-with-PCB/32586941686.html
// on  https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
#include <GFX_ILI9486.h>
#include <GFX_TFT_IO/FSMC/GFX_STM32F407ZGM4_FSMC.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i97_800x480_NT35510_STM32F407VxT6
// tft https://www.aliexpress.com/item/32676929794.html "maithoga IPS 3.97 inch 34P 16.7M HD TFT LCD Screen with Adapter Board (Touch/No Touch) NT35510 Drive IC 480(RGB)*800" display from Ruijia Industry
// on  https://www.aliexpress.com/item/1005001806399987.html STM32F407VxT6 board
#include <GFX_NT35510.h>
#include <GFX_TFT_IO/GFX_STM32F407VxT6.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i97_800x480_NT35510_STM32F407VxT6_FSMC
// tft https://www.aliexpress.com/item/32676929794.html "maithoga IPS 3.97 inch 34P 16.7M HD TFT LCD Screen with Adapter Board (Touch/No Touch) NT35510 Drive IC 480(RGB)*800" display from Ruijia Industry
// on  https://www.aliexpress.com/item/1005001806399987.html STM32F407VxT6 board
#include <GFX_NT35510.h>
#include <GFX_TFT_IO/FSMC/GFX_STM32F407VxT6_FSMC.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i97_800x480_OTM8009A_STM32F407VxT6
// tft https://www.aliexpress.com/item/IPS-3-97-inch-HD-TFT-LCD-Touch-Screen-Module-OTM8009A-Drive-IC-800-480/32676929794.html replaced by NT35510 version
// on  https://www.aliexpress.com/item/1005001806399987.html STM32F407VxT6 board
#include <GFX_OTM8009A.h>
#include <GFX_TFT_IO/GFX_STM32F407VxT6.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i97_800x480_OTM8009A_STM32F407VxT6_FSMC
// tft https://www.aliexpress.com/item/IPS-3-97-inch-HD-TFT-LCD-Touch-Screen-Module-OTM8009A-Drive-IC-800-480/32676929794.html replaced by NT35510 version
// on  https://www.aliexpress.com/item/1005001806399987.html STM32F407VxT6 board
#include <GFX_OTM8009A.h>
#include <GFX_TFT_IO/FSMC/GFX_STM32F407VxT6_FSMC.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i97_800x480_OTM8009A_STM32F407ZGM4
// tft https://www.aliexpress.com/item/IPS-3-97-inch-HD-TFT-LCD-Touch-Screen-Module-OTM8009A-Drive-IC-800-480/32676929794.html replaced by NT35510 version
// on  https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
#include <GFX_OTM8009A.h>
#include <GFX_TFT_IO/GFX_STM32F407ZGM4.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_3i97_800x480_OTM8009A_STM32F407ZGM4_FSMC
// tft https://www.aliexpress.com/item/IPS-3-97-inch-HD-TFT-LCD-Touch-Screen-Module-OTM8009A-Drive-IC-800-480/32676929794.html replaced by NT35510 version
// on  https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
#include <GFX_OTM8009A.h>
#include <GFX_TFT_IO/FSMC/GFX_STM32F407ZGM4_FSMC.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_4i0_480x320_ST7796S_STM32F407VxT6
// tft https://www.aliexpress.com/item/1899444540.html "4.0 inch HD TFT LCD Screen with PCB Board ILI9488 ST7796S Drive IC 320(RGB)*480 8Bit Parallel Interface (Touch/No Touch)" Maithoga
// on  https://www.aliexpress.com/item/1005001806399987.html STM32F407VxT6 board
#include <GFX_ST7796S.h>
#include <GFX_TFT_IO/GFX_STM32F407VxT6.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif

#ifdef USE_4i0_480x320_ST7796S_STM32F407VxT6_FSMC
// tft https://www.aliexpress.com/item/1899444540.html "4.0 inch HD TFT LCD Screen with PCB Board ILI9488 ST7796S Drive IC 320(RGB)*480 8Bit Parallel Interface (Touch/No Touch)" Maithoga
// on  https://www.aliexpress.com/item/1005001806399987.html STM32F407VxT6 board
#include <GFX_ST7796S.h>
#include <GFX_TFT_IO/FSMC/GFX_STM32F407VxT6_FSMC.h>
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter
#endif
