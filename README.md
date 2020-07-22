## TFT Display Classes based on GFX_ROOT and GFX_Extensions

This library supports some TFTs using GFX_IO (my Adafruit_SPITFT clone)

### Version 1.0.1
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