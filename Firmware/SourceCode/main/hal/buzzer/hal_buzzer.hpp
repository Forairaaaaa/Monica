/**
 * @file hal_buzzer.hpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <stdint.h>
#include "../arduino/Tone.h"


namespace BUZZER {

    class BUZZER {
        private:
            uint8_t _pin;

        public:
            BUZZER() : _pin(45) {}

            inline void init(int8_t pin) { _pin = pin; }

            /* Wrap */
            inline void tone(unsigned int frequency, unsigned long duration = 0) { ARDUINO::tone(_pin, frequency, duration); }
            inline void noTone() { ARDUINO::noTone(_pin); }

    };

}
