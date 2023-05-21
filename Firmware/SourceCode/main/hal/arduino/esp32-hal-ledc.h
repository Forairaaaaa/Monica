// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef _ESP32_HAL_LEDC_H_
#define _ESP32_HAL_LEDC_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    NOTE_C, NOTE_Cs, NOTE_D, NOTE_Eb, NOTE_E, NOTE_F, NOTE_Fs, NOTE_G, NOTE_Gs, NOTE_A, NOTE_Bb, NOTE_B, NOTE_MAX
} note_t;

typedef struct {
    uint8_t pin;                    // Pin assigned to channel
    uint8_t channel;                // Channel number
    uint8_t channel_resolution;     // Resolution of channel
} *ledc_channel_handle_t;

//channel 0-15 resolution 1-16bits freq limits depend on resolution
bool        ledcAttach(uint8_t pin, uint32_t freq, uint8_t resolution);
bool        ledcWrite(uint8_t pin, uint32_t duty);
uint32_t    ledcWriteTone(uint8_t pin, uint32_t freq);
uint32_t    ledcWriteNote(uint8_t pin, note_t note, uint8_t octave);
uint32_t    ledcRead(uint8_t pin);
uint32_t    ledcReadFreq(uint8_t pin);
bool        ledcDetach(uint8_t pin);
uint32_t    ledcChangeFrequency(uint8_t pin, uint32_t freq, uint8_t resolution);


#ifdef __cplusplus
}
#endif

#endif /* _ESP32_HAL_LEDC_H_ */
