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
#include "power/hal_power.hpp"
#include "power/hal_power.hpp"
#include "tp/hal_tp.hpp"


#define delay(ms) vTaskDelay(pdMS_TO_TICKS(ms))


class HAL {
    private:
        


    public:
        HAL() = default;
        ~HAL() = default;


        /* Display */
        LGFX_Monica disp;

        /* Touch pad */
        FT3168::TP_FT3168 tp;

        /* PMU AXP2101 */
        AXP2101::AXP2101 pmu;



        void init();
        void update();


};
