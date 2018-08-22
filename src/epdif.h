/**
 *  @filename   :   epdif.h
 *  @brief      :   Header file of epdif.cpp providing EPD interface functions
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

#ifndef EPDIF_H
#define EPDIF_H

#include <Arduino.h> // for int16_t
#include <SPI.h> // for SPIClass

class EpdIf {

public:

    EpdIf(
        SPIClass& spiPort,
        // dispDinPin, // mapped to SPI MOSI pin (Photon A5)
        // dispClkPin, // mapped to SPI SCLK pin (Photon A3)
        int16_t dispCsPin, // mapped to SPI SS pin (Photon A2)
        int16_t dispDcPin,
        int16_t dispResetPin,
        int16_t dispBusyPin
    ) : _spiPort(spiPort),
        // dispDinPin,
        // dispClkPin,
        _pin_disp_cs(dispCsPin),
        _pin_disp_dc(dispDcPin),
        _pin_disp_rst(dispResetPin),
        _pin_disp_bsy(dispBusyPin)
    {}

    ~EpdIf(void);

    int  IfInit(void);
    void DigitalWrite(int pin, int value);
    int  DigitalRead(int pin);
    void DelayMs(unsigned int delaytime);
    void SpiTransfer(unsigned char data);

protected:
    SPIClass& _spiPort;
    int16_t _pin_disp_cs;
    int16_t _pin_disp_dc;
    int16_t _pin_disp_rst;
    int16_t _pin_disp_bsy;
    bool _1stInitialized;

};

#endif // EPDIF_H

/* END OF FILE */
