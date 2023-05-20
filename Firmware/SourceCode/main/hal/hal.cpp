/**
 * @file hal.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "hal.h"


void HAL::init()
{
    

    /* Amoled */
    disp.init();
    disp.setColorDepth(16);
    // disp.setBrightness(255);

}


void HAL::update()
{


    disp.fillScreen(TFT_RED);
    printf("r\n");
    delay(2000);

    disp.fillScreen(TFT_GREEN);
    printf("g\n");
    delay(2000);
    
    disp.fillScreen(TFT_BLUE);
    printf("b\n");
    delay(2000);




}

