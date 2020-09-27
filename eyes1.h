//
//  eyes1.h
//
//  Created by Gabriele Mondada on September 17, 2020
//  Copyright © 2020 Gabriele Mondada
//  Distributed under the terms of the MIT License (https://opensource.org/licenses/MIT)
//

#pragma once
#include "Arduino.h"
#include <cstdint>
#include <cstring>
#include "mx.h"
#include "frames.h"


class Eyes;

class Eye : public BitFrame<8, 8> {
    friend Eyes;
private:
    int opening = 6; // ouverture de l'oeil, de 0 à 6
    int dirX = 0;    // direction du regard en X, de -1 à 1
    int dirY = 0;    // direction du regard en X, de -1 à 1
    void render();

public:
    void setDirection(int x, int y) {
        dirX = max(-1, min(1, x));
        dirY = max(-1, min(1, y));
    }
    void setOpening(int op) {
        opening = max(0, min(6, op));
    }
};

class Eyes : public BitFrame<16, 8> {
public:
    Eye left;
    Eye right;

    void show(MX& mx) {
        left.render();
        right.render();
        paint(0, 0, left);
        paint(8, 0, right);
        mx.show(*this);
    }
    void setDirection(int x, int y) {
        left.setDirection(x, y);
        right.setDirection(x, y);
    }
    void setOpening(int op) {
        left.setOpening(op);
        right.setOpening(op);
    }
    void showBlinkAnimation(MX& mx, bool left = true, bool right = true);
};
