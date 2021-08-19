

// Display Library example for SPI e-paper panels from Dalian Good Display and boards from Waveshare.
// Requires HW SPI and Adafruit_GFX. Caution: the e-paper panels require 3.3V supply AND data lines!
//
// Display Library based on Demo Example from Good Display: http://www.e-paper-display.com/download_list/downloadcategoryid=34&isMode=false.html
//
// Author: Jean-Marc Zingg
//
// Version: see library.properties
//
// Library: https://github.com/ZinggJM/GxEPD2

// Supporting Arduino Forum Topics:
// Waveshare e-paper displays with SPI: http://forum.arduino.cc/index.php?topic=487007.0
// Good Display ePaper for Arduino: https://forum.arduino.cc/index.php?topic=436411.0

// see GxEPD2_wiring_examples.h for wiring suggestions and examples

// base class GxEPD2_GFX can be used to pass references or pointers to the display instance as parameter, uses ~1.2k more code
// enable or disable GxEPD2_GFX base class
#define ENABLE_GxEPD2_GFX 0

// uncomment next line to use class GFX of library GFX_Root instead of Adafruit_GFX
//#include <GFX.h>
// Note: if you use this with ENABLE_GxEPD2_GFX 1:
//       uncomment it in GxEPD2_GFX.h too, or add #include <GFX.h> before any #include <GxEPD2_GFX.h>

#include <GxEPD2_3C.h>
#include "Fonts/FreeMonoBold9pt7b.h"
#include <Adafruit_GFX.h>

// select the display class and display driver class in the following file (new style):
// note that many of these selections would require paged display to fit in ram
//#include <GxEPD2_display_selection_new_style.h>

// or select the display constructor line below (old style), double commented out selections don't fit in ram:
// note: this list may not get updated with new panels

// can't use package "STMF1 Boards (STM32Duino.com)" (Roger Clark) anymore with Adafruit_GFX, use "STM32 Boards (selected from submenu)" (STMicroelectronics)

// Pin definition
#define RST_PIN         45
#define DC_PIN          49
#define CS_PIN          53
#define BUSY_PIN        47

#define MAX_DISPLAY_BUFFER_SIZE 7000ul // ~15k is a good compromise
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
// select one and adapt to your mapping
// 3-color e-papers
#define MAX_HEIGHT_3C(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
GxEPD2_3C<GxEPD2_270c, MAX_HEIGHT_3C(GxEPD2_270c)> display(GxEPD2_270c(/*CS=4*/ CS_PIN, /*DC=*/ DC_PIN, /*RST=*/ RST_PIN, /*BUSY=*/ BUSY_PIN));
#include "bitmaps/Bitmaps3c176x264.h" // 2.7"  b/w/r

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("setup");
  delay(100);
  display.init(115200);
  clearScreen();
  // first update should be full refresh
  //printLabelValue("Bitcoin", "10.3k", 176/4, 264/4, 0);
  printLabel("SMI Index", 10, 20, 0);
  printLabel("12'000 ", 10, 40, 0);
  printLabel("--------------", 10, 50, 0);
  printLabel("Bitcoin", 10, 60, 0);
  printLabel("43.3k$ ", 10, 80, 0);
  printLabel("--------------", 10, 90, 0);
  printLabel("Ethereum", 10, 100, 0);
  printLabel("3000$ ", 10, 120, 0);
  printLabel("--------------", 10, 130, 0);
}

void loop() {
  delay(1000);
  bool ok = checkSerialAvailable();
  char outStr[10];
  ok = ok && readDataSerial(outStr, sizeof(outStr));
  ok = ok && printLabel(outStr, 10, 40, 0);
  if(!ok) {
      printLabel("Error Serial", 10, 40, 0);    
  }
}

bool checkSerialAvailable() {
  if(Serial.available() > 0) {
    return true;
  }
  return false;
}

bool readDataSerial(char *input, int size_arr) {
   String dataStr = Serial.readString();
   dataStr.toCharArray(input, size_arr);
   return true;
}

void clearScreen() {
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.display(false); // full update 
}

bool printLabel(const char* label, uint16_t x, uint16_t y, uint16_t rotation) {
  display.setRotation(rotation);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(label, 0, 0, &tbx, &tby, &tbw, &tbh);
  // full window mode is the initial mode, set it anyway
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE); 
  display.setCursor(x, y);
  display.print(label);
  display.displayWindow(x, y-(tbh), tbw, tbh+1);
  return true;
}

void printLabelValue(const char* label, const char* value, uint16_t x, uint16_t y, uint16_t rotation) {
  display.setRotation(rotation);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(label, 0, 0, &tbx, &tby, &tbw, &tbh);
  // full window mode is the initial mode, set it anyway
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE); 
  display.setCursor(x, y);
  display.print(label);
  display.setCursor(x, y+20);
  display.print(value);
  display.display(false); // full update
}

void printLabelCenter(const char* label, uint16_t rotation) {
  display.setRotation(rotation);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(label, 0, 0, &tbx, &tby, &tbw, &tbh);
  // center bounding box by transposition of origin:
  uint16_t x_ = ((display.width() - tbw) / 2) - tbx;
  uint16_t y_ = ((display.height() - tbh) / 2) - tby;
  // full window mode is the initial mode, set it anyway
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE); 
  display.setCursor(x_, y_);
  display.print(label);
  display.display(true); // partial update
}


void helloFullScreenPartialMode() {
  //Serial.println("helloFullScreenPartialMode");
  const char fullscreen[] = "full screen update";
  const char fpm[] = "fast partial mode";
  const char spm[] = "slow partial mode";
  const char npm[] = "no partial mode";
  // full window mode is the initial mode, set it anyway
  display.setFullWindow();
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  const char* updatemode;
  if (display.epd2.hasFastPartialUpdate)
  {
    updatemode = fpm;
  }
  else if (display.epd2.hasPartialUpdate)
  {
    updatemode = spm;
  }
  else
  {
    updatemode = npm;
  }
  // do this outside of the loop
  int16_t tbx, tby; uint16_t tbw, tbh;
  // center update text
  display.getTextBounds(fullscreen, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t utx = ((display.width() - tbw) / 2) - tbx;
  uint16_t uty = ((display.height() / 4) - tbh / 2) - tby;
  // center update mode
  display.getTextBounds(updatemode, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t umx = ((display.width() - tbw) / 2) - tbx;
  uint16_t umy = ((display.height() * 3 / 4) - tbh / 2) - tby;
  // center HelloWorld
  display.getTextBounds("Bitcoin", 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t hwx = ((display.width() - tbw) / 2) - tbx;
  uint16_t hwy = ((display.height() - tbh) / 2) - tby;
  display.fillScreen(GxEPD_WHITE);
  display.setCursor(hwx, hwy);
  display.print("Bitcoin");
  display.setCursor(utx, uty);
  display.print(fullscreen);
  display.setCursor(umx, umy);
  display.print(updatemode);
  display.display(true); // partial update
  //Serial.println("helloFullScreenPartialMode done");
}

void printBitcoinValue(const char* value) {
  //Serial.println("helloArduino");
  display.setRotation(1);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(display.epd2.hasColor ? GxEPD_RED : GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  // align with centered HelloWorld
  display.getTextBounds(value, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t x = ((display.width() - tbw) / 2) - tbx;
  // height might be different
  display.getTextBounds(value, 0, 0, &tbx, &tby, &tbw, &tbh);
  uint16_t y = ((display.height() / 4) - tbh / 2) - tby; // y is base line!
  // make the window big enough to cover (overwrite) descenders of previous text
  uint16_t wh = FreeMonoBold9pt7b.yAdvance;
  uint16_t wy = (display.height() / 4) - wh / 2;
  // full window mode is the initial mode, set it anyway
  display.setFullWindow();
  // DO NOT display.fillScreen(GxEPD_WHITE); keep current content!
  display.fillRect(0, wy, display.width(), wh, GxEPD_WHITE); // clear rect instead
  display.setCursor(x, y);
  display.print(value);
  display.displayWindow(0, wy, display.width(), wh);
  delay(1000);
  //Serial.println("helloArduino done");
}

void showBox(uint16_t x, uint16_t y, uint16_t w, uint16_t h, bool partial) {
  //Serial.println("showBox");
  display.setRotation(1);
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.fillRect(x, y, w, h, GxEPD_BLACK);
  if (partial)
  {
    display.displayWindow(x, y, w, h);
  } else {
    display.display(false); // full update
  }
  //Serial.println("showBox done");
}

void drawCornerTest() {
  display.setFullWindow();
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  for (uint16_t r = 0; r <= 4; r++)
  {
    display.setRotation(r);
    display.fillScreen(GxEPD_WHITE);
    display.fillRect(0, 0, 8, 8, GxEPD_BLACK);
    display.fillRect(display.width() - 18, 0, 16, 16, GxEPD_BLACK);
    display.fillRect(display.width() - 25, display.height() - 25, 24, 24, GxEPD_BLACK);
    display.fillRect(0, display.height() - 33, 32, 32, GxEPD_BLACK);
    display.setCursor(display.width() / 2, display.height() / 2);
    display.print(display.getRotation());
    display.display(false); // full update
    delay(2000);
  }
}

// note for partial update window and setPartialWindow() method:
// partial update window size and position is on byte boundary in physical x direction
// the size is increased in setPartialWindow() if x or w are not multiple of 8 for even rotation, y or h for odd rotation
// see also comment in GxEPD2_BW.h, GxEPD2_3C.h or GxEPD2_GFX.h for method setPartialWindow()
// showPartialUpdate() purposely uses values that are not multiples of 8 to test this

void showPartialUpdate()
{
  // some useful background
  printLabelValue("Bitcoin", "10.0k", 0, 0, 0);
  // use asymmetric values for test
  uint16_t box_x = 10;
  uint16_t box_y = 15;
  uint16_t box_w = 70;
  uint16_t box_h = 20;
  uint16_t cursor_y = box_y + box_h - 6;
  float value = 13.95;
  uint16_t incr = display.epd2.hasFastPartialUpdate ? 1 : 3;
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();
  // show where the update box is
  for (uint16_t r = 0; r < 4; r++)
  {
    display.setRotation(r);
    display.fillRect(box_x, box_y, box_w, box_h, GxEPD_BLACK);
    //display.fillScreen(GxEPD_BLACK);
    display.displayWindow(box_x, box_y, box_w, box_h);
    delay(2000);
    display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    display.displayWindow(box_x, box_y, box_w, box_h);
    delay(1000);
  }
  //return;
  // show updates in the update box
  for (uint16_t r = 0; r < 4; r++)
  {
    display.setRotation(r);
    for (uint16_t i = 1; i <= 10; i += incr)
    {
      display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
      display.setCursor(box_x, cursor_y);
      display.print(value * i, 2);
      display.displayWindow(box_x, box_y, box_w, box_h);
      delay(500);
    }
    delay(1000);
    display.fillRect(box_x, box_y, box_w, box_h, GxEPD_WHITE);
    display.displayWindow(box_x, box_y, box_w, box_h);
    delay(1000);
  }
}


void drawBitmaps()
{
  display.setFullWindow();
#ifdef _GxBitmaps3c176x264_H_
  drawBitmaps3c176x264();
#endif
}

#ifdef _GxBitmaps176x264_H_
void drawBitmaps176x264()
{
#if !defined(__AVR)
  const unsigned char* bitmaps[] =
  {
    Bitmap176x264_1, Bitmap176x264_2, Bitmap176x264_3, Bitmap176x264_4, Bitmap176x264_5
  };
#else
  const unsigned char* bitmaps[] =
  {
    Bitmap176x264_1, Bitmap176x264_2 //, Bitmap176x264_3, Bitmap176x264_4, Bitmap176x264_5
  };
#endif
  if (display.epd2.panel == GxEPD2::GDEW027W3)
  {
    for (uint16_t i = 0; i < sizeof(bitmaps) / sizeof(char*); i++)
    {
      display.fillScreen(GxEPD_WHITE);
      display.drawInvertedBitmap(0, 0, bitmaps[i], display.epd2.WIDTH, display.epd2.HEIGHT, GxEPD_BLACK);
      display.display();
      delay(2000);
    }
  }
}
#endif




struct bitmap_pair
{
  const unsigned char* black;
  const unsigned char* red;
};

#ifdef _GxBitmaps3c176x264_H_
void drawBitmaps3c176x264()
{
  bitmap_pair bitmap_pairs[] =
  {
    {Bitmap3c176x264_black, Bitmap3c176x264_red}
  };
  if (display.epd2.panel == GxEPD2::GDEW027C44)
  {
    for (uint16_t i = 0; i < sizeof(bitmap_pairs) / sizeof(bitmap_pair); i++)
    {
      display.fillScreen(GxEPD_WHITE);
      display.drawBitmap(0, 0, bitmap_pairs[i].black, display.epd2.WIDTH, display.epd2.HEIGHT, GxEPD_BLACK);
      display.drawBitmap(0, 0, bitmap_pairs[i].red, display.epd2.WIDTH, display.epd2.HEIGHT, GxEPD_RED);
      display.display();
      delay(2000);
    }
  }
}
#endif
