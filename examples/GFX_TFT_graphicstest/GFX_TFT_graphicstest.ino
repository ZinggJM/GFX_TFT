/* Modified by Bodmer to be an example for TFT_HX8357 library.
   This sketch uses the GLCD font only.

   The performance for each test is reported to the serial
   port at 38400 baud.

   This test occupies the whole of the display therefore
   will take twice as long as it would on a 320 x 240
   display. Bear this in mind when making performance
   comparisons.

   Make sure all the required font is loaded by editting the
   User_Setup.h file in the TFT_HX8357 library folder.

   Original header is at the end of the sketch, some text in it is
   not applicable to the HX8357 display supported by this example.
*/

// modified by Jean-Marc Zingg to be an example for the GFX_TFT library (from GxTFT library)
// original source taken from https://github.com/Bodmer/TFT_HX8357

// include library, make path known
#include <GFX_TFT.h>

// select a known configuration (by uncomment in following file):
#include "GFX_TFT_known_configurations.h"

// or select a configuration by uncommenting driver class, optional IO plugin, and display instance

#ifndef GFX_TFT_CLASS // not selected by GFX_TFT_known_configurations.h

// include the display class (select one)
//#include <GFX_ILI9341.h>
//#include <GFX_ILI9486.h>
//#include <GFX_ILI9488.h>
//#include <GFX_ST7796S.h>
//#include <GFX_OTM8009A.h>
//#include <GFX_NT35510.h>
//#include <GFX_SSD1963.h>
//#include <GFX_ILI9806.h>

// include the optional io plugin class (select none or one)
//#include <GFX_TFT_IO/FSMC/GFX_STM32F407ZGM4_FSMC.h>
//#include <GFX_TFT_IO/GFX_STM32F103C.h>
//#include <GFX_TFT_IO/GFX_STM32F103C_TIKY.h>
//#include <GFX_TFT_IO/GFX_STM32F103V_TIKY.h>
//#include <GFX_TFT_IO/FSMC/GFX_STM32F103V_FSMC_TIKY.h>
//#include <GFX_TFT_IO/GFX_STM32F103ZET6.h>
//#include <GFX_TFT_IO/FSMC/GFX_STM32F103ZET6_FSMC.h>
//#include <GFX_TFT_IO/GFX_STM32F103ZET6M3.h>
//#include <GFX_TFT_IO/FSMC/GFX_STM32F103ZET6M3_FSMC.h>
//#include <GFX_TFT_IO/GFX_STM32F407VET6.h>
//#include <GFX_TFT_IO/FSMC/GFX_STM32F407VET6_FSMC.h>
//#include <GFX_TFT_IO/GFX_STM32F407VxT6.h>
//#include <GFX_TFT_IO/FSMC/GFX_STM32F407VxT6_FSMC.h>
//#include <GFX_TFT_IO/GFX_STM32F407ZET6.h>
//#include <GFX_TFT_IO/FSMC/GFX_STM32F407ZET6_FSMC.h>
//#include <GFX_TFT_IO/GFX_STM32F407ZGM4.h>
//#include <GFX_TFT_IO/FSMC/GFX_STM32F407ZGM4_FSMC.h>
//#include <GFX_TFT_IO/GFX_STM32Nucleo144.h>
//#include <GFX_TFT_IO/FSMC/GFX_STM32Nucleo144_FSMC.h>

#if defined(GFX_TFT_CLASS) && defined(GFX_TFT_IO_PLUGIN_CLASS)

// select the plugin class constructor (only one)
GFX_TFT_IO_PLUGIN_CLASS io_plugin; // plugin without parameters

// select the tft class constructor (only one)
GFX_TFT_CLASS tft(io_plugin); // constructor with plugin parameter

#elif defined(GFX_TFT_CLASS)

// adapt the constructor parameters to your wiring for the appropriate processor conditional, 
// or add a new one or adapt the catch all other default

#if defined (ESP8266)
#ifdef _GFX_ILI9341_H_
GFX_ILI9341 tft(/*CS=D8*/ SS, /*DC=D3*/ 0, /*RST=D4*/ 2, /*BL=D2*/ 4); // my epd connection shield
#endif
#endif

#endif // selected GFX_TFT_CLASS or GFX_TFT_CLASS and GFX_TFT_IO_PLUGIN_CLASS

#endif // not selected by GFX_TFT_known_configurations.h

#if !defined(ESP8266)
#define yield()
#endif

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//#undef Serial
//#define Serial if(0) Serial1

void setup()
{
  Serial.begin(115200);
  while (!Serial) yield();
  Serial.println();
  Serial.println("setup");
  //Serial.println(String(controller.name) + " Test on " + String(io.name));

  tft.init();

  Serial.println("tft.init() done");

  Serial.println(F("Benchmark                Time (microseconds)"));

  Serial.print(F("Screen fill              "));
  Serial.println(testFillScreen());
  delay(500);

  //return;

  Serial.print(F("Text                     "));
  Serial.println(testText());
  delay(3000);

  //return;

  Serial.print(F("Lines                    "));
  Serial.println(testLines(CYAN));
  delay(500);

  //return;

  Serial.print(F("Horiz/Vert Lines         "));
  Serial.println(testFastLines(RED, BLUE));
  delay(500);

  Serial.print(F("Rectangles (outline)     "));
  Serial.println(testRects(GREEN));
  delay(500);

  Serial.print(F("Rectangles (filled)      "));
  Serial.println(testFilledRects(YELLOW, MAGENTA));
  delay(500);

  Serial.print(F("Circles (filled)         "));
  Serial.println(testFilledCircles(10, MAGENTA));

  Serial.print(F("Circles (outline)        "));
  Serial.println(testCircles(10, WHITE));
  delay(500);

  Serial.print(F("Triangles (outline)      "));
  Serial.println(testTriangles());
  delay(500);

  //while(1) yield();

  Serial.print(F("Triangles (filled)       "));
  Serial.println(testFilledTriangles());
  delay(500);

  //return;

  Serial.print(F("Rounded rects (outline)  "));
  Serial.println(testRoundRects());
  delay(500);

  //while(1) yield();

  Serial.print(F("Rounded rects (filled)   "));
  Serial.println(testFilledRoundRects());
  delay(500);

  //while (1) yield();

  //testEllipses();
  //testCurves();

  Serial.println(F("Done!"));

}

void loop(void)
{
  for (uint8_t rotation = 0; rotation < 4; rotation++)
  {
    tft.setRotation(rotation);
    testText();
    delay(10000);
  }
}

unsigned long testFillScreen() {
  unsigned long start = micros();
  tft.fillScreen(BLACK);
  yield();
  delay(200); // too fast to be seen on some
  tft.fillScreen(RED);
  yield();
  delay(200); // too fast to be seen on some
  tft.fillScreen(GREEN);
  yield();
  delay(200); // too fast to be seen on some
  tft.fillScreen(BLUE);
  yield();
  delay(200); // too fast to be seen on some
  tft.fillScreen(BLACK);
  yield();
  return micros() - start;
}

unsigned long testText() {
  tft.fillScreen(BLACK);
  unsigned long start = micros();
  tft.setCursor(0, 0);
  tft.setTextColor(WHITE);  tft.setTextSize(1);
  tft.println("Hello World!");
  tft.setTextColor(YELLOW); tft.setTextSize(2);
  tft.println(1234.56);
  tft.setTextColor(RED);    tft.setTextSize(3);
  tft.println(0xDEADBEEF, HEX);
  tft.println();
  tft.setTextColor(GREEN);
  tft.setTextSize(5);
  tft.println("Groop");
  tft.setTextSize(2);
  tft.println("I implore thee,");
  tft.setTextSize(1);
  tft.setTextColor(GREEN);
  tft.println("my foonting turlingdromes.");
  tft.println("And hooptiously drangle me");
  tft.println("with crinkly bindlewurdles,");
  tft.println("Or I will rend thee");
  tft.println("in the gobberwarts");
  tft.println("with my blurglecruncheon,");
  tft.println("see if I don't!");
  return micros() - start;
}

unsigned long testLines(uint16_t color) {
  unsigned long start, t;
  int           x1, y1, x2, y2,
                w = tft.width(),
                h = tft.height();

  tft.fillScreen(BLACK);
  yield();

  x1 = y1 = 0;
  y2    = h - 1;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  yield();
  x2    = w - 1;
  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  yield();
  t     = micros() - start; // fillScreen doesn't count against timing

  tft.fillScreen(BLACK);
  yield();

  x1    = w - 1;
  y1    = 0;
  y2    = h - 1;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  yield();
  x2    = 0;
  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  yield();
  t    += micros() - start;

  tft.fillScreen(BLACK);
  yield();

  x1    = 0;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  yield();
  x2    = w - 1;
  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  yield();
  t    += micros() - start;

  tft.fillScreen(BLACK);
  yield();

  x1    = w - 1;
  y1    = h - 1;
  y2    = 0;
  start = micros();
  for (x2 = 0; x2 < w; x2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  yield();
  x2    = 0;
  for (y2 = 0; y2 < h; y2 += 6) tft.drawLine(x1, y1, x2, y2, color);
  yield();

  return micros() - start;
}

unsigned long testFastLines(uint16_t color1, uint16_t color2) {
  unsigned long start;
  int           x, y, w = tft.width(), h = tft.height();

  tft.fillScreen(BLACK);
  yield();
  start = micros();
  for (y = 0; y < h; y += 5) tft.drawFastHLine(0, y, w, color1);
  yield();
  for (x = 0; x < w; x += 5) tft.drawFastVLine(x, 0, h, color2);
  yield();

  return micros() - start;
}

unsigned long testRects(uint16_t color) {
  unsigned long start;
  int           n, i, i2,
                cx = tft.width()  / 2,
                cy = tft.height() / 2;

  tft.fillScreen(BLACK);
  n     = min(tft.width(), tft.height());
  start = micros();
  for (i = 2; i < n; i += 6) {
    i2 = i / 2;
    tft.drawRect(cx - i2, cy - i2, i, i, color);
  }

  return micros() - start;
}

unsigned long testFilledRects(uint16_t color1, uint16_t color2) {
  unsigned long start, t = 0;
  int           n, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  n = min(tft.width(), tft.height());
  for (i = n; i > 0; i -= 6) {
    i2    = i / 2;
    start = micros();
    tft.fillRect(cx - i2, cy - i2, i, i, color1);
    t    += micros() - start;
    // Outlines are not included in timing results
    tft.drawRect(cx - i2, cy - i2, i, i, color2);
    yield();
  }

  return t;
}

unsigned long testFilledCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int x, y, w = tft.width(), h = tft.height(), r2 = radius * 2;

  tft.fillScreen(BLACK);
  start = micros();
  for (x = radius; x < w; x += r2) {
    for (y = radius; y < h; y += r2) {
      tft.fillCircle(x, y, radius, color);
    }
    yield();
  }

  return micros() - start;
}

unsigned long testCircles(uint8_t radius, uint16_t color) {
  unsigned long start;
  int           x, y, r2 = radius * 2,
                      w = tft.width()  + radius,
                      h = tft.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros();
  for (x = 0; x < w; x += r2) {
    for (y = 0; y < h; y += r2) {
      tft.drawCircle(x, y, radius, color);
    }
    yield();
  }

  return micros() - start;
}

unsigned long testTriangles() {
  unsigned long start;
  int           n, i, cx = tft.width()  / 2 - 1,
                      cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  n     = min(cx, cy);
  start = micros();
  for (i = 0; i < n; i += 5) {
    tft.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      tft.color565(0, 0, i));
  }

  return micros() - start;
}

unsigned long testFilledTriangles() {
  unsigned long start, t = 0;
  int           i, cx = tft.width()  / 2 - 1,
                   cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  start = micros();
  for (i = min(cx, cy); i > 10; i -= 5) {
    start = micros();
    tft.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                     tft.color565(0, i, i));
    t += micros() - start;
    tft.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
                     tft.color565(i, i, 0));
    yield();
  }

  return t;
}

unsigned long testRoundRects() {
  unsigned long start;
  int           w, i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  w     = min(tft.width(), tft.height());
  start = micros();
  for (i = 0; i < w; i += 6) {
    i2 = i / 2;
    tft.drawRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.color565(i, 0, 0));
  }

  return micros() - start;
}

unsigned long testFilledRoundRects() {
  unsigned long start;
  int           i, i2,
                cx = tft.width()  / 2 - 1,
                cy = tft.height() / 2 - 1;

  tft.fillScreen(BLACK);
  start = micros();
  for (i = min(tft.width(), tft.height()); i > 20; i -= 6) {
    i2 = i / 2;
    tft.fillRoundRect(cx - i2, cy - i2, i, i, i / 8, tft.color565(0, i, 0));
    yield();
  }

  return micros() - start;
}

#if 0
void testEllipses() 
{
  tft.fillScreen(BLACK);
  for (int i = 0; i < 40; i++)
  {
    int rx = random(60);
    int ry = random(60);
    int x = rx + random(480 - rx - rx);
    int y = ry + random(320 - ry - ry);
    tft.fillEllipse(x, y, rx, ry, random(0xFFFF));
  }
  delay(2000);
  tft.fillScreen(BLACK);
  for (int i = 0; i < 40; i++)
  {
    int rx = random(60);
    int ry = random(60);
    int x = rx + random(480 - rx - rx);
    int y = ry + random(320 - ry - ry);
    tft.drawEllipse(x, y, rx, ry, random(0xFFFF));
  }
  delay(2000);
}

void testCurves() 
{
  uint16_t x = tft.width() / 2;
  uint16_t y = tft.height() / 2;
  tft.fillScreen(BLACK);

  tft.drawEllipse(x, y, 100, 60, PURPLE);
  tft.fillCurve(x, y, 80, 30, 0, CYAN);
  tft.fillCurve(x, y, 80, 30, 1, MAGENTA);
  tft.fillCurve(x, y, 80, 30, 2, YELLOW);
  tft.fillCurve(x, y, 80, 30, 3, RED);
  delay(2000);

  tft.drawCurve(x, y, 90, 50, 0, CYAN);
  tft.drawCurve(x, y, 90, 50, 1, MAGENTA);
  tft.drawCurve(x, y, 90, 50, 2, YELLOW);
  tft.drawCurve(x, y, 90, 50, 3, RED);
  tft.fillCircle(x, y, 30, BLUE);
  delay(5000);
}
#endif

// Original sketch header
/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
