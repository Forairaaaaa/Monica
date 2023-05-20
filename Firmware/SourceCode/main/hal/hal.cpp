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
    

    /* Display */
    disp.init();
    disp.setColorDepth(16);
    disp.setBrightness(128);


    /* Touch pad and I2C port 0 */
    auto cfg = tp.config();
    cfg.pull_up_en = false;
    cfg.i2c_port = HAL_PIN_I2C_PORT;
    tp.config(cfg);
    tp.init(HAL_PIN_I2C_SDA, HAL_PIN_I2C_SCL, 3, 12, true, 400000);



}



static FT3168::TouchPoint_t tpp;

void HAL::update()
{


    tp.getTouchRaw(tpp);
    if(tpp.x != -1) {
        printf("%d %d\n", tpp.x, tpp.y);


        disp.drawCircle(tpp.x, tpp.y, 5, TFT_YELLOW);
    }




}

