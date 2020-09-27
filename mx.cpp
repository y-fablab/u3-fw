//
//  mx.cpp
//
//  Created by Gabriele Mondada on September 17, 2020
//  Copyright © 2020 Gabriele Mondada
//  Distributed under the terms of the MIT License (https://opensource.org/licenses/MIT)
//

#include "mx.h"


// warning: do not use SS on Arduino
#define SPI_CS 4 // GPIO4 = D2


void MX::init(void) {
    digitalWrite(SPI_CS, LOW);
    pinMode(SPI_CS, OUTPUT);
    SPI.begin();
    writeCmd(0x0c00);
    writeCmd(0x0900);
    writeCmd(0x0a03); // intensity
    writeCmd(0x0b07);
    clear();
    writeCmd(0x0c01);
}

/**
 * Write the same value on both left and right controllers.
 */
void MX::writeCmd(uint16_t data) {
    SPI.beginTransaction(spiSettings);
    digitalWrite(SPI_CS, LOW);
    SPI.transfer16(data);
    SPI.transfer16(data);
    digitalWrite(SPI_CS, HIGH);
    SPI.endTransaction();
}

void MX::writeRow(int row, uint16_t pattern) {
    SPI.beginTransaction(spiSettings);
    digitalWrite(SPI_CS, LOW);
    uint16_t addr = (row + 1) << 8;
    uint16_t left = pattern & 0xff;
    uint16_t right = pattern >> 8;
    SPI.transfer16(addr | left);
    SPI.transfer16(addr | right);
    digitalWrite(SPI_CS, HIGH);
    SPI.endTransaction();
}

void MX::show(const BitFrame<16, 8>& frame) {
    for (int i=0; i<8; i++) {
        writeRow(i, frame.data[2 * i + 1] << 8 | frame.data[2 * i]);
    }
}

void MX::clear() {
    for (int i=0; i<8; i++) {
        writeRow(i, 0);
    }
}
