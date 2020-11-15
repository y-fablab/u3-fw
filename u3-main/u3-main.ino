
#include <Servo.h>
#include <EEPROM.h>
#include "JQ6500_Serial.h"
#include "mx.h"
#include "eyes1.h"
#include "frames.h"
#include "font5.h"
#include "gfx.h"


/*** literals ***/

#define GPIO_SERVO     15  // GPIO15 = D8
#define GPIO_SHUTDOWN   0  // GPIO0  = D3
#define GPIO_MP3_TX     2  // GPIO2  = D4
#define GPIO_MP3_RX     5  // GPIO5  = D1
#define GPIO_SWITCH    16  // GPIO16 = D0

#define SERVO_HOME       15
#define SERVO_OPEN_10MM  50
#define SERVO_PUNCH      150


/*** types ***/

enum SeqType {
    SEQ_TYPE_SHUTDOWN,
    SEQ_TYPE_WELCOME,   // lors de l'enclanchement
    SEQ_TYPE_BYE,       // lorsque le punch réussit
    SEQ_TYPE_DECEPTION, // lorsqu'on éteint avant le punch
    SEQ_TYPE_ASTONISH,  // lorsqu'on réallume après un DECEPTION
    SEQ_TYPE_ANGRY,     // lorsqu'on empêche le punch de switcher off
    SEQ_TYPE_DELIRIUM,  // lorsqu'on est en ANGRY et qu'on éteint la boîte
    SEQ_TYPE_MAX
};

struct Seq {
    SeqType type;
    SeqType (* func)();
    int probability;
};


/*** globals ***/

MX mx;
Servo servo;
JQ6500_Serial mp3(GPIO_MP3_RX, GPIO_MP3_TX);

extern Seq seqList[];
extern int seqCount;


/*** functions ***/

void mp3Init() {
    static bool done = false;
    if (done) {
        // mp3 module already initialized - simulate the delay needed to initialize
        delay(1250);
    } else {
        // first call - we really initialize the mp3 module
        mp3.begin(9600);
        mp3.reset();
        mp3.setVolume(15);
        mp3.setLoopMode(MP3_LOOP_NONE);
        done = true;
    }
}

void mp3FastPlay(int index) {
    static bool first = true;
    if (first) {
        Serial1.begin(9600);
        first = false;
    }
    Serial1.write((byte)0x7E);
    Serial1.write((byte)0x04);
    Serial1.write((byte)0x03);
    Serial1.write((byte)(index >> 8));
    Serial1.write((byte)(index >> 0));
    Serial1.write((byte)0xEF);
}

bool isFlipSwitchOn() {
    return digitalRead(GPIO_SWITCH);
}

void shutDown() {

    // set the shutdown signal

    pinMode(GPIO_SHUTDOWN, OUTPUT);
    digitalWrite(GPIO_SHUTDOWN, LOW);

    delay(200);

    /*
     * If we reach this point, we failed to switch the box off.
     * Probably, the user has switched the flip switch on again, or the board is powered via USB.
     */

    if (isFlipSwitchOn()) {

        /*
         * The flip switch is on again.
         * Reset the shutdown signal.
         */

        digitalWrite(GPIO_SHUTDOWN, HIGH);
        pinMode(GPIO_SHUTDOWN, INPUT);

    } else {

        /*
         * We are powered by USB. So, no way to shut down.
         * We have to stay on and wait for the user to switch the flip switch on.
         * It's important to keep GPIO_SHUTDOWN down in order to avoid
         * draining the battery.
         */

        BitFrame<16, 8> f;
        f.clear();
        f.setBit(11, 7, true);
        f.setBit(13, 7, true);
        f.setBit(15, 7, true);
        mx.show(f);

        for (;;) {
            delay(50);
            if (isFlipSwitchOn())
                break;
        }

        // reset the shutdown signal

        digitalWrite(GPIO_SHUTDOWN, HIGH);
        pinMode(GPIO_SHUTDOWN, INPUT);
    }
}

void waitFlipSwitchOff(int timeout) {
    unsigned long startTime = millis();
    for (;;) {
        if (millis() - startTime >= timeout)
            break;
        if (!isFlipSwitchOn())
            break;
    }
}

void punchSwitch() {
    servo.write(SERVO_PUNCH);
    waitFlipSwitchOff(700);
    servo.write(SERVO_HOME);
    delay(700);
}

/**
 * Run a sequence of the given type.
 * The sequence is picked from the seqList.
 * If `random` is true, we pick it randomly, avoinding
 * to pick the sequence we just played. Otherwise we pick the
 * next in the list.
 */
SeqType runSeq(SeqType type, bool random) {

    // read index from persistent storage
    EEPROM.begin(SEQ_TYPE_MAX + 1);
    int index = EEPROM.read((int)type);
    if (index >= seqCount)
        index = 0;

    if (random) {
        uint8_t seed = EEPROM.read(SEQ_TYPE_MAX);
        seed++;
        EEPROM.write(SEQ_TYPE_MAX, seed);
        randomSeed(seed);

        int sum = 0;
        for (int i = 0; i < seqCount; i++) {
            if (seqList[i].type == type)
                sum += seqList[i].probability;
        }

        // prevent using the current index
        if (seqList[index].type == type && seqList[index].probability < sum) {
            sum -= seqList[index].probability;
            index++;
            if (index >= seqCount)
                index = 0;
        }

        int step = ::random(sum);

        for (;;) {
            if (seqList[index].type == type && seqList[index].probability > 0) {
                step -= seqList[index].probability;
                if (step < 0)
                    break;
            }
            index++;
            if (index >= seqCount)
                index = 0;
        }
    } else {
        // search next sequence
        for (int i = 0; i < seqCount; i++) {
            index++;
            if (index >= seqCount)
                index = 0;
            if (seqList[index].type == type && seqList[index].probability > 0)
                break;
        }
    }

    // save index persistently
    EEPROM.write((int)type, index);
    EEPROM.end();

    return seqList[index].func();
}

void setup() {
    mx.init();

    servo.write(SERVO_HOME);
    servo.attach(GPIO_SERVO);
    servo.write(SERVO_HOME);
    delay(10); // does it help to avoid movement glitch?

    pinMode(GPIO_SWITCH, INPUT);

    Serial.begin(115200);
}

void loop() {

//  fastPlay(1);
//  mp3.playFileByIndexNumber(5);
//  Serial.println("\nHello !\n");

    SeqType type = SEQ_TYPE_WELCOME;
    bool random = true;

    for (;;) {
        type = runSeq(type, random);

        switch (type) {
            case SEQ_TYPE_SHUTDOWN:
                shutDown();
                return;
            case SEQ_TYPE_DELIRIUM:
                random = false;
                break;
            default:
                random = true;
                break;
        }
    }
}


/*** sequences ***/

SeqType welcome1() {
    Eyes eyes;

    for (int i=0; i<=6; i++) {
        eyes.setOpening(i);
        eyes.show(mx);
        delay(50);
    }

    mp3Init(); // this takes 1250ms

    eyes.showBlinkAnimation(mx);

    delay(500);

    for (int i = 0; i< 10; i++) {
        delay(100);
        if (!isFlipSwitchOn())
            return SEQ_TYPE_DECEPTION;
    }

    punchSwitch();

    if (isFlipSwitchOn())
        return SEQ_TYPE_ANGRY;

    return SEQ_TYPE_BYE;
}

SeqType bye1() {
    Eyes eyes;

    eyes.setExpression(EyeExpressionNeutral);
    eyes.show(mx);

    delay(500);

    eyes.showBlinkAnimation(mx, false, true);

    delay(1000);

    for (int i=6; i>=0; i--) {
        eyes.setOpening(i);
        eyes.show(mx);
        delay(50);
    }

    delay(100);

    return SEQ_TYPE_SHUTDOWN;
}

SeqType deception1() {
    Eyes eyes;

    eyes.setExpression(EyeExpressionCry);
    eyes.show(mx);
    delay(800);
    eyes.setBit(15, 6, true);
    mx.show(eyes);
    delay(150);
    eyes.setBit(15, 7, true);
    mx.show(eyes);
    delay(150);
    eyes.setBit(15, 6, false);
    mx.show(eyes);
    delay(150);
    eyes.setBit(15, 7, false);
    mx.show(eyes);

    for (int i = 0; i< 20; i++) {
        delay(100);
        if (isFlipSwitchOn())
            return SEQ_TYPE_ASTONISH;
    }

    eyes.setExpression(EyeExpressionNeutral);
    eyes.show(mx);

    delay(500);

    for (int i=6; i>=0; i--) {
        eyes.setOpening(i);
        eyes.show(mx);
        delay(50);
    }

    delay(100);

    return SEQ_TYPE_SHUTDOWN;
}

SeqType deception2() {
    BitFrame<4, 4> z4({
        0b11110000,
        0b00100000,
        0b01000000,
        0b11110000,
    });
    BitFrame<5, 5> z5({
        0b11111000,
        0b00010000,
        0b00100000,
        0b01000000,
        0b11111000,
    });
    BitFrame<5, 6> z6({
        0b11111000,
        0b00010000,
        0b00100000,
        0b01000000,
        0b10000000,
        0b11111000,
    });

    BitFrame<16, 8> screen;
    screen.paint(0, 1, z6);
    screen.paint(6, 2, z5);
    screen.paint(12, 3, z4);

    int normalIntentity = mx.getIntensity();
    mx.setIntensity(1);

    mx.show(screen);

    for (int j = 0; j < 3; j++) {
        for (int i = 0; i <= 1000; i++) {
            float t = 0.001f * i;
            float pos = SERVO_HOME + (SERVO_OPEN_10MM - SERVO_HOME) * t;
            servo.write(pos);
            delay(5);
            if (isFlipSwitchOn()) {
                servo.write(SERVO_HOME);
                mx.clear();
                mx.setIntensity(normalIntentity);
                return SEQ_TYPE_ASTONISH;
            }
        }

        for (int i = 0; i< 10; i++) {
            delay(100);
            if (isFlipSwitchOn()) {
                servo.write(SERVO_HOME);
                mx.clear();
                mx.setIntensity(normalIntentity);
                return SEQ_TYPE_ASTONISH;
            }
        }

        for (int i = 0; i <= 1000; i++) {
            float t = 0.001f * (1000 - i);
            float pos = SERVO_HOME + (SERVO_OPEN_10MM - SERVO_HOME) * t;
            servo.write(pos);
            delay(5);
            if (isFlipSwitchOn()) {
                servo.write(SERVO_HOME);
                mx.clear();
                mx.setIntensity(normalIntentity);
                return SEQ_TYPE_ASTONISH;
            }
        }

        for (int i = 0; i< 10; i++) {
            delay(100);
            if (isFlipSwitchOn()) {
                servo.write(SERVO_HOME);
                mx.clear();
                mx.setIntensity(normalIntentity);
                return SEQ_TYPE_ASTONISH;
            }
        }
    }

    servo.write(SERVO_HOME);

    delay(500);

    mx.clear();
    mx.setIntensity(normalIntentity);

    return SEQ_TYPE_SHUTDOWN;
}

SeqType astonish1() {
    Eyes eyes;

    eyes.setExpression(EyeExpressionHappy);
    eyes.show(mx);
    delay(800);
    showHearts(false, mx);
    delay(200);
    showHearts(true, mx);
    delay(200);
    showHearts(false, mx);
    delay(200);
    showHearts(true, mx);
    delay(200);
    showHearts(false, mx);
    delay(200);
    showHearts(true, mx);
    delay(200);
    eyes.show(mx);

    delay(500);

    for (int i = 0; i < 10; i++) {
        delay(100);
        if (!isFlipSwitchOn())
            return SEQ_TYPE_DECEPTION;
    }

    punchSwitch();

    if (isFlipSwitchOn())
        return SEQ_TYPE_ANGRY;

    return SEQ_TYPE_BYE;
}

SeqType astonish2() {
    Eyes eyes;

    eyes.setExpression(EyeExpressionNeutral);
    eyes.show(mx);
    delay(500);
    eyes.setExpression(EyeExpressionAstonish);
    eyes.show(mx);
    delay(500);
    showQuestionMarks(mx);
    delay(150);
    eyes.show(mx);
    delay(100);
    showQuestionMarks(mx);
    delay(150);
    eyes.show(mx);

    delay(500);

    eyes.setExpression(EyeExpressionNeutral);
    eyes.show(mx);

    for (int i = 0; i < 10; i++) {
        delay(100);
        if (!isFlipSwitchOn())
            return SEQ_TYPE_DECEPTION;
    }

    punchSwitch();

    if (isFlipSwitchOn())
        return SEQ_TYPE_ANGRY;

    return SEQ_TYPE_BYE;
}

/**
 * Séquence de type ANGRY qui affiche yeux fachés et élairs.
 */
SeqType angryWithThunders() {
    Eyes eyes;

    eyes.setExpression(EyeExpressionAngry);
    eyes.show(mx);
    delay(800);
    int normalIntentity = mx.getIntensity();
    showThunders(mx);
    delay(100);
    mx.setIntensity(15);
    delay(100);
    mx.setIntensity(normalIntentity);
    delay(100);
    mx.setIntensity(15);
    delay(100);
    mx.setIntensity(normalIntentity);
    delay(100);
    eyes.show(mx);
    delay(2000);
    if (isFlipSwitchOn()) {
        punchSwitch();
        delay(100);
        if (isFlipSwitchOn())
            return SEQ_TYPE_ANGRY;
        else
            return SEQ_TYPE_BYE;
    } else {
        return SEQ_TYPE_DELIRIUM;
    }
}

void tictac() {
    static bool tic;
    tic = !tic;
    if (tic)
        servo.write(SERVO_HOME + 4);
    else
        servo.write(SERVO_HOME);
}

SeqType deliriumWithBumb() {

    bool slow = isFlipSwitchOn();

    BitFrame<16, 8> textFrame;

    int max = 60 + 60 + 59;

    tictac();
    delay(100);

    for (int i=0; i<=max; i++) {
        int value = max - i;

        show3DigitTime(value, mx);

        if (isFlipSwitchOn() == slow) {
            delay(900);
            tictac();
            delay(100);
        } else {
            delay(30);
            if ((i % 4) == 0)
                tictac();
        }
    }

    showExplosionAnimation(mx);

    if (isFlipSwitchOn())
        punchSwitch();

    delay(500);

    if (isFlipSwitchOn())
        return SEQ_TYPE_ANGRY;

    return SEQ_TYPE_SHUTDOWN;
}

SeqType deliriumShowingFablabAd() {
    showScrollingText("Y-FABLAB.CH", mx);
    delay(100);
    return SEQ_TYPE_SHUTDOWN;
}


/*** sequence list ***/

Seq seqList[] = {
    { SEQ_TYPE_WELCOME, welcome1, 100 },
    { SEQ_TYPE_BYE, bye1, 100 },
    { SEQ_TYPE_DECEPTION, deception1, 100 },
    { SEQ_TYPE_DECEPTION, deception2, 100 },
    { SEQ_TYPE_ASTONISH, astonish1, 100 },
    { SEQ_TYPE_ASTONISH, astonish2, 100 },
    { SEQ_TYPE_ANGRY, angryWithThunders, 100 },
    { SEQ_TYPE_DELIRIUM, deliriumWithBumb, 100 },
    { SEQ_TYPE_DELIRIUM, deliriumShowingFablabAd, 10 },
};

int seqCount = sizeof(seqList) / sizeof(*seqList);
