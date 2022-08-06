#pragma once

#include "MyDefines.hpp"

#define ANGLE_PER_CHANGE 6
#define MAX_COUNT 60

void interuptKnob();

class MyKnob {
    private:
        bool up_;
        bool left_;
        bool down_;
        bool right_;
        bool push_;
        bool input_push_;
        bool previous_input_push_;
        bool changed_;
        bool encoderA_;
        bool encoderA_previous;
        bool encoderB_;
        bool encoderB_previous;
        int16_t encoderPossition_;
        int16_t encoderPossition_previous;

    public:
        MyKnob() :
            up_(false), left_(false), down_(false), right_(false), push_(false),
            input_push_(false), previous_input_push_(false), changed_(false),
            encoderA_(false), encoderA_previous(false), encoderB_(false), encoderB_previous(false),
            encoderPossition_(0)
        {
            setupIO();
            // setInterupts();
        }

        ~MyKnob() = default;

        void setupIO() {

            pinMode(UP_A_PIN, INPUT_PULLUP);
            pinMode(LEFT_B_PIN, INPUT_PULLUP);
            pinMode(DOWN_C_PIN, INPUT_PULLUP);
            pinMode(RIGHT_D_PIN, INPUT_PULLUP);
            pinMode(PUSH_P_PIN, INPUT_PULLUP);

            pinMode(ENCODER_A, INPUT_PULLUP);
            pinMode(ENCODER_B, INPUT_PULLUP);

            
        }

        // void setInterupts() {
        //     // attachInterrupt(digitalPinToInterrupt(PUSH_P_PIN), interuptKnob, CHANGE);
        // }

        bool checkDigitalInput(uint8_t pin) {
            uint8_t count = 0;
            for(auto i = 0; i < 20; ++i) {
                count += digitalRead(pin);
            }
            // Serial.print("pin = ");
            // Serial.print(pin);
            // Serial.print(" count = ");
            // Serial.println(count);

            return (count > 15? true: false);
        }

        void updateKnobPossition() {
            
            input_push_ = !checkDigitalInput(PUSH_P_PIN);
            up_ = !checkDigitalInput(UP_A_PIN);
            left_ = !checkDigitalInput(LEFT_B_PIN);
            down_ = !checkDigitalInput(DOWN_C_PIN);
            right_ = !checkDigitalInput(RIGHT_D_PIN);
            
            if (input_push_)
                push_ = !(up_ || left_ || down_ || right_);
            else {
                push_ = false;
            }

        }

        void printDirection() {

            Serial.print("pins = ");
            Serial.print(up_);
            Serial.print(" ");
            Serial.print(left_);
            Serial.print(" ");
            Serial.print(down_);
            Serial.print(" ");
            Serial.print(right_);
            Serial.print(" ");
            Serial.print(push_);
            Serial.print(" - ");
            Serial.print(input_push_);
            Serial.println();

            changed_ = false;

        }
        
        bool reqPrint() { return changed_; }
        void setReqPrint() {
            if(input_push_ != previous_input_push_) {
                previous_input_push_ = input_push_;
                changed_ = true; 
            }

        }
            
        bool encoderStatus() {
            encoderA_ = checkDigitalInput(ENCODER_A);
            encoderB_ = checkDigitalInput(ENCODER_B);

            // encoderA_ = digitalRead(ENCODER_A);
            // encoderB_ = digitalRead(ENCODER_B);
            
            if (encoderA_ != encoderA_previous) {
                if (encoderA_ == true) {
                    if (encoderB_ == false)
                        ++encoderPossition_;
                    else
                        --encoderPossition_;
                }
                else {
                    if (encoderB_ == false)
                        --encoderPossition_;
                    else
                        ++encoderPossition_;
                }
                encoderA_previous = encoderA_;
            }

            if (encoderB_ != encoderB_previous) {
                if (encoderB_ == true) {
                    if (encoderA_ == false)
                        --encoderPossition_;
                    else
                        ++encoderPossition_;
                }
                else {
                    if (encoderA_ == false)
                        ++encoderPossition_;
                    else
                        --encoderPossition_;
                }
                encoderB_previous = encoderB_;
            }

            if (encoderPossition_ > MAX_COUNT)
                encoderPossition_ = 0;
            if (encoderPossition_ < 0)
                encoderPossition_ = MAX_COUNT;

            if (encoderPossition_ != encoderPossition_previous) {
                encoderPossition_previous = encoderPossition_;
                return true;
            }
            return false;
        }

        void printEncoder() {
            Serial.println(encoderPossition_ * ANGLE_PER_CHANGE);
        }
   
};

MyKnob myKnob = MyKnob();

// void interuptKnob() {
//     myKnob.updateKnobPossition();
//     myKnob.setReqPrint();
// }