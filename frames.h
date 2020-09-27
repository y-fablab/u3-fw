//
//  frames.h
//
//  Created by Gabriele Mondada on September 17, 2020
//  Copyright © 2020 Gabriele Mondada
//  Distributed under the terms of the MIT License (https://opensource.org/licenses/MIT)
//

#pragma once
#include "Arduino.h"
#include <cstdint>

class BitFrameBase {
public:
    int width;
    int height;
    uint8_t data[0];

    int getWidth() const { return width; };
    int getHeight() const { return height; }
    int getRowSize() const { return (width + 7) / 8; }

    bool getBit(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return false;
        return data[x / 8 + getRowSize() * y] & (0x80 >> (x % 8));
    }

    void setBit(int x, int y, bool value) {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return;
        if (value)
            data[x / 8 + getRowSize() * y] |= 0x80 >> (x % 8);
        else
            data[x / 8 + getRowSize() * y] &= ~(0x80 >> (x % 8));
    }

    void paint(int x, int y, const BitFrameBase& frame) {
        int x_min = x;
        if (x_min < 0)
            x_min = 0;
        int y_min = y;
        if (y_min < 0)
            y_min = 0;
        int x_max = x + frame.getWidth();
        if (x_max > width)
            x_max = width;
        int y_max = y + frame.getHeight();
        if (y_max > height)
            y_max = height;
        int dx = x_max - x_min;
        int dy = y_max - y_min;
        for (int iy = 0; iy < dy; iy++) {
            for (int ix = 0; ix < dx; ix++) {
                bool b = frame.getBit(ix + x_min - x, iy + y_min - y);
                setBit(ix + x_min, iy + y_min, b);
            }
        }
    }
};

template <int _width, int _height> class BitFrame : public BitFrameBase {
public:
    uint8_t data[(_width + 7) / 8 * _height];

    BitFrame() {
        clear();
    }

    BitFrame(const uint8_t (& arr)[(_width + 7) / 8 * _height]) {
        width = _width;
        height = _height;
        memcpy(data, &arr, (_width + 7) / 8 * _height);
    }

    int getWidth() { return _width; }
    int getHeight() { return _height; }
    int getRowSize() { return (_width + 7) / 8; }

    void clear() {
        memset(data, 0, sizeof(data));
    }

    void fill() {
        memset(data, -1, sizeof(data));
    }

    void random() {
        for (int i=0; i<sizeof(data); i++) {
            data[i] = ::random(0x100);
        }
    }

    bool getBit(int x, int y) {
        if (x < 0 || x >= _width || y < 0 || y >= _height)
            return false;
        return data[x / 8 + getRowSize() * y] & (0x80 >> (x % 8));
    }

    void setBit(int x, int y, bool value) {
        if (x < 0 || x >= _width || y < 0 || y >= _height)
            return;
        if (value)
            data[x / 8 + getRowSize() * y] |= 0x80 >> (x % 8);
        else
            data[x / 8 + getRowSize() * y] &= ~(0x80 >> (x % 8));
    }

    template <typename T>
    void paint(int x, int y, T& frame) {
        int x_min = x;
        if (x_min < 0)
            x_min = 0;
        int y_min = y;
        if (y_min < 0)
            y_min = 0;
        int x_max = x + frame.getWidth();
        if (x_max > _width)
            x_max = _width;
        int y_max = y + frame.getHeight();
        if (y_max > _height)
            y_max = _height;
        int dx = x_max - x_min;
        int dy = y_max - y_min;
        for (int iy = 0; iy < dy; iy++) {
            for (int ix = 0; ix < dx; ix++) {
                bool b = frame.getBit(ix + x_min - x, iy + y_min - y);
                setBit(ix + x_min, iy + y_min, b);
            }
        }
    }
};

template <int width, int height>
class ByteFrame {
public:
    uint8_t data[width * height];


    ByteFrame() {
        clear();
    }

    ByteFrame(const uint8_t (& arr)[width * height]) {
        memcpy(data, &arr, width * height);
    }

    int getWidth() { return width; }
    int getHeight() { return height; }

    void clear() {
        memset(data, 0, sizeof(data));
    }

    uint8_t getByte(int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return 0;
        return data[x + width * y];
    }

    void setByte(int x, int y, uint8_t value) {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return;
        data[x + width * y] = value;
    }

    BitFrame<width, height> &sparkledBitFrame() {
        BitFrame<width, height> ret;
        for (int y = 0; y < width; y++) {
            for (int x = 0; x < width; x++) {
                bool value = random(255) < getByte(x, y);
                ret.setBit(x, y, value);
            }
        }
        return ret;
    }
};
