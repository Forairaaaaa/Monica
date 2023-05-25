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

    /* Buzzer */
    buzz.init(HAL_PIN_BUZZER);

    /* Lvgl */
    lvgl.init(&disp, &tp);

    /* IMU BMI270 + BMM150 */
    imu.init();
    /* Setup wrist wear wakeup interrupt */
    imu.setWristWearWakeup();
    /* Enable step counter */
    imu.enableStepCounter();



    // float x, y, z;

    // while (1) {

    //     if (imu.accelerationAvailable()) {
    //         imu.readAcceleration(x, y, z); 

    //         printf("%0.4f \t %0.4f \t %0.4f\n", x, y, z);
            
    //     }


    //     printf("%ld\n", imu.getSteps());
    

    //     vTaskDelay(10);
    // }



}


void HAL::update()
{
    lvgl.update();
}

