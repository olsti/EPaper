/**
 *  @filename   :   epd2in7b-demo.ino
 *  @brief      :   2.7inch e-paper display (B) demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     July 17 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <SPI.h>
#include "imagedata.h"
#include "epdpaint.h"
#include "epd2in7b.h"

#define COLORED     1
#define UNCOLORED   0
#define SIZEPAINT 1024

static Epd epd;
static unsigned char image[SIZEPAINT];
static Paint paint(image, 264, 176);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  if (epd.Init() != 0) {
    return;
  }
}

void loop() {
  delay(5000);
    /* This clears the SRAM of the e-paper display */
  //epd.ClearFrame();

  /**
    * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
    * In this case, a smaller image buffer is allocated and you have to 
    * update a partial display several times.
    * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
    */
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "_Bitcoin_", &Font8, COLORED);
  epd.TransmitPartialBlack(paint.GetImage(), 80, 176, paint.GetWidth(), paint.GetHeight());
  epd.RefreshPartial(0, 0, 80, 176);
  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "10000", &Font8, COLORED);
  epd.TransmitPartialBlack(paint.GetImage(), 80, 176, paint.GetWidth(), paint.GetHeight());
  epd.RefreshPartial(0, 0, 80, 176);
  delay(2000);
  paint.Clear(UNCOLORED);
  paint.DrawRectangle(0, 0, 40, 50, COLORED);
  paint.DrawLine(0, 0, 40, 50, COLORED);
  epd.TransmitPartialBlack(paint.GetImage(), 10, 130, paint.GetWidth(), paint.GetHeight());
  epd.RefreshPartial(8, 128, 40, 56);
  
  //paint.Clear(UNCOLORED);
  //paint.DrawCircle(32, 32, 30, COLORED);
  //epd.TransmitPartialBlack(paint.GetImage(), 90, 120, paint.GetWidth(), paint.GetHeight());

  //paint.Clear(UNCOLORED);
  //paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
  //epd.TransmitPartialRed(paint.GetImage(), 10, 200, paint.GetWidth(), paint.GetHeight());

  //paint.Clear(UNCOLORED);
  //paint.DrawFilledCircle(32, 32, 30, COLORED);
  //epd.TransmitPartialRed(paint.GetImage(), 90, 190, paint.GetWidth(), paint.GetHeight());

  /* This displays the data from the SRAM in e-Paper module */
  epd.DisplayFrame();

  /* This displays an image */
  //epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);
}
  /* This clears the SRAM of the e-paper display */
  //epd.ClearFrame();

  /**
    * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
    * In this case, a smaller image buffer is allocated and you have to 
    * update a partial display several times.
    * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
    */
    /*

  paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 0, "Bitcoin ", &Font16, COLORED);
  epd.TransmitPartialBlack(paint.GetImage(), 16, 32, paint.GetWidth(), paint.GetHeight());
  
  paint.SetWidth(64);
  paint.SetHeight(64);
  */
/*
  paint.Clear(UNCOLORED);
  paint.DrawRectangle(0, 0, 40, 50, COLORED);
  paint.DrawLine(0, 0, 40, 50, COLORED);
  paint.DrawLine(40, 0, 0, 50, COLORED);
  epd.TransmitPartialBlack(paint.GetImage(), 10, 130, paint.GetWidth(), paint.GetHeight());
  
  paint.Clear(UNCOLORED);
  paint.DrawCircle(32, 32, 30, COLORED);
  epd.TransmitPartialBlack(paint.GetImage(), 90, 120, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledRectangle(0, 0, 40, 50, COLORED);
  epd.TransmitPartialRed(paint.GetImage(), 10, 200, paint.GetWidth(), paint.GetHeight());

  paint.Clear(UNCOLORED);
  paint.DrawFilledCircle(32, 32, 30, COLORED);
  epd.TransmitPartialRed(paint.GetImage(), 90, 190, paint.GetWidth(), paint.GetHeight());
*/
  /* This displays the data from the SRAM in e-Paper module */
  //epd.DisplayFrame();

  /* This displays an image */
  //epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);

  /* Deep sleep */
  //epd.Sleep();

//void loop() {
//  delay(5000);
//  int incomingByte = 0; // for incoming serial data
//  // put your main code here, to run repeatedly:
//  if(Serial.available() > 0) {
//    String price = Serial.readString();
//  //incomingByte = 10000;
//  //String price = String(incomingByte);
//  // Length (with one extra character for the null terminator)
//    int str_len = price.length() + 1; 
//  // Prepare the character array (the buffer) 
//    char char_array[str_len];
//    price.toCharArray(char_array, str_len);
//    paint.Clear(UNCOLORED);
//    paint.DrawStringAt(0, 32, "BITCOIN ", &Font16, COLORED);  
//    epd.TransmitPartialBlack(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
//    epd.DisplayFrame();
//  } else {
//    paint.Clear(UNCOLORED);
//    paint.DrawStringAt(0, 32, "ERROR SERIAL ", &Font16, COLORED);  
//    epd.TransmitPartialBlack(paint.GetImage(), 0, 0, paint.GetWidth(), paint.GetHeight());
//    epd.DisplayFrame();
//  }
//  
//}
