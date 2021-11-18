## TFT Display Classes based on GFX_ROOT and GFX_Extensions

This library supports some TFTs using GFX_IO (my Adafruit_SPITFT clone)

### Version 1.0.6
- fix for STM32 official package pin number range (int16_t)
- update of conditional compile conditions to macros of actual STM32 package
#### Version 1.0.5
- added 18-bit color handling for native SPI use on ILI9486
#### Version 1.0.4
- renamed SPI display classes GFX_ILI9486, GFX_ILI9488 to GFX_SPI_ILI9486, GFX_SPI_ILI9488
- added display classes GFX_ILI9486, GFX_ILI9488, GFX_NT35510, GFX_ST7796S
- added IO plugins GFX_STM32F103ZET6 and GFX_STM32F103ZET6_FSMC
- added IO plugins GFX_STM32F103ZET6M3 and GFX_STM32F103ZET6M3
- added IO plugins GFX_STM32F407VxT6 and GFX_STM32F407VxT6_FSMC
- added IO plugins GFX_STM32F407ZGM4 and GFX_STM32F407ZGM4_FSMC
- added GFX_TFT_known_configurations.h to example GFX_TFT_graphicstest
#### Version 1.0.3
- added IO plugins GFX_STM32Nucleo144 and GFX_STM32Nucleo144_FSMC
- added IO plugins GFX_STM32F407VET6 and GFX_STM32F407VET6_FSMC
- added IO plugins GFX_STM32F407ZET6 and GFX_STM32F407ZET6_FSMC
#### Version 1.0.2
- work in progress version(s), using same version number
- adding controllers and io plugins from GxTFT
- added IO plugins GFX_STM32F103C and GFX_STM32F103C_TIKY
- added IO plugins GFX_STM32F103V_TIKY and GFX_STM32F103V_FSMC_TIKY
- added display class GFX_ILI9806
- added display class GFX_SSD1963
#### Version 1.0.1
- added option for IO driver plugins
- added IO driver plugin GFX_STM32F407ZGM4_FSMC (for package "STM32 Cores by STMicroelectronics")
- added display class GFX_ILI9341
- added display class GFX_OTM8009A
- added example GFX_TFT_graphicstest.ino
#### Version 1.0.0
- initial version with some SPI TFTs taken from my ZxTFT library
- example GFX_SPI_TFT_graphicstest.ino
- SSD1283A, e.g. https://www.aliexpress.com/item/4000116854157.html
- ILI9486, e.g. https://www.aliexpress.com/item/32661117216.html
- ILI9488, e.g. https://www.aliexpress.com/item/32908809356.html