/**
 *  @filename   :   epd2in9-demo.ino
 *  @brief      :   2.9inch e-paper display demo
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     September 9 2017
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
/**
 * May 06 2018: port to Particle Photon by Martin Schreiber @gruemscheli (twitter)
 *
 * Hookup 2.9:     UNO:           PHOTON:
 *  3V3 (red) ---> 3V3       ---> 3V3
 *  GND (blk) ---- GND       ---- GND
 *  DIN (blu) ---> MOSI(D11) ---> MOSI(A5)
 *  --             MISO(D12)      MISO(A4)
 *	CLK (ylo) ---> SCLK(D13) ---> SCK (A3)
 *  CS  (ora) ---> SS  (D10) ---> SS  (A2*|D5)
 *  DC  (grn) ---> OUT (D9)  ---> OUT (A1|D2)
 *  RST (wht) ---> OUT (D8)  ---> OUT (A0|D3)
 *  BUSY(mag) <--- IN  (D7)  <--- IN  (A7|D4)
 *
 * useful links:
 *	Waveshare e-paper:
 * 		https://www.waveshare.com/2.9inch-e-paper-module.htm
 *	  https://www.waveshare.com/wiki/2.9inch_e-Paper_Module
 *		https://mcuoneclipse.com/2017/11/04/fascinating-details-of-waveshare-e-paper-displays/
 *  PHOTON pinout:
 * 		http://wiki.amperka.ru/_media/продукты:particle-photon-headers:particle-photon-headers_pinout.png?cache=
 * 	Arduino UNO pinout:
 *		http://www.pighixxx.com/test/wp-content/uploads/2017/05/uno.png
 */

#include <SPI.h>
#include <epd2in9.h>
#include <epdpaint.h>
#include "imagedata.h"

#define COLORED     0
#define UNCOLORED   1

/**
  * Due to RAM not enough in Arduino UNO, a frame buffer is not allowed.
  * In this case, a smaller image buffer is allocated and you have to
  * update a partial display several times.
  * 1 byte = 8 pixels, therefore you have to set 8*N pixels at a time.
  */
unsigned char image[1024];
Paint paint(image, 0, 0);    // width should be the multiple of 8

#define CS_PIN   D5
#define DC_PIN   D2
#define RST_PIN  D3
#define BSY_PIN  D4

Epd epd(SPI, CS_PIN, DC_PIN, RST_PIN, BSY_PIN);

SerialLogHandler logHandler (
	LOG_LEVEL_ALL, // LOG_LEVEL_WARN// Logging level for non-application messages
	{ { "app", LOG_LEVEL_ALL }, }
);


void setup() {
	// put your setup code here, to run once

	// wait for the developer to connect monitor serial.
	// (i.e. with CLI: particle serial monitor)
	if (Serial.isConnected()) {
		for (int i = 5; i>0; i--) {
			Log.trace("continue setup() in %d seconds...", i);
			delay(1000);
		}
	}


	Log.trace("e-Paper ---- full update mode ----");

	Log.trace("e-Paper initializing for full updates...");
  if (epd.Init(lut_full_update) != 0) {
		  Log.error("initializing e-Paper for full updates FAILED.");
      return;
  }
	Log.trace("e-Paper initializing for full updates OK.");

	/**
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to clear the frame memory twice.
   */
	Log.trace("e-Paper clearing memory...");
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
  epd.ClearFrameMemory(0xFF);   // bit set = white, bit reset = black
  epd.DisplayFrame();
	Log.trace("e-Paper clearing memory OK.");

	Log.trace("e-Paper setting rotation and resolution memory...");
  paint.SetRotate(ROTATE_0);
	Log.trace("e-Paper setting rotation and resolution memory OK.");

	// For simplicity, the arguments are explicit numerical coordinates.

	Log.trace("e-Paper drawing string \"Hello World!\" inverted...");
  paint.SetWidth(128);
  paint.SetHeight(24);
  paint.Clear(COLORED);
  paint.DrawStringAt(0, 4, "Hello World!", &Font16, UNCOLORED);
  epd.SetFrameMemory(paint.GetImage(), 0, 4, paint.GetWidth(), paint.GetHeight());
	Log.trace("e-Paper drawing string \"Hello World!\" inverted OK.");

	Log.trace("drawing string \"e-Paper Demo\" normal...");
	paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, "e-Paper Demo", &Font12, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 16, 32, paint.GetWidth(), paint.GetHeight());
	Log.trace("drawing string \"e-Paper Demo\" normal OK.");

	Log.trace("e-Paper drawing transparent square with lines...");
  paint.SetWidth(64);
  paint.SetHeight(64);
  paint.Clear(UNCOLORED);
  paint.DrawRectangle(0, 0, 40, 40, COLORED);
  paint.DrawLine(0, 0, 40, 40, COLORED);
  paint.DrawLine(40, 0, 0, 40, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 8, 60, paint.GetWidth(), paint.GetHeight());
	Log.trace("e-Paper drawing transparent square with lines OK.");

	Log.trace("e-Paper drawing transparent circle...");
  paint.Clear(UNCOLORED);
  paint.DrawCircle(32, 32, 30, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 60, 50, paint.GetWidth(), paint.GetHeight());
	Log.trace("e-Paper drawing opaque circle OK.");

	Log.trace("e-Paper drawing opaque square...");
  paint.Clear(UNCOLORED);
  paint.DrawFilledRectangle(0, 0, 40, 40, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 8, 130, paint.GetWidth(), paint.GetHeight());
	Log.trace("e-Paper drawing opaque square OK.");

	Log.trace("e-Paper drawing opaque circle...");
  paint.Clear(UNCOLORED);
  paint.DrawFilledCircle(32, 32, 30, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 60, 120, paint.GetWidth(), paint.GetHeight());
	Log.trace("e-Paper drawing opaque circle OK.");

	Log.trace("e-Paper performing full update...");
	epd.DisplayFrame();
	Log.trace("e-Paper performing full update OK.");


	for (int i = 5; i>0; i--) {
		Log.trace("continue setup() in %d seconds...", i);
		delay(1000);
	}


	Log.trace("e-Paper ---- partial update mode ----");

	Log.trace("e-Paper initializing for partial updates...");
  if (epd.Init(lut_partial_update) != 0) {
      Log.error("e-Paper initializing for partial updates FAILED");
      return;
  }
	Log.trace("e-Paper initializing for partial updates OK.");

  /**
   *  there are 2 memory areas embedded in the e-paper display
   *  and once the display is refreshed, the memory area will be auto-toggled,
   *  i.e. the next action of SetFrameMemory will set the other memory area
   *  therefore you have to set the frame memory and refresh the display twice.
   */
	Log.trace("e-Paper drawing imgage...");
  epd.SetFrameMemory(IMAGE_DATA);
  epd.DisplayFrame();
  epd.SetFrameMemory(IMAGE_DATA);
  epd.DisplayFrame();
	Log.trace("e-Paper drawing imgage OK.");

}


#define UPDATE_EVERY_MS 1000
unsigned long _nextUpdateMillis = 0;

void loop() {

	unsigned long millisNow = millis();
	if(_nextUpdateMillis > millisNow) return;
	_nextUpdateMillis = millisNow + UPDATE_EVERY_MS;
	if(Time.second() % 5 != 0) return;

	static bool firstLoop = true;
	if(firstLoop) {
		Log.trace("e-Paper updating time continuously every 5 seconds...");
		firstLoop = false;
		paint.SetWidth(32);
	  paint.SetHeight(144);
	  paint.SetRotate(ROTATE_90);
	}
	Log.trace("e-Paper partially updating current time...");
  char time_string[9];
  snprintf(time_string, sizeof(time_string), "%02d:%02d:%02d", Time.hour(), Time.minute(), Time.second());
	paint.Clear(UNCOLORED);
  paint.DrawStringAt(0, 4, time_string, &Font24, COLORED);
  epd.SetFrameMemory(paint.GetImage(), 96, 0, paint.GetWidth(), paint.GetHeight());
  epd.DisplayFrame();
	Log.trace("e-Paper partially updating current time OK.");

}
