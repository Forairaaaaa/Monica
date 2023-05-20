/**
 * @file hal.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "disp/hal_disp.hpp"
#include "tp/hal_tp.hpp"


#define delay(ms) vTaskDelay(pdMS_TO_TICKS(ms))


#define HAL_PIN_I2C_PORT    0
#define HAL_PIN_I2C_SCL     10
#define HAL_PIN_I2C_SDA     11


class HAL {
    private:
        


    public:
        HAL() = default;
        ~HAL() = default;


        /* Display */
        LGFX_Monica disp;

        /* Touch pad */
        FT3168::tp_ft3168 tp;




        void init();
        void update();


};
