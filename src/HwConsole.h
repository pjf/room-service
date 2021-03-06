#ifndef HWCONSOLE_H
#define HWCONSOLE_H

#ifndef UNIT_TEST
#ifndef NATIVE

#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_TLC5947.h>
#include <string>

#include "types.h"

class Game;

class HwConsole {
    private:
        const byte tlcQty = 5;

        const byte pwrLed   = 13;
        const byte tlcLatch = 14;
        const byte tlcOE    = 15;   // Output enable
        const byte tlcClock = 16;
        const byte tlcData  = 17;

        const byte lcdAddr  = 0x27;
        const byte lcdAddr2 = 0x26;
        const byte lcdBacklightPin = 3;

        std::string playerInput;

        Keypad InitKeypad();
        void updateLeds(Game *game);
        void updateLCDs(Game *game);

        void displayLcdCode(const char *, byte target = 0);

        // This is a rather awful way of doing this; we should
        // have code that supports N displays.
        static const byte numDisplays = 2;
        LiquidCrystal_I2C lcd;
        LiquidCrystal_I2C lcd2;

    public:
        Keypad keypad;
        Adafruit_TLC5947 tlc;
        LiquidCrystal_I2C *lcds[numDisplays];

        HwConsole()
            : playerInput("")
            , lcd(LiquidCrystal_I2C(lcdAddr,  2,1,0,4,5,6,7))
            , lcd2(LiquidCrystal_I2C(lcdAddr2,2,1,0,4,5,6,7))
            , keypad(InitKeypad())
            , tlc(Adafruit_TLC5947(tlcQty, tlcClock, tlcData, tlcLatch))
        {
            Serial.begin(9600);
            tlc.begin();

            // Ugh, kludgy.
            lcds[0] = &lcd;
            lcds[1] = &lcd2;

            // Set up power LED and tlc output enable.
            pinMode(pwrLed, OUTPUT);
            pinMode(tlcOE, OUTPUT);

            // Show we're powered on.
            powerLed(HIGH);

            // Disable light output, clear them, and re-enable.
            // Disabling output first ensures all the controllers have
            // enough juice in case of dodgy power.
            disableLights();
            clearLights();
            enableLights();

            // Enable our 16x2 displays
            
            for (int i=0; i < numDisplays; ++i) {
                lcds[i]->begin(16,2);
                lcds[i]->setBacklightPin(lcdBacklightPin,POSITIVE);
                lcds[i]->setBacklight(HIGH);
                lcds[i]->home(); // Cursor -> 0,0
                lcds[i]->print("Stay A While");
                lcds[i]->setCursor(0,1);
                lcds[i]->print("Stay... FOREVER");
            }
        }

        void enableLights();
        void disableLights();
        void resetLights();
        void clearLights();
        void powerLed(uint8_t);
        void updateConsole(Game *);
        void updateKeypad(game_state_t, std::string *);

        void clearLcds();
        void displayLcd(const std::string line1, const std::string line2, byte target = 0);
        void displayLcd(const char *line1, const char *line2 = NULL, byte target = 0);
        void displayLcd(strpair_t, byte target = 0);
        void clearLcdLine(byte line, byte target = 0);
};

#endif  // NATIVE
#endif  // UNIT_TEST

#endif
