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
#include "button/Button.h"
#include "buzzer/hal_buzzer.hpp"
#include "disp/hal_disp.hpp"
#include "lvgl/hal_lvgl.hpp"
#include "power/hal_power.hpp"
#include "rtc/hal_rtc.hpp"
#include "sd_card/hal_sd_crad.hpp"
#include "tp/hal_tp.hpp"
#include <BMI270.h>


class HAL {
    private:
        bool _isSleeping ;


    public:
        HAL() : _isSleeping(false) {}
        ~HAL() = default;

        
        /* Sleep flag */
        inline void isSleeping(bool sleep) { _isSleeping = sleep; }
        inline bool isSleeping(void) { return _isSleeping; } 


        /* Display */
        LGFX_Monica disp;

        /* Touch pad */
        FT3168::TP_FT3168 tp;

        /* PMU AXP2101 */
        AXP2101::AXP2101 pmu;

        /* RTC PCF8563 */
        PCF8563::PCF8563 rtc;

        /* SD card */
        SD_CARD::SD_Card sd;

        /* Buttons */
        Button btnA = Button(GPIO_NUM_0);
        Button btnB = Button(GPIO_NUM_39);

        /* Buzzer */
        BUZZER::BUZZER buzz;

        /* IMU BMM270 + BMM150 */
        BMI270::BMI270 imu;

        /* Lvgl */
        LVGL::LVGL lvgl;


        /**
         * @brief Hal init 
         * 
         */
        void init();

        /**
         * @brief Update hal
         * 
         */
        void update();


};
