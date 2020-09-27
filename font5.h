//
//  font5.h
//
//  Created by Gabriele Mondada on September 17, 2020
//  Copyright © 2020 Gabriele Mondada
//  Distributed under the terms of the MIT License (https://opensource.org/licenses/MIT)
//

#pragma once
#include "frames.h"


namespace Font5 {

    extern BitFrame<3, 5> digit_1x5_colon;
    extern BitFrame<4, 5> digit_4x5_0;
    extern BitFrame<4, 5> digit_4x5_1;
    extern BitFrame<4, 5> digit_4x5_2;
    extern BitFrame<4, 5> digit_4x5_3;
    extern BitFrame<4, 5> digit_4x5_4;
    extern BitFrame<4, 5> digit_4x5_5;
    extern BitFrame<4, 5> digit_4x5_6;
    extern BitFrame<4, 5> digit_4x5_7;
    extern BitFrame<4, 5> digit_4x5_8;
    extern BitFrame<4, 5> digit_4x5_9;

    extern BitFrame<5, 5> letter_5x5_A;
    extern BitFrame<4, 5> letter_4x5_B;
    extern BitFrame<4, 5> letter_4x5_C;
    extern BitFrame<4, 5> letter_4x5_D;
    extern BitFrame<4, 5> letter_4x5_E;
    extern BitFrame<4, 5> letter_4x5_F;
    extern BitFrame<4, 5> letter_4x5_G;
    extern BitFrame<4, 5> letter_4x5_H;
    extern BitFrame<1, 5> letter_1x5_I;
    extern BitFrame<4, 5> letter_4x5_J;
    extern BitFrame<4, 5> letter_4x5_K;
    extern BitFrame<4, 5> letter_4x5_L;
    extern BitFrame<5, 5> letter_5x5_M;
    extern BitFrame<5, 5> letter_5x5_N;
    extern BitFrame<4, 5> letter_4x5_O;
    extern BitFrame<4, 5> letter_4x5_P;
    extern BitFrame<4, 5> letter_4x5_Q;
    extern BitFrame<4, 5> letter_4x5_R;
    extern BitFrame<4, 5> letter_4x5_S;
    extern BitFrame<5, 5> letter_5x5_T;
    extern BitFrame<4, 5> letter_4x5_U;
    extern BitFrame<5, 5> letter_5x5_V;
    extern BitFrame<5, 5> letter_5x5_W;
    extern BitFrame<5, 5> letter_5x5_X;
    extern BitFrame<5, 5> letter_5x5_Y;
    extern BitFrame<5, 5> letter_5x5_Z;
    extern BitFrame<5, 5> letter_5x5_question_mark;
    extern BitFrame<4, 5> letter_4x5_dash;
    extern BitFrame<1, 5> letter_1x5_dot;

    static inline BitFrame<4, 5>& digit(int value) {
        switch (value) {
            case 0: return digit_4x5_0;
            case 1: return digit_4x5_1;
            case 2: return digit_4x5_2;
            case 3: return digit_4x5_3;
            case 4: return digit_4x5_4;
            case 5: return digit_4x5_5;
            case 6: return digit_4x5_6;
            case 7: return digit_4x5_7;
            case 8: return digit_4x5_8;
            case 9: return digit_4x5_9;
        }
    }

    const BitFrameBase& alpha(char c);
};
