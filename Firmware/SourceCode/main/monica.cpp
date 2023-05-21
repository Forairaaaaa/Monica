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
#include <mooncake.h>
#include "hal/hal.h"


static MOONCAKE::Framework mooncake_ui;
static HAL hal;


extern "C" void app_main(void)
{
    /* Hardware init */
    hal.init();


    /* UI framwork init */
    mooncake_ui.setDisplay(hal.disp.width(), hal.disp.height());
    mooncake_ui.init();


    while (1) {
        mooncake_ui.update();
        hal.update();
        vTaskDelay(pdMS_TO_TICKS(2));
    }

}
