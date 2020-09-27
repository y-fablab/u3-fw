//
//  eyes1.cpp
//
//  Created by Gabriele Mondada on September 17, 2020
//  Copyright © 2020 Gabriele Mondada
//  Distributed under the terms of the MIT License (https://opensource.org/licenses/MIT)
//

#include "eyes1.h"
#include <cstdbool>


void Eye::render() {

    // compute centra part, rows 2, 3, 4 and 5

    int posX = dirX + 1;
    posX = max(0, min(2, posX));
    int posY = dirY + 1;
    posY = max(0, min(2, posY));

    uint8_t center[4] = { 0, 0, 0, 0 };
    center[posY] = 0b00110000 >> posX;
    center[posY + 1] = 0b00110000 >> posX;

    // compute entire eye

    int startY = 7 - opening;
    for (int y=0; y<8; y++) {
        if (y == 0 || y == 7) {
            data[y] = 0;
        } else if (y < startY) {
            data[y] = 0;
        } else if (y == startY) {
            data[y] = 0b00111100;
        } else if (y == 6) {
            data[y] = 0b00111100;
        } else {
            data[y] = 0b01111110 & ~center[y - 2];
        }
    }
}

void Eyes::showBlinkAnimation(MX &mx, bool left, bool right) {
    for (int i=6; i>=1; i--) {
        if (left)
            this->left.setOpening(i);
        if (right)
            this->right.setOpening(i);
        show(mx);
        delay(20);
    }
    for (int i=2; i<=6; i++) {
        if (left)
            this->left.setOpening(i);
        if (right)
            this->right.setOpening(i);
        show(mx);
        delay(20);
    }
}
