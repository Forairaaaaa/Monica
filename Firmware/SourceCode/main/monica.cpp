/**
 * @file monica.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "hal/hal.h"

static HAL Hal;


extern "C" void app_main(void)
{
    Hal.init();


    while (1) {
        Hal.update();
    }

}
