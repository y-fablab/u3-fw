//
//  font5.cpp
//
//  Created by Gabriele Mondada on September 17, 2020
//  Copyright © 2020 Gabriele Mondada
//  Distributed under the terms of the MIT License (https://opensource.org/licenses/MIT)
//

#include "font5.h"


BitFrame<3, 5> Font5::digit_1x5_colon({
    0b00000000,
    0b10000000,
    0b00000000,
    0b10000000,
    0b00000000,
});

BitFrame<4, 5> Font5::digit_4x5_0({
    0b01100000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b01100000,
});

BitFrame<4, 5> Font5::digit_4x5_1({
    0b00100000,
    0b01100000,
    0b00100000,
    0b00100000,
    0b00100000,
});

BitFrame<4, 5> Font5::digit_4x5_2({
    0b11100000,
    0b00010000,
    0b00100000,
    0b01000000,
    0b11110000,
});

BitFrame<4, 5> Font5::digit_4x5_3({
    0b11100000,
    0b00010000,
    0b01100000,
    0b00010000,
    0b11100000,
});

BitFrame<4, 5> Font5::digit_4x5_4({
    0b00100000,
    0b01100000,
    0b10100000,
    0b11110000,
    0b00100000,
});

BitFrame<4, 5> Font5::digit_4x5_5({
    0b11110000,
    0b10000000,
    0b11100000,
    0b00010000,
    0b11100000,
});

BitFrame<4, 5> Font5::digit_4x5_6({
    0b01100000,
    0b10000000,
    0b11100000,
    0b10010000,
    0b01100000,
});

BitFrame<4, 5> Font5::digit_4x5_7({
    0b11110000,
    0b00010000,
    0b00100000,
    0b01000000,
    0b01000000,
});

BitFrame<4, 5> Font5::digit_4x5_8({
    0b01100000,
    0b10010000,
    0b11110000,
    0b10010000,
    0b01100000,
});

BitFrame<4, 5> Font5::digit_4x5_9({
    0b01100000,
    0b10010000,
    0b01110000,
    0b00010000,
    0b01100000,
});

/*
BitFrame<4, 8> Font5::letter_4x8_a({
    0b00000000,
    0b00000000,
    0b01100000,
    0b10010000,
    0b10010000,
    0b01110000,
    0b00000000,
    0b00000000,
});

BitFrame<4, 8> Font5::letter_4x8_b({
    0b10000000,
    0b10000000,
    0b11100000,
    0b10010000,
    0b10010000,
    0b11100000,
    0b00000000,
    0b00000000,
});

BitFrame<4, 8> Font5::letter_4x8_c({
    0b00000000,
    0b00000000,
    0b01110000,
    0b10000000,
    0b10000000,
    0b01110000,
    0b00000000,
    0b00000000,
});

BitFrame<4, 8> Font5::letter_4x8_d({
    0b00010000,
    0b00010000,
    0b01110000,
    0b10010000,
    0b10010000,
    0b01110000,
    0b00000000,
    0b00000000,
});
*/

BitFrame<5, 5> Font5::letter_5x5_A({
    0b00100000,
    0b01010000,
    0b11111000,
    0b10001000,
    0b10001000,
});

BitFrame<4, 5> Font5::letter_4x5_B({
    0b11100000,
    0b10010000,
    0b11100000,
    0b10010000,
    0b11100000,
});

BitFrame<4, 5> Font5::letter_4x5_C({
    0b01110000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b01110000,
});

BitFrame<4, 5> Font5::letter_4x5_D({
    0b11100000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b11100000,
});

BitFrame<4, 5> Font5::letter_4x5_E({
    0b11110000,
    0b10000000,
    0b11100000,
    0b10000000,
    0b11110000,
});

BitFrame<4, 5> Font5::letter_4x5_F({
    0b11110000,
    0b10000000,
    0b11100000,
    0b10000000,
    0b10000000,
});

BitFrame<4, 5> Font5::letter_4x5_G({
    0b01110000,
    0b10000000,
    0b10110000,
    0b10010000,
    0b01110000,
});

BitFrame<4, 5> Font5::letter_4x5_H({
    0b10010000,
    0b10010000,
    0b11110000,
    0b10010000,
    0b10010000,
});

BitFrame<1, 5> Font5::letter_1x5_I({
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
});

BitFrame<4, 5> Font5::letter_4x5_J({
    0b00010000,
    0b00010000,
    0b00010000,
    0b10010000,
    0b01100000,
});

BitFrame<4, 5> Font5::letter_4x5_K({
    0b10010000,
    0b10100000,
    0b11000000,
    0b10100000,
    0b10010000,
});

BitFrame<4, 5> Font5::letter_4x5_L({
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b11110000,
});

BitFrame<5, 5> Font5::letter_5x5_M({
    0b10001000,
    0b11011000,
    0b10101000,
    0b10001000,
    0b10001000,
});

BitFrame<5, 5> Font5::letter_5x5_N({
    0b10001000,
    0b11001000,
    0b10101000,
    0b10011000,
    0b10001000,
});

BitFrame<4, 5> Font5::letter_4x5_O({
    0b01100000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b01100000,
});

BitFrame<4, 5> Font5::letter_4x5_P({
    0b11100000,
    0b10010000,
    0b11100000,
    0b10000000,
    0b10000000,
});

BitFrame<4, 5> Font5::letter_4x5_Q({
    0b01100000,
    0b10010000,
    0b10010000,
    0b10110000,
    0b01110000,
});

BitFrame<4, 5> Font5::letter_4x5_R({
    0b11100000,
    0b10010000,
    0b11100000,
    0b10100000,
    0b10010000,
});

BitFrame<4, 5> Font5::letter_4x5_S({
    0b01110000,
    0b10000000,
    0b01100000,
    0b00010000,
    0b11100000,
});

BitFrame<5, 5> Font5::letter_5x5_T({
    0b11111000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
});

BitFrame<4, 5> Font5::letter_4x5_U({
    0b10010000,
    0b10010000,
    0b10010000,
    0b10010000,
    0b01100000,
});

BitFrame<5, 5> Font5::letter_5x5_V({
    0b10001000,
    0b10001000,
    0b10001000,
    0b01010000,
    0b00100000,
});

BitFrame<5, 5> Font5::letter_5x5_W({
    0b10001000,
    0b10001000,
    0b10101000,
    0b10101000,
    0b01010000,
});

BitFrame<5, 5> Font5::letter_5x5_X({
    0b10001000,
    0b01010000,
    0b00100000,
    0b01010000,
    0b10001000,
});

BitFrame<5, 5> Font5::letter_5x5_Y({
    0b10001000,
    0b01010000,
    0b00100000,
    0b00100000,
    0b00100000,
});

BitFrame<5, 5> Font5::letter_5x5_Z({
    0b11111000,
    0b00010000,
    0b00100000,
    0b01000000,
    0b11111000,
});

BitFrame<5, 5> Font5::letter_5x5_question_mark({
    0b01110000,
    0b10001000,
    0b00010000,
    0b00000000,
    0b00100000,
});

BitFrame<4, 5> Font5::letter_4x5_dash({
    0b00000000,
    0b00000000,
    0b11110000,
    0b00000000,
    0b00000000,
});

BitFrame<1, 5> Font5::letter_1x5_dot({
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b10000000,
});

const BitFrameBase& Font5::alpha(char c) {
    switch (c) {
        case 'A': return letter_5x5_A;
        case 'B': return letter_4x5_B;
        case 'C': return letter_4x5_C;
        case 'D': return letter_4x5_D;
        case 'E': return letter_4x5_E;
        case 'F': return letter_4x5_F;
        case 'G': return letter_4x5_G;
        case 'H': return letter_4x5_H;
        case 'I': return letter_1x5_I;
        case 'J': return letter_4x5_J;
        case 'K': return letter_4x5_K;
        case 'L': return letter_4x5_L;
        case 'M': return letter_5x5_M;
        case 'N': return letter_5x5_N;
        case 'O': return letter_4x5_O;
        case 'P': return letter_4x5_P;
        case 'Q': return letter_4x5_Q;
        case 'R': return letter_4x5_R;
        case 'S': return letter_4x5_S;
        case 'T': return letter_5x5_T;
        case 'U': return letter_4x5_U;
        case 'V': return letter_5x5_V;
        case 'W': return letter_5x5_W;
        case 'X': return letter_5x5_X;
        case 'Y': return letter_5x5_Y;
        case 'Z': return letter_5x5_Z;
        case '-': return letter_4x5_dash;
        case '.': return letter_1x5_dot;
        default:  return letter_5x5_question_mark;
    }
}
