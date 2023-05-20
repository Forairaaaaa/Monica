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



#define delay(ms) vTaskDelay(pdMS_TO_TICKS(ms))


class HAL {
    private:
        


    public:
        HAL() = default;
        ~HAL() = default;


        LGFX_Monica disp;


        void init();
        void update();


};
