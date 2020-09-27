//
//  mx.h
//
//  Created by Gabriele Mondada on September 17, 2020
//  Copyright © 2020 Gabriele Mondada
//  Distributed under the terms of the MIT License (https://opensource.org/licenses/MIT)
//

#pragma once
#include <cstdint>
#include <cstring>
#include <SPI.h>
#include "Arduino.h"
#include "frames.h"


class MX {
private:
    SPISettings spiSettings;

public:
    MX() : spiSettings(1000000, MSBFIRST, SPI_MODE0) {}
    void init();
    void writeCmd(uint16_t data);
    void writeRow(int row, uint16_t pattern);
    void show(const BitFrame<16, 8>& frame);
    void clear();
};
