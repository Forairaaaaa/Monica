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


    /* Buttons */
    btnA.begin();
    btnB.begin();


    /* Once button and power setup, check boot mode */
    checkBootMode();


    /* RTC PCF8563 */
    rtc.init(HAL_PIN_I2C_SDA, HAL_PIN_I2C_SCL, HAL_PIN_RTC_INTR);


    /* Buzzer */
    buzz.init(HAL_PIN_BUZZER);


    /* SD card */
    sd.init();


    /* Lvgl */
    lvgl.init(&disp, &tp);


    /* IMU BMI270 + BMM150 */
    imu.init();
    /* Setup wrist wear wakeup interrupt */
    imu.setWristWearWakeup();
    /* Enable step counter */
    imu.enableStepCounter();


}


void HAL::update()
{
    lvgl.update();
}


void HAL::checkBootMode()
{
    /* Press button B while power on to enter USB MSC mode */
    if (!btnB.read()) {
        vTaskDelay(pdMS_TO_TICKS(20));
        if (!btnB.read()) {

            disp.fillScreen(TFT_BLACK);
            disp.setTextSize(3);
            disp.setCursor(0, 50);
            disp.printf(" :)\n Release Key\n To Enter\n USB MSC Mode\n");

            while (!btnB.read()) {
                vTaskDelay(pdMS_TO_TICKS(10));
            }

            disp.fillScreen(TFT_BLACK);
            disp.setTextSize(4);
            disp.setCursor(0, 50);
            disp.printf(" USB MSC Mode\n\n\n\n\n\n\n\n\n\n Reboot     ->");

            hal_enter_usb_msc_mode();

        }
    }

}