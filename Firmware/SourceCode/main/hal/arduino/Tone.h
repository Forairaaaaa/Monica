/**
 * @file Tone.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
/* Don't want to include Arduino.h */
#pragma once

namespace ARDUINO {

    void tone(uint8_t _pin, unsigned int frequency, unsigned long duration = 0);
    void noTone(uint8_t _pin);

}