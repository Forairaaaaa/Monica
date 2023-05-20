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


void HAL::init()
{
    

    /* Display */
    disp.init();
    disp.setColorDepth(16);
    disp.setBrightness(255);


    /* Touch pad and I2C port 0 */
    auto cfg = tp.config();
    cfg.pull_up_en = false;
    cfg.i2c_port = HAL_PIN_I2C_PORT;
    tp.config(cfg);
    tp.init(HAL_PIN_I2C_SDA, HAL_PIN_I2C_SCL, HAL_PIN_TP_RST, HAL_PIN_TP_INTR, true, 400000);


    /* PMU AXP2101 */
    pmu.init(HAL_PIN_I2C_SDA, HAL_PIN_I2C_SCL, HAL_PIN_AXP_INTR);

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
        disp.setCursor(10, 50);
        disp.printf("is charging            \n");
    }
    else {
        disp.setTextSize(2);
        disp.setTextColor(TFT_YELLOW, TFT_BLACK);
        disp.setCursor(10, 50);
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



}

