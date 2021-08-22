
// Author: Timo Olsthoorn EPaper Finance
// Author Lib: EPaper LibraryJean-Marc Zingg
// Library: https://github.com/ZinggJM/GxEPD2

#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_3C.h>
#include "Fonts/FreeMonoBold9pt7b.h"
#include "Fonts/FreeMonoBold12pt7b.h"
#include <Adafruit_GFX.h>

// Pin definition
#define RST_PIN         45
#define DC_PIN          49
#define CS_PIN          53
#define BUSY_PIN        47

#define MAX_DISPLAY_BUFFER_SIZE 6000ul 
#define MAX_HEIGHT(EPD) (EPD::HEIGHT <= MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8) ? EPD::HEIGHT : MAX_DISPLAY_BUFFER_SIZE / (EPD::WIDTH / 8))
#define MAX_HEIGHT_3C(EPD) (EPD::HEIGHT <= (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8) ? EPD::HEIGHT : (MAX_DISPLAY_BUFFER_SIZE / 2) / (EPD::WIDTH / 8))
GxEPD2_3C<GxEPD2_270c, MAX_HEIGHT_3C(GxEPD2_270c)> display(GxEPD2_270c(/*CS=4*/ CS_PIN, /*DC=*/ DC_PIN, /*RST=*/ RST_PIN, /*BUSY=*/ BUSY_PIN));
#include "bitmaps/Bitmaps3c176x264.h" // 2.7"  b/w/r

#define VERSION "V1.1"
#define STRING_SIZE 20

enum class ValueType {
  UNDEF,
  SMI,
  BTC,
  ETH 
};

void setup() {
  Serial.begin(115200);
  delay(100);
  display.init(115200);
  clearScreen();
  printHeader(VERSION);
  Serial.print("<ARD_READY>");
}

void loop() {
  delay(500);
  char outStr[STRING_SIZE+1]; //+1 for char-end
  bool ok = readDataSerial(outStr, sizeof(outStr));
  ValueType valueType = ValueType::UNDEF;
  ok = ok && checkInputValue(outStr, valueType);
  ok = ok && printValueType(outStr, valueType);
  if(!ok) {
      Serial.print("no data recieved\n"); 
  } else {
    //show data a little longer
    delay(3000);
  }
}


void clearScreen() {
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.display(false); // full update 
}

bool printHeader(const char* vers) {
  display.setRotation(0);
  display.setFont(&FreeMonoBold9pt7b);
  display.setTextColor(GxEPD_BLACK);
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE);
  display.setCursor(130, 10);
  display.print("V1.1");
  display.setCursor(10, 20);
  display.print("SMI Index");
  display.setCursor(10, 40);
  display.print("No Data");
  display.setCursor(10, 50);
  display.print("--------------");
  display.setCursor(10, 60);
  display.print("Bitcoin");
  display.setCursor(10, 80);
  display.print("No Data");
  display.setCursor(10, 90);
  display.print("--------------");
  display.setCursor(10, 100);
  display.print("Ethereum");
  display.setCursor(10, 120);
  display.print("No Data");
  display.setCursor(10, 130);
  display.print("--------------");
  display.display(false); // full update
  return true;
}


bool checkSerialAvailable() {
  if(Serial.available() > 0) {
    Serial.print("New Value available\n");
    return true;
  }
  return false;
}

bool readDataSerial(char *input, int size_arr) {
  String dataStr;
  while(checkSerialAvailable()) {
    dataStr += Serial.readString();
  }
   dataStr.toCharArray(input, size_arr);
   Serial.print(input);
   if(dataStr.length()>0) {
    return true;
   }
   return false;
}

bool checkInputValue(char *input, ValueType &valueType) {
  bool ok = false;
  //input[1] first char is startmarker
  if(strncmp(&input[1], "SMI", 3 == 0)) {
    valueType  = ValueType::SMI;
    Serial.print("SMI Value found");
    ok = true;
  } else if (strncmp(&input[1], "BTC", 3 == 0)) {
    valueType  = ValueType::BTC;
    Serial.print("BTC Value found");
    ok = true;
  } else if (strncmp(&input[1], "ETH", 3 == 0)) {
    valueType  = ValueType::ETH;
    Serial.print("ETH Value found");
    ok = true;
  }
  return ok;
}

bool printValueType(const char* value, ValueType valueType) {
  bool ok = false;
  switch(valueType) {
    case ValueType::UNDEF:
    //do nothing
    break;
    case ValueType::SMI:
      ok = printValue(value, 10, 40, 0);
      break;
    case ValueType::BTC:
      ok = printValue(value, 10, 80, 0);
      break;
    case ValueType::ETH:
      ok = printValue(value, 10, 120, 0);
      break;
  }
  return ok;
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
  display.displayWindow(x, y-(tbh), tbw+2, tbh+1);
  return true;
}

bool printValue(const char* value, uint16_t x, uint16_t y, uint16_t rotation) {
  display.setRotation(rotation);
  display.setFont(&FreeMonoBold12pt7b);
  display.setTextColor(GxEPD_BLACK);
  int16_t tbx, tby; uint16_t tbw, tbh;
  display.getTextBounds(value, 0, 0, &tbx, &tby, &tbw, &tbh);
  // full window mode is the initial mode, set it anyway
  display.setFullWindow();
  display.fillScreen(GxEPD_WHITE); 
  display.setCursor(x, y);
  display.print(value);
  display.displayWindow(x, y-(tbh), display.width(), tbh+1);
  return true;
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
