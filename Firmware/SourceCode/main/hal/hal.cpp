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
#include "hal_config.h"


tm rtc_time;


void HAL::init()
{
    

    /* Display */
    disp.init();
    disp.setColorDepth(16);
    disp.setBrightness(200);


    /* Touch pad and I2C port 0 (default) */
    auto cfg = tp.config();
    cfg.pull_up_en = false;
    tp.config(cfg);
    tp.init(HAL_PIN_I2C_SDA, HAL_PIN_I2C_SCL, HAL_PIN_TP_RST, HAL_PIN_TP_INTR, true, 400000);

    /* PMU AXP2101 */
    pmu.init(HAL_PIN_I2C_SDA, HAL_PIN_I2C_SCL, HAL_PIN_AXP_INTR);


    /* RTC PCF8563 */
    rtc.init(HAL_PIN_I2C_SDA, HAL_PIN_I2C_SCL, HAL_PIN_RTC_INTR);


    // rtc_time.tm_hour = 2;
    // rtc_time.tm_min = 6;
    // rtc_time.tm_sec = 0;
    // rtc_time.tm_year = 2023;
    // rtc_time.tm_mon = 4;
    // rtc_time.tm_mday = 21;
    // rtc_time.tm_wday = 0;
    // rtc.setTime(rtc_time);

}



static FT3168::TouchPoint_t tpp;



bool screenOn = true;

void HAL::update()
{

    if (pmu.isKeyPressed()) {
        printf("666 %lld\n", esp_timer_get_time());

        disp.fillScreen(TFT_BLACK);

        screenOn = !screenOn;
    }

    if (!screenOn) {
        return;
    }


    rtc.getTime(rtc_time);
    printf("%02d:%02d:%02d %d-%d-%d-%d\n", rtc_time.tm_hour, rtc_time.tm_min, rtc_time.tm_sec, rtc_time.tm_year, rtc_time.tm_mon + 1, rtc_time.tm_mday, rtc_time.tm_wday);
    
    disp.setTextSize(2);
    disp.setTextColor(TFT_YELLOW, TFT_BLACK);
    disp.setCursor(10, 50);
    disp.printf("%02d:%02d:%02d %d-%d-%d-%d  \n", rtc_time.tm_hour, rtc_time.tm_min, rtc_time.tm_sec, rtc_time.tm_year, rtc_time.tm_mon + 1, rtc_time.tm_mday, rtc_time.tm_wday);



    tp.getTouchRaw(tpp);
    if(tpp.x != -1) {
        // printf("%d %d\n", tpp.x, tpp.y);


        disp.fillSmoothCircle(tpp.x, tpp.y, 10, TFT_YELLOW);
    }


    

    if (pmu.isKeyLongPressed()) {
        printf("777 %lld\n", esp_timer_get_time());
        disp.fillScreen(TFT_BLUE);
    }


    // delay(10);


    if (pmu.isCharging()) {
        disp.setTextSize(2);
        disp.setTextColor(TFT_YELLOW, TFT_BLACK);
        disp.setCursor(10, 100);
        disp.printf("is charging            \n");
    }
    else {
        disp.setTextSize(2);
        disp.setTextColor(TFT_YELLOW, TFT_BLACK);
        disp.setCursor(10, 100);
        disp.printf("not charging           \n");
    }


    if (pmu.isChargeDone()) {
        disp.setTextSize(2);
        disp.setTextColor(TFT_YELLOW, TFT_BLACK);
        disp.setCursor(10, 150);
        disp.printf("charge done              \n");
    }
    else {
        disp.setTextSize(2);
        disp.setTextColor(TFT_YELLOW, TFT_BLACK);
        disp.setCursor(10, 150);
        disp.printf("charge not done          \n");
    }



    disp.setTextSize(2);
    disp.setTextColor(TFT_YELLOW, TFT_BLACK);
    disp.setCursor(10, 200);
    disp.printf("bat: %d%%    \n", pmu.batteryLevel());




    delay(1000);
}

