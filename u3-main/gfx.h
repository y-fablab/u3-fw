//
//  gfx.h
//
//  Created by Gabriele Mondada on September 17, 2020
//  Copyright © 2020 Gabriele Mondada
//  Distributed under the terms of the MIT License (https://opensource.org/licenses/MIT)
//

#pragma once
#include <string>
#include "Arduino.h"
#include "mx.h"


void showExplosionAnimation(MX& mx);
void show3DigitTime(int t, MX& mx);
void showScrollingText(std::string str, MX& mx);
void showQuestionMarks(MX& mx);
void showThunders(MX& mx);
void showHearts(bool full, MX& mx);
