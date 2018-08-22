/**
 *  @filename   :   epdif.cpp
 *  @brief      :   Implements EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     August 10 2017
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
 */

#include "epdif.h"

EpdIf::~EpdIf() {
  
};

void EpdIf::DigitalWrite(int pin, int value) {
    digitalWriteFast(pin, value);
}

int EpdIf::DigitalRead(int pin) {
    return pinReadFast(pin);
}

void EpdIf::DelayMs(unsigned int delaytime) {
    delay(delaytime);
}

void EpdIf::SpiTransfer(unsigned char data) {
    digitalWrite(_pin_disp_cs, LOW);
    _spiPort.transfer(data);
    digitalWrite(_pin_disp_cs, HIGH);
}

int EpdIf::IfInit(void) {
    pinMode(_pin_disp_cs, OUTPUT);
    pinMode(_pin_disp_rst, OUTPUT);
    pinMode(_pin_disp_dc, OUTPUT);
    pinMode(_pin_disp_bsy, INPUT);
    // End existing transaction. Important when reinitializing.
    // On scenario is when a reinitialization needs to be done
    // to switch from full updates to partial updates.
    if (!_1stInitialized) {
      _1stInitialized = true;
    }
    else {
      _spiPort.end();
      _spiPort.endTransaction();
    }
    _spiPort.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
    _spiPort.begin();
    return 0;
}

/* END OF FILE */
