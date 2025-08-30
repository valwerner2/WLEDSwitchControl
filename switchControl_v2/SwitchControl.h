//
// Created by valentinw on 10.07.25.
//

#include "wled.h"

#pragma once

class SwitchControl : public Usermod {
private:
    static const int switchCount = 4;
    const int switchPins[switchCount] = {12, 25 , 27, 26};
    std::vector<std::vector<int>> switchSeg = {
            {0, 2},   // switch 0
            {1},      // switch 1
            {3, 5},   // switch 2
            {4}       // switch 3
    };
    bool lastSwitchState[switchCount] = {LOW, LOW, LOW, LOW};
    bool switchState[switchCount] = {LOW, LOW, LOW, LOW};


public:
    SwitchControl() = default;
    ~SwitchControl() override = default;
    void setup() override {
        if (!Serial) {
            Serial.begin(115200);
            delay(100);
        }
        Serial.print("SETTING UP MOD\n");
        for (int i = 0; i < switchCount; i++) {
            Serial.print("SETTING BUTTON ");
            Serial.print(i);
            Serial.print(" \n");
            pinMode(switchPins[i], INPUT_PULLDOWN); // HIGH ACTIVE
        }
    }
    void loop() override{
        static unsigned long lastButtonCheck =  millis();

        bool anyOn = false;
        if( millis() - lastButtonCheck >= 500)
        {
            lastButtonCheck =  millis();
            for (int i = 0; i < switchCount; i++) {
                switchState[i] = digitalRead(switchPins[i]);

                for(int currentSegIndex = 0; currentSegIndex < switchSeg[i].size(); currentSegIndex++)
                {
                    //does segment exists?
                    if (switchSeg[i][currentSegIndex] < strip.getSegmentsNum())
                    {
                        //Serial.print(String(i) + "(" + String(switchState[i]) + ")" + " -> " + String(switchSeg[i][currentSegIndex]) + "\n");
                        // set segment power
                        Segment &seg = strip.getSegment(switchSeg[i][currentSegIndex]);
                        seg.setOption(SEG_OPTION_ON, switchState[i] == HIGH);
                        if(switchState[i] == HIGH){anyOn = true;}
                    }
                }

                lastSwitchState[i] = switchState[i];
            }
            bool isOn = bri > 0;
            if(anyOn != isOn)
            {
                toggleOnOff();
            }
            // Update LED
            strip.show();
            /*
            String printSwitchState = "[";
            for(int i = 0; i < switchCount; i++)
            {
                printSwitchState += String(switchState[i]) + " ";
            }
            printSwitchState += "]\n\n\n";

            Serial.print(printSwitchState);
             */
        }
    }
};

