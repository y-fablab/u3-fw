
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
    SEQ_TYPE_WELCOME,
    SEQ_TYPE_ANGRY,
    SEQ_TYPE_DELIRIUM,
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

    eyes.setDirection(0, -1);
    eyes.show(mx);

    mp3.playFileByIndexNumber(5);

    delay(500);

    eyes.showBlinkAnimation(mx);

    delay(1000);

    if (!isFlipSwitchOn()) {
        // the user has switched off - no fun
        goto end;
    }

    punchSwitch();

    eyes.setDirection(0, 0);
    eyes.show(mx);

    if (isFlipSwitchOn()) {
        /*
         * I tried to switch off but it doesn't work :-(
         * Is the user preventing me to switch off?
         * Is a mechanical problem?
         */
        return SEQ_TYPE_ANGRY;
    }

    delay(800);

    eyes.showBlinkAnimation(mx, false, true);

    delay(1000);

end:

    for (int i=6; i>=0; i--) {
        eyes.setOpening(i);
        eyes.show(mx);
        delay(50);
    }

    delay(1000);

    return SEQ_TYPE_SHUTDOWN;
}

void tictac() {
    static bool tic;
    tic = !tic;
    if (tic)
        servo.write(SERVO_HOME + 4);
    else
        servo.write(SERVO_HOME);
}

SeqType bumb() {

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

SeqType fablabAd() {
    showScrollingText("Y-FABLAB.CH", mx);
    delay(100);
    return SEQ_TYPE_SHUTDOWN;
}

SeqType grrr() {
    showScrollingText("GRRRR...", mx);
    delay(100);
    if (isFlipSwitchOn()) {
        punchSwitch();
        delay(100);
        if (isFlipSwitchOn())
            return SEQ_TYPE_ANGRY;
    } else {
        return SEQ_TYPE_DELIRIUM;
    }
}

void test9() {
  //pinMode(GPIO_SHUTDOWN, OUTPUT);
  // digitalWrite(GPIO_SHUTDOWN, LOW);

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

  mx.show(screen);

  for (int i = 0; i <= 1000; i++) {
    float t = 0.001f * i;
    float pos = SERVO_HOME + (SERVO_OPEN_10MM - SERVO_HOME) * t;
    servo.write(pos);
    delay(5);
  }

  delay(1000);

  for (int i = 0; i <= 1000; i++) {
    float t = 0.001f * (1000 - i);
    float pos = SERVO_HOME + (SERVO_OPEN_10MM - SERVO_HOME) * t;
    servo.write(pos);
    delay(5);
  }

  delay(1000);

  if (isFlipSwitchOn()) {
    punchSwitch();
  } else {
    servo.write(SERVO_HOME);
    delay(500);
  }

  delay(500);
  shutDown();
}


/*** sequence list ***/

Seq seqList[] = {
    { SEQ_TYPE_WELCOME, welcome1, 100 },
    { SEQ_TYPE_ANGRY, grrr, 100 },
    { SEQ_TYPE_DELIRIUM, bumb, 100 },
    { SEQ_TYPE_DELIRIUM, fablabAd, 10 },
};

int seqCount = sizeof(seqList) / sizeof(*seqList);
