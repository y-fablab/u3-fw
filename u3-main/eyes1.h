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

enum EyeExpression {
    EyeExpressionNeutral = 0,
    EyeExpressionAstonish,
    EyeExpressionAngry,
    EyeExpressionCry,
    EyeExpressionSadLevel1,
    EyeExpressionSadLevel2,
    EyeExpressionTired,
    EyeExpressionHappy,
    EyeExpressionSqueeze,
};

class Eye : public BitFrame<8, 8> {
    friend Eyes;
private:
    int opening = 6; // ouverture de l'oeil, de 0 à 6
    int dirX = 0;    // direction du regard en X, de -1 à 1
    int dirY = 0;    // direction du regard en X, de -1 à 1
    EyeExpression expression = EyeExpressionNeutral;
    bool right = false;

public:
    void setDirection(int x, int y) {
        dirX = max(-1, min(1, x));
        dirY = max(-1, min(1, y));
    }
    void setDirection(int dir) {
        switch (dir) {
        default:
            setDirection(0, 0);
            break;
        case 0:
            setDirection(0, -1);
            break;
        case 1:
            setDirection(1, -1);
            break;
        case 2:
            setDirection(1, 0);
            break;
        case 3:
            setDirection(1, 1);
            break;
        case 4:
            setDirection(0, 1);
            break;
        case 5:
            setDirection(-1, 1);
            break;
        case 6:
            setDirection(-1, 0);
            break;
        case 7:
            setDirection(-1, -1);
            break;
        }
    }
    void setOpening(int op) {
        opening = max(0, min(6, op));
    }
    void setExpression(EyeExpression expr) {
        expression = expr;
    }
    void setRight(bool right) {
        this->right = right;
    }
    void render();
};

class Eyes : public BitFrame<16, 8> {
public:
    Eye left;
    Eye right;

    Eyes() {
        right.setRight(true);
    }
    void render() {
        left.render();
        right.render();
        paint(0, 0, left);
        paint(8, 0, right);
    }
    void show(MX& mx) {
        render();
        mx.show(*this);
    }
    void setDirection(int x, int y) {
        left.setDirection(x, y);
        right.setDirection(x, y);
    }
    void setDirection(int dir) {
        left.setDirection(dir);
        right.setDirection(dir);
    }
    void setOpening(int op) {
        left.setOpening(op);
        right.setOpening(op);
    }
    void setExpression(EyeExpression expr) {
        left.setExpression(expr);
        right.setExpression(expr);
    }
    void showBlinkAnimation(MX& mx, bool left = true, bool right = true);
};
