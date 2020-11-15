//
//  gfx.cpp
//
//  Created by Gabriele Mondada on September 17, 2020
//  Copyright © 2020 Gabriele Mondada
//  Distributed under the terms of the MIT License (https://opensource.org/licenses/MIT)
//

#include "gfx.h"
#include "frames.h"
#include "font5.h"


static uint8_t profile[] = {
    0,
    64, 128, 192, 255, 255, 192, 128, 64, 0, 0, 0,
    64, 128, 192, 255, 255, 192, 128, 64, 0, 0, 0,
    32, 64, 72, 128, 160, 192,
    255, 255, 255, 255, 192, 255, 192, 255, 192,
    255, 255, 255, 255, 192, 255, 192, 255, 192,
    255, 255, 255, 255, 192, 255, 192, 255, 192,
    255, 255, 32, 255, 255, 255, 255, 255,
    255, 255, 32, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
    255, 255, 255, 255, 255, 255, 255, 255,
};


static float hill(float x) {
    return 1.5f * (1.0f - cosf((float)M_PI * 2.0f *x));
}

void showExplosionAnimation(MX& mx) {
    ByteFrame<16, 8> f;
    int len = sizeof(profile) - 8 + 1;
    for (int t=0; t<len; t++) {
        for (int y = 0; y < 8; y++) {
            for (int x = 0; x < 16; x++) {
                float dx = fabsf(7.5f - (float)x);
                float dy = fabsf(3.5f - (float)y);
                float r = sqrtf(dx * dx + dy * dy);

                int d = (int)floorf(r);
                if (d > 8)
                    d = 8;

                int a = t - d;
                if (a < 0)
                    a = 0;
                if (a > len)
                    a = len;

                f.setByte(x, y, profile[a]);
            }
        }
        auto mask = f.sparkledBitFrame();
        mx.show(mask);
        delay(25);
    }

    delay(1000);

    for (int t = 0; t < 160; t++) {
        int value_max = 255 - t * 255 / 40;
        for (int y = 0; y < 8; y++) {
            int value = value_max + y * 255 / 4;
            if (value < 0)
                value = 0;
            if (value > 255)
                value = 255;
            for (int x = 0; x < 16; x++) {
                f.setByte(x, y, value);
            }
        }
        auto mask = f.sparkledBitFrame();
        mx.show(mask);
        delay(25);
    }
}

void show3DigitTime(int t, MX& mx) {
    BitFrame<16, 8> textFrame;

    int digit1 = t / 60;
    int digit2 = (t % 60) / 10;
    int digit3 = t % 10;

    textFrame.paint(0, 1, Font5::digit(digit1));
    textFrame.paint(5, 1, Font5::digit_1x5_colon);
    textFrame.paint(7, 1, Font5::digit(digit2));
    textFrame.paint(12, 1, Font5::digit(digit3));
    mx.show(textFrame);
}

void showScrollingText(std::string str, MX& mx) {
    BitFrame<16, 8> frame;

    int offset = -16;
    for (;;) {
        int len = 0;

        frame.clear();

        for (std::string::iterator it = str.begin(); it < str.end(); it++) {
            const BitFrameBase& glyph = Font5::alpha(*it);
            frame.paint(len - offset, 2, glyph);
            len += glyph.width + 1;
        }

        mx.show(frame);

        delay(50);
        offset++;
        if (offset > len)
            break;
    }
}

void showQuestionMarks(MX& mx) {
  BitFrame<16, 8> screen({
    0b00011100, 0b00111000,
    0b00100010, 0b01000100,
    0b00000010, 0b00000100,
    0b00000100, 0b00001000,
    0b00001000, 0b00010000,
    0b00001000, 0b00010000,
    0b00000000, 0b00000000,
    0b00001000, 0b00010000,
  });
  mx.show(screen);
}

void showThunders(MX& mx) {
  BitFrame<16, 8> screen({
    0b00000100, 0b00001000,
    0b00001000, 0b00010000,
    0b00010000, 0b00100000,
    0b00111110, 0b01111100,
    0b00000100, 0b00001000,
    0b00001000, 0b00010000,
    0b00010000, 0b00100000,
    0b00100000, 0b01000000,
  });
  mx.show(screen);
}

void showHearts(bool full, MX& mx) {
    if (full) {
        BitFrame<16, 8> screen({
            0b00000000, 0b00000000,
            0b01101100, 0b00110110,
            0b11111110, 0b01111111,
            0b11111110, 0b01111111,
            0b11111110, 0b01111111,
            0b01111100, 0b00111110,
            0b00111000, 0b00011100,
            0b00010000, 0b00001000,
        });
        mx.show(screen);
    } else {
        BitFrame<16, 8> screen({
            0b00000000, 0b00000000,
            0b01101100, 0b00110110,
            0b10010010, 0b01001001,
            0b10000010, 0b01000001,
            0b10000010, 0b01000001,
            0b01000100, 0b00100010,
            0b00101000, 0b00010100,
            0b00010000, 0b00001000,
        });
        mx.show(screen);
    }
}
