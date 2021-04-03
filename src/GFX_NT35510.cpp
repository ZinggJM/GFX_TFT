// created by Jean-Marc Zingg to be the GFX_NT35510 class for the GFX_TFT library
// code extracts taken from http://www.lcdwiki.com/res/Program/Parallel_Port/3.97inch/8_16BIT_NT35510_800x480_MRB3973_V1.2/3.97inch_8&16BIT_Module_NT35510_800x480_MRB3973_V1.2.zip (lcd.h, lcd.c)
// code extracts taken from https://github.com/adafruit/Adafruit-GFX-Library
//
// License: GNU GENERAL PUBLIC LICENSE V3, see LICENSE
//
// this class works with "maithoga IPS 3.97 inch 34P 16.7M HD TFT LCD Screen with Adapter Board (Touch/No Touch) NT35510 Drive IC 480(RGB)*800" display from Ruijia Industry
// e.g. https://www.aliexpress.com/item/32676929794.html
// this display matches the FSMC TFT connector of the STM32F407ZG-M4 board
// e.g. https://www.aliexpress.com/item/STM32F407ZGT6-Development-Board-ARM-M4-STM32F4-cortex-M4-core-Board-Compatibility-Multiple-Extension/32795142050.html
// this display also matches the FSMC TFT connector of the STM32F407VxT6 board e.g. https://www.aliexpress.com/item/1005001806399987.html
//
// note: this display class uses 16bit commands, for 16 bit solder jumper settings

#include "GFX_NT35510.h"

#define SPI_SPEED 40000000

#define MADCTL     0x3600
#define MADCTL_MY  0x80
#define MADCTL_MX  0x40
#define MADCTL_MV  0x20
#define MADCTL_ML  0x10
#define MADCTL_RGB 0x00
#define MADCTL_BGR 0x08

#define RGB_or_BGR 0x00 // RGB

GFX_NT35510::GFX_NT35510(int8_t cs_pin, int8_t dc_pin, int8_t rst_pin, int8_t bl_pin) :
#if defined(ESP8266)
  GFX_TFT_IO(480, 800, cs_pin, dc_pin, rst_pin)
#else
  GFX_TFT_IO(480, 800, &SPI, cs_pin, dc_pin, rst_pin)
#endif
{
  _bl_pin = bl_pin;
  _bgr = RGB_or_BGR;
}

GFX_NT35510::GFX_NT35510(int8_t cs_pin, int8_t dc_pin, int8_t mosi_pin, int8_t sclk_pin, int8_t rst_pin, int8_t bl_pin) :
  GFX_TFT_IO(480, 800, cs_pin, dc_pin, mosi_pin, sclk_pin, rst_pin, -1)
{
  _bl_pin = bl_pin;
  _bgr = RGB_or_BGR;
}

GFX_NT35510::GFX_NT35510(SPIClass *spi, int8_t cs_pin, int8_t dc_pin, int8_t rst_pin, int8_t bl_pin) :
#if defined(ESP8266)
  GFX_TFT_IO(480, 800, cs_pin, dc_pin, rst_pin)
#else
  GFX_TFT_IO(480, 800, spi, cs_pin, dc_pin, rst_pin)
#endif
{
  (void) spi;
  _bl_pin = bl_pin;
  _bgr = RGB_or_BGR;
}

GFX_NT35510::GFX_NT35510(GFX_TFT_IO_Plugin& plugin) : GFX_TFT_IO(480, 800, &plugin)
{
  _bl_pin = -1;
  _bgr = RGB_or_BGR;
}

void GFX_NT35510::begin(uint32_t freq)
{
  init(freq);
}

void GFX_NT35510::init(uint32_t freq)
{
  Serial.println("init");
  if (!freq) freq = SPI_SPEED;
  initSPI(freq);
  startWrite();
  delay(20);
  //************* NT35510��ʼ��**********//
  writeCommand16(0xF000); writeData16(0x55);
  writeCommand16(0xF001); writeData16(0xAA);
  writeCommand16(0xF002); writeData16(0x52);
  writeCommand16(0xF003); writeData16(0x08);
  writeCommand16(0xF004); writeData16(0x01);
  //# AVDD: manual); writeData16(
  writeCommand16(0xB600); writeData16(0x34);
  writeCommand16(0xB601); writeData16(0x34);
  writeCommand16(0xB602); writeData16(0x34);

  writeCommand16(0xB000); writeData16(0x0D);//09
  writeCommand16(0xB001); writeData16(0x0D);
  writeCommand16(0xB002); writeData16(0x0D);
  //# AVEE: manual); writeData16( -6V
  writeCommand16(0xB700); writeData16(0x24);
  writeCommand16(0xB701); writeData16(0x24);
  writeCommand16(0xB702); writeData16(0x24);

  writeCommand16(0xB100); writeData16(0x0D);
  writeCommand16(0xB101); writeData16(0x0D);
  writeCommand16(0xB102); writeData16(0x0D);
  //#Power Control for
  //VCL
  writeCommand16(0xB800); writeData16(0x24);
  writeCommand16(0xB801); writeData16(0x24);
  writeCommand16(0xB802); writeData16(0x24);

  writeCommand16(0xB200); writeData16(0x00);

  //# VGH: Clamp Enable); writeData16(
  writeCommand16(0xB900); writeData16(0x24);
  writeCommand16(0xB901); writeData16(0x24);
  writeCommand16(0xB902); writeData16(0x24);

  writeCommand16(0xB300); writeData16(0x05);
  writeCommand16(0xB301); writeData16(0x05);
  writeCommand16(0xB302); writeData16(0x05);

  ///writeCommand16(0xBF00); writeData16(0x01);

  //# VGL(LVGL):
  writeCommand16(0xBA00); writeData16(0x34);
  writeCommand16(0xBA01); writeData16(0x34);
  writeCommand16(0xBA02); writeData16(0x34);
  //# VGL_REG(VGLO)
  writeCommand16(0xB500); writeData16(0x0B);
  writeCommand16(0xB501); writeData16(0x0B);
  writeCommand16(0xB502); writeData16(0x0B);
  //# VGMP/VGSP:
  writeCommand16(0xBC00); writeData16(0X00);
  writeCommand16(0xBC01); writeData16(0xA3);
  writeCommand16(0xBC02); writeData16(0X00);
  //# VGMN/VGSN
  writeCommand16(0xBD00); writeData16(0x00);
  writeCommand16(0xBD01); writeData16(0xA3);
  writeCommand16(0xBD02); writeData16(0x00);
  //# VCOM=-0.1
  writeCommand16(0xBE00); writeData16(0x00);
  writeCommand16(0xBE01); writeData16(0x63);//4f
  //  VCOMH+0x01;
  //#R+
  writeCommand16(0xD100); writeData16(0x00);
  writeCommand16(0xD101); writeData16(0x37);
  writeCommand16(0xD102); writeData16(0x00);
  writeCommand16(0xD103); writeData16(0x52);
  writeCommand16(0xD104); writeData16(0x00);
  writeCommand16(0xD105); writeData16(0x7B);
  writeCommand16(0xD106); writeData16(0x00);
  writeCommand16(0xD107); writeData16(0x99);
  writeCommand16(0xD108); writeData16(0x00);
  writeCommand16(0xD109); writeData16(0xB1);
  writeCommand16(0xD10A); writeData16(0x00);
  writeCommand16(0xD10B); writeData16(0xD2);
  writeCommand16(0xD10C); writeData16(0x00);
  writeCommand16(0xD10D); writeData16(0xF6);
  writeCommand16(0xD10E); writeData16(0x01);
  writeCommand16(0xD10F); writeData16(0x27);
  writeCommand16(0xD110); writeData16(0x01);
  writeCommand16(0xD111); writeData16(0x4E);
  writeCommand16(0xD112); writeData16(0x01);
  writeCommand16(0xD113); writeData16(0x8C);
  writeCommand16(0xD114); writeData16(0x01);
  writeCommand16(0xD115); writeData16(0xBE);
  writeCommand16(0xD116); writeData16(0x02);
  writeCommand16(0xD117); writeData16(0x0B);
  writeCommand16(0xD118); writeData16(0x02);
  writeCommand16(0xD119); writeData16(0x48);
  writeCommand16(0xD11A); writeData16(0x02);
  writeCommand16(0xD11B); writeData16(0x4A);
  writeCommand16(0xD11C); writeData16(0x02);
  writeCommand16(0xD11D); writeData16(0x7E);
  writeCommand16(0xD11E); writeData16(0x02);
  writeCommand16(0xD11F); writeData16(0xBC);
  writeCommand16(0xD120); writeData16(0x02);
  writeCommand16(0xD121); writeData16(0xE1);
  writeCommand16(0xD122); writeData16(0x03);
  writeCommand16(0xD123); writeData16(0x10);
  writeCommand16(0xD124); writeData16(0x03);
  writeCommand16(0xD125); writeData16(0x31);
  writeCommand16(0xD126); writeData16(0x03);
  writeCommand16(0xD127); writeData16(0x5A);
  writeCommand16(0xD128); writeData16(0x03);
  writeCommand16(0xD129); writeData16(0x73);
  writeCommand16(0xD12A); writeData16(0x03);
  writeCommand16(0xD12B); writeData16(0x94);
  writeCommand16(0xD12C); writeData16(0x03);
  writeCommand16(0xD12D); writeData16(0x9F);
  writeCommand16(0xD12E); writeData16(0x03);
  writeCommand16(0xD12F); writeData16(0xB3);
  writeCommand16(0xD130); writeData16(0x03);
  writeCommand16(0xD131); writeData16(0xB9);
  writeCommand16(0xD132); writeData16(0x03);
  writeCommand16(0xD133); writeData16(0xC1);
  //#G+
  writeCommand16(0xD200); writeData16(0x00);
  writeCommand16(0xD201); writeData16(0x37);
  writeCommand16(0xD202); writeData16(0x00);
  writeCommand16(0xD203); writeData16(0x52);
  writeCommand16(0xD204); writeData16(0x00);
  writeCommand16(0xD205); writeData16(0x7B);
  writeCommand16(0xD206); writeData16(0x00);
  writeCommand16(0xD207); writeData16(0x99);
  writeCommand16(0xD208); writeData16(0x00);
  writeCommand16(0xD209); writeData16(0xB1);
  writeCommand16(0xD20A); writeData16(0x00);
  writeCommand16(0xD20B); writeData16(0xD2);
  writeCommand16(0xD20C); writeData16(0x00);
  writeCommand16(0xD20D); writeData16(0xF6);
  writeCommand16(0xD20E); writeData16(0x01);
  writeCommand16(0xD20F); writeData16(0x27);
  writeCommand16(0xD210); writeData16(0x01);
  writeCommand16(0xD211); writeData16(0x4E);
  writeCommand16(0xD212); writeData16(0x01);
  writeCommand16(0xD213); writeData16(0x8C);
  writeCommand16(0xD214); writeData16(0x01);
  writeCommand16(0xD215); writeData16(0xBE);
  writeCommand16(0xD216); writeData16(0x02);
  writeCommand16(0xD217); writeData16(0x0B);
  writeCommand16(0xD218); writeData16(0x02);
  writeCommand16(0xD219); writeData16(0x48);
  writeCommand16(0xD21A); writeData16(0x02);
  writeCommand16(0xD21B); writeData16(0x4A);
  writeCommand16(0xD21C); writeData16(0x02);
  writeCommand16(0xD21D); writeData16(0x7E);
  writeCommand16(0xD21E); writeData16(0x02);
  writeCommand16(0xD21F); writeData16(0xBC);
  writeCommand16(0xD220); writeData16(0x02);
  writeCommand16(0xD221); writeData16(0xE1);
  writeCommand16(0xD222); writeData16(0x03);
  writeCommand16(0xD223); writeData16(0x10);
  writeCommand16(0xD224); writeData16(0x03);
  writeCommand16(0xD225); writeData16(0x31);
  writeCommand16(0xD226); writeData16(0x03);
  writeCommand16(0xD227); writeData16(0x5A);
  writeCommand16(0xD228); writeData16(0x03);
  writeCommand16(0xD229); writeData16(0x73);
  writeCommand16(0xD22A); writeData16(0x03);
  writeCommand16(0xD22B); writeData16(0x94);
  writeCommand16(0xD22C); writeData16(0x03);
  writeCommand16(0xD22D); writeData16(0x9F);
  writeCommand16(0xD22E); writeData16(0x03);
  writeCommand16(0xD22F); writeData16(0xB3);
  writeCommand16(0xD230); writeData16(0x03);
  writeCommand16(0xD231); writeData16(0xB9);
  writeCommand16(0xD232); writeData16(0x03);
  writeCommand16(0xD233); writeData16(0xC1);
  //#B+
  writeCommand16(0xD300); writeData16(0x00);
  writeCommand16(0xD301); writeData16(0x37);
  writeCommand16(0xD302); writeData16(0x00);
  writeCommand16(0xD303); writeData16(0x52);
  writeCommand16(0xD304); writeData16(0x00);
  writeCommand16(0xD305); writeData16(0x7B);
  writeCommand16(0xD306); writeData16(0x00);
  writeCommand16(0xD307); writeData16(0x99);
  writeCommand16(0xD308); writeData16(0x00);
  writeCommand16(0xD309); writeData16(0xB1);
  writeCommand16(0xD30A); writeData16(0x00);
  writeCommand16(0xD30B); writeData16(0xD2);
  writeCommand16(0xD30C); writeData16(0x00);
  writeCommand16(0xD30D); writeData16(0xF6);
  writeCommand16(0xD30E); writeData16(0x01);
  writeCommand16(0xD30F); writeData16(0x27);
  writeCommand16(0xD310); writeData16(0x01);
  writeCommand16(0xD311); writeData16(0x4E);
  writeCommand16(0xD312); writeData16(0x01);
  writeCommand16(0xD313); writeData16(0x8C);
  writeCommand16(0xD314); writeData16(0x01);
  writeCommand16(0xD315); writeData16(0xBE);
  writeCommand16(0xD316); writeData16(0x02);
  writeCommand16(0xD317); writeData16(0x0B);
  writeCommand16(0xD318); writeData16(0x02);
  writeCommand16(0xD319); writeData16(0x48);
  writeCommand16(0xD31A); writeData16(0x02);
  writeCommand16(0xD31B); writeData16(0x4A);
  writeCommand16(0xD31C); writeData16(0x02);
  writeCommand16(0xD31D); writeData16(0x7E);
  writeCommand16(0xD31E); writeData16(0x02);
  writeCommand16(0xD31F); writeData16(0xBC);
  writeCommand16(0xD320); writeData16(0x02);
  writeCommand16(0xD321); writeData16(0xE1);
  writeCommand16(0xD322); writeData16(0x03);
  writeCommand16(0xD323); writeData16(0x10);
  writeCommand16(0xD324); writeData16(0x03);
  writeCommand16(0xD325); writeData16(0x31);
  writeCommand16(0xD326); writeData16(0x03);
  writeCommand16(0xD327); writeData16(0x5A);
  writeCommand16(0xD328); writeData16(0x03);
  writeCommand16(0xD329); writeData16(0x73);
  writeCommand16(0xD32A); writeData16(0x03);
  writeCommand16(0xD32B); writeData16(0x94);
  writeCommand16(0xD32C); writeData16(0x03);
  writeCommand16(0xD32D); writeData16(0x9F);
  writeCommand16(0xD32E); writeData16(0x03);
  writeCommand16(0xD32F); writeData16(0xB3);
  writeCommand16(0xD330); writeData16(0x03);
  writeCommand16(0xD331); writeData16(0xB9);
  writeCommand16(0xD332); writeData16(0x03);
  writeCommand16(0xD333); writeData16(0xC1);

  //#R-///////////////////////////////////////////
  writeCommand16(0xD400); writeData16(0x00);
  writeCommand16(0xD401); writeData16(0x37);
  writeCommand16(0xD402); writeData16(0x00);
  writeCommand16(0xD403); writeData16(0x52);
  writeCommand16(0xD404); writeData16(0x00);
  writeCommand16(0xD405); writeData16(0x7B);
  writeCommand16(0xD406); writeData16(0x00);
  writeCommand16(0xD407); writeData16(0x99);
  writeCommand16(0xD408); writeData16(0x00);
  writeCommand16(0xD409); writeData16(0xB1);
  writeCommand16(0xD40A); writeData16(0x00);
  writeCommand16(0xD40B); writeData16(0xD2);
  writeCommand16(0xD40C); writeData16(0x00);
  writeCommand16(0xD40D); writeData16(0xF6);
  writeCommand16(0xD40E); writeData16(0x01);
  writeCommand16(0xD40F); writeData16(0x27);
  writeCommand16(0xD410); writeData16(0x01);
  writeCommand16(0xD411); writeData16(0x4E);
  writeCommand16(0xD412); writeData16(0x01);
  writeCommand16(0xD413); writeData16(0x8C);
  writeCommand16(0xD414); writeData16(0x01);
  writeCommand16(0xD415); writeData16(0xBE);
  writeCommand16(0xD416); writeData16(0x02);
  writeCommand16(0xD417); writeData16(0x0B);
  writeCommand16(0xD418); writeData16(0x02);
  writeCommand16(0xD419); writeData16(0x48);
  writeCommand16(0xD41A); writeData16(0x02);
  writeCommand16(0xD41B); writeData16(0x4A);
  writeCommand16(0xD41C); writeData16(0x02);
  writeCommand16(0xD41D); writeData16(0x7E);
  writeCommand16(0xD41E); writeData16(0x02);
  writeCommand16(0xD41F); writeData16(0xBC);
  writeCommand16(0xD420); writeData16(0x02);
  writeCommand16(0xD421); writeData16(0xE1);
  writeCommand16(0xD422); writeData16(0x03);
  writeCommand16(0xD423); writeData16(0x10);
  writeCommand16(0xD424); writeData16(0x03);
  writeCommand16(0xD425); writeData16(0x31);
  writeCommand16(0xD426); writeData16(0x03);
  writeCommand16(0xD427); writeData16(0x5A);
  writeCommand16(0xD428); writeData16(0x03);
  writeCommand16(0xD429); writeData16(0x73);
  writeCommand16(0xD42A); writeData16(0x03);
  writeCommand16(0xD42B); writeData16(0x94);
  writeCommand16(0xD42C); writeData16(0x03);
  writeCommand16(0xD42D); writeData16(0x9F);
  writeCommand16(0xD42E); writeData16(0x03);
  writeCommand16(0xD42F); writeData16(0xB3);
  writeCommand16(0xD430); writeData16(0x03);
  writeCommand16(0xD431); writeData16(0xB9);
  writeCommand16(0xD432); writeData16(0x03);
  writeCommand16(0xD433); writeData16(0xC1);

  //#G-//////////////////////////////////////////////
  writeCommand16(0xD500); writeData16(0x00);
  writeCommand16(0xD501); writeData16(0x37);
  writeCommand16(0xD502); writeData16(0x00);
  writeCommand16(0xD503); writeData16(0x52);
  writeCommand16(0xD504); writeData16(0x00);
  writeCommand16(0xD505); writeData16(0x7B);
  writeCommand16(0xD506); writeData16(0x00);
  writeCommand16(0xD507); writeData16(0x99);
  writeCommand16(0xD508); writeData16(0x00);
  writeCommand16(0xD509); writeData16(0xB1);
  writeCommand16(0xD50A); writeData16(0x00);
  writeCommand16(0xD50B); writeData16(0xD2);
  writeCommand16(0xD50C); writeData16(0x00);
  writeCommand16(0xD50D); writeData16(0xF6);
  writeCommand16(0xD50E); writeData16(0x01);
  writeCommand16(0xD50F); writeData16(0x27);
  writeCommand16(0xD510); writeData16(0x01);
  writeCommand16(0xD511); writeData16(0x4E);
  writeCommand16(0xD512); writeData16(0x01);
  writeCommand16(0xD513); writeData16(0x8C);
  writeCommand16(0xD514); writeData16(0x01);
  writeCommand16(0xD515); writeData16(0xBE);
  writeCommand16(0xD516); writeData16(0x02);
  writeCommand16(0xD517); writeData16(0x0B);
  writeCommand16(0xD518); writeData16(0x02);
  writeCommand16(0xD519); writeData16(0x48);
  writeCommand16(0xD51A); writeData16(0x02);
  writeCommand16(0xD51B); writeData16(0x4A);
  writeCommand16(0xD51C); writeData16(0x02);
  writeCommand16(0xD51D); writeData16(0x7E);
  writeCommand16(0xD51E); writeData16(0x02);
  writeCommand16(0xD51F); writeData16(0xBC);
  writeCommand16(0xD520); writeData16(0x02);
  writeCommand16(0xD521); writeData16(0xE1);
  writeCommand16(0xD522); writeData16(0x03);
  writeCommand16(0xD523); writeData16(0x10);
  writeCommand16(0xD524); writeData16(0x03);
  writeCommand16(0xD525); writeData16(0x31);
  writeCommand16(0xD526); writeData16(0x03);
  writeCommand16(0xD527); writeData16(0x5A);
  writeCommand16(0xD528); writeData16(0x03);
  writeCommand16(0xD529); writeData16(0x73);
  writeCommand16(0xD52A); writeData16(0x03);
  writeCommand16(0xD52B); writeData16(0x94);
  writeCommand16(0xD52C); writeData16(0x03);
  writeCommand16(0xD52D); writeData16(0x9F);
  writeCommand16(0xD52E); writeData16(0x03);
  writeCommand16(0xD52F); writeData16(0xB3);
  writeCommand16(0xD530); writeData16(0x03);
  writeCommand16(0xD531); writeData16(0xB9);
  writeCommand16(0xD532); writeData16(0x03);
  writeCommand16(0xD533); writeData16(0xC1);
  //#B-///////////////////////////////
  writeCommand16(0xD600); writeData16(0x00);
  writeCommand16(0xD601); writeData16(0x37);
  writeCommand16(0xD602); writeData16(0x00);
  writeCommand16(0xD603); writeData16(0x52);
  writeCommand16(0xD604); writeData16(0x00);
  writeCommand16(0xD605); writeData16(0x7B);
  writeCommand16(0xD606); writeData16(0x00);
  writeCommand16(0xD607); writeData16(0x99);
  writeCommand16(0xD608); writeData16(0x00);
  writeCommand16(0xD609); writeData16(0xB1);
  writeCommand16(0xD60A); writeData16(0x00);
  writeCommand16(0xD60B); writeData16(0xD2);
  writeCommand16(0xD60C); writeData16(0x00);
  writeCommand16(0xD60D); writeData16(0xF6);
  writeCommand16(0xD60E); writeData16(0x01);
  writeCommand16(0xD60F); writeData16(0x27);
  writeCommand16(0xD610); writeData16(0x01);
  writeCommand16(0xD611); writeData16(0x4E);
  writeCommand16(0xD612); writeData16(0x01);
  writeCommand16(0xD613); writeData16(0x8C);
  writeCommand16(0xD614); writeData16(0x01);
  writeCommand16(0xD615); writeData16(0xBE);
  writeCommand16(0xD616); writeData16(0x02);
  writeCommand16(0xD617); writeData16(0x0B);
  writeCommand16(0xD618); writeData16(0x02);
  writeCommand16(0xD619); writeData16(0x48);
  writeCommand16(0xD61A); writeData16(0x02);
  writeCommand16(0xD61B); writeData16(0x4A);
  writeCommand16(0xD61C); writeData16(0x02);
  writeCommand16(0xD61D); writeData16(0x7E);
  writeCommand16(0xD61E); writeData16(0x02);
  writeCommand16(0xD61F); writeData16(0xBC);
  writeCommand16(0xD620); writeData16(0x02);
  writeCommand16(0xD621); writeData16(0xE1);
  writeCommand16(0xD622); writeData16(0x03);
  writeCommand16(0xD623); writeData16(0x10);
  writeCommand16(0xD624); writeData16(0x03);
  writeCommand16(0xD625); writeData16(0x31);
  writeCommand16(0xD626); writeData16(0x03);
  writeCommand16(0xD627); writeData16(0x5A);
  writeCommand16(0xD628); writeData16(0x03);
  writeCommand16(0xD629); writeData16(0x73);
  writeCommand16(0xD62A); writeData16(0x03);
  writeCommand16(0xD62B); writeData16(0x94);
  writeCommand16(0xD62C); writeData16(0x03);
  writeCommand16(0xD62D); writeData16(0x9F);
  writeCommand16(0xD62E); writeData16(0x03);
  writeCommand16(0xD62F); writeData16(0xB3);
  writeCommand16(0xD630); writeData16(0x03);
  writeCommand16(0xD631); writeData16(0xB9);
  writeCommand16(0xD632); writeData16(0x03);
  writeCommand16(0xD633); writeData16(0xC1);

  //#Enable Page0
  writeCommand16(0xF000); writeData16(0x55);
  writeCommand16(0xF001); writeData16(0xAA);
  writeCommand16(0xF002); writeData16(0x52);
  writeCommand16(0xF003); writeData16(0x08);
  writeCommand16(0xF004); writeData16(0x00);
  //# RGB I/F Setting
  writeCommand16(0xB000); writeData16(0x08);
  writeCommand16(0xB001); writeData16(0x05);
  writeCommand16(0xB002); writeData16(0x02);
  writeCommand16(0xB003); writeData16(0x05);
  writeCommand16(0xB004); writeData16(0x02);
  //## SDT:
  writeCommand16(0xB600); writeData16(0x08);
  writeCommand16(0xB500); writeData16(0x50);//0x6b ???? 480x854       0x50 ???? 480x800

  //## Gate EQ:
  writeCommand16(0xB700); writeData16(0x00);
  writeCommand16(0xB701); writeData16(0x00);

  //## Source EQ:
  writeCommand16(0xB800); writeData16(0x01);
  writeCommand16(0xB801); writeData16(0x05);
  writeCommand16(0xB802); writeData16(0x05);
  writeCommand16(0xB803); writeData16(0x05);

  //# Inversion: Column inversion (NVT)
  writeCommand16(0xBC00); writeData16(0x00);
  writeCommand16(0xBC01); writeData16(0x00);
  writeCommand16(0xBC02); writeData16(0x00);

  //# BOE's Setting(default)
  writeCommand16(0xCC00); writeData16(0x03);
  writeCommand16(0xCC01); writeData16(0x00);
  writeCommand16(0xCC02); writeData16(0x00);

  //# Display Timing:
  writeCommand16(0xBD00); writeData16(0x01);
  writeCommand16(0xBD01); writeData16(0x84);
  writeCommand16(0xBD02); writeData16(0x07);
  writeCommand16(0xBD03); writeData16(0x31);
  writeCommand16(0xBD04); writeData16(0x00);

  writeCommand16(0xBA00); writeData16(0x01);

  writeCommand16(0xFF00); writeData16(0xAA);
  writeCommand16(0xFF01); writeData16(0x55);
  writeCommand16(0xFF02); writeData16(0x25);
  writeCommand16(0xFF03); writeData16(0x01);

  writeCommand16(0x3500); writeData16(0x00);
  writeCommand16(0x3600); writeData16(0x00);
  writeCommand16(0x3a00); writeData16(0x55);  ////55=16?/////66=18?
  writeCommand16(0x1100);
  delay(120);
  writeCommand16(0x2900 );
  writeCommand16(0x2c00);
  endWrite();
  if (_bl_pin >= 0) pinMode(_bl_pin, OUTPUT);
  enableDisplay(true);
}

void GFX_NT35510::setRotation(uint8_t r)
{
  GFX_Root::setRotation(r);
  startWrite();
  writeCommand16(MADCTL);
  switch (r & 3)
  {
    case 0:
      writeData(_bgr);
      break;
    case 1:
      writeData(MADCTL_MX | MADCTL_MV | _bgr);
      break;
    case 2:
      writeData(MADCTL_MX | MADCTL_MY | _bgr);
      break;
    case 3:
      writeData(MADCTL_MY | MADCTL_MV | _bgr);
      break;
  }
  endWrite();
}

void GFX_NT35510::invertDisplay(bool i)
{
  _bgr = i ? MADCTL_BGR : 0x00;
  setRotation(rotation);
}


void GFX_NT35510::invert(bool i)
{
  invertDisplay(i);
}

void GFX_NT35510::enableDisplay(bool enable)
{
  startWrite();
  writeCommand16(enable ? 0x2900 : 0x2800);  // Display ON / Display OFF
  endWrite();
  if (_bl_pin >= 0) digitalWrite(_bl_pin, enable ? HIGH : LOW);
  enableBacklight(enable); // for _plugin
}

void GFX_NT35510::setAddrWindow(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
  uint16_t xe = x + w - 1;
  uint16_t ye = y + h - 1;
  writeCommand16(0x2A00);
  writeData16(x >> 8);
  writeCommand16(0x2A01);
  writeData16(x & 0x00ff);
  writeCommand16(0x2A02);
  writeData16(xe >> 8);
  writeCommand16(0x2A03);
  writeData16(xe & 0x00ff);
  writeCommand16(0x2B00);
  writeData16(y >> 8);
  writeCommand16(0x2B01);
  writeData16(y & 0x00ff);
  writeCommand16(0x2B02);
  writeData16(ye >> 8);
  writeCommand16(0x2B03);
  writeData16(ye & 0x00ff);
  writeCommand16(0x2C00);
}
