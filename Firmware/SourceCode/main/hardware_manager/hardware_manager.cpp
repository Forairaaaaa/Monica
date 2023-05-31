/**
 * @file hardware_manager.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "hardware_manager.h"
#include "../hal/hal_config.h"
#include <esp_log.h>
#include <esp_sleep.h>


#define delay(ms) vTaskDelay(pdMS_TO_TICKS(ms))


namespace HM {


    static const char* TAG = "HM";


    void Hardware_Manager::_update_rtc_time()
    {
        /* Read RTC */
        rtc.getTime(_rtc_data.rtc_time);
        // printf("%02d:%02d:%02d %d-%d-%d-%d\n",
        //     _rtc_data.rtc_time.tm_hour, _rtc_data.rtc_time.tm_min, _rtc_data.rtc_time.tm_sec,
        //     _rtc_data.rtc_time.tm_year, _rtc_data.rtc_time.tm_mon + 1, _rtc_data.rtc_time.tm_mday, _rtc_data.rtc_time.tm_wday);

        /* Write into database */
        _rtc_data.time_ptr->hour = _rtc_data.rtc_time.tm_hour;
        _rtc_data.time_ptr->min = _rtc_data.rtc_time.tm_min;
        _rtc_data.time_ptr->sec = _rtc_data.rtc_time.tm_sec;
        _rtc_data.time_ptr->year = _rtc_data.rtc_time.tm_year - 1900;
        _rtc_data.time_ptr->mon = _rtc_data.rtc_time.tm_mon;
        _rtc_data.time_ptr->mday = _rtc_data.rtc_time.tm_mday;
        _rtc_data.time_ptr->wday = _rtc_data.rtc_time.tm_wday;

        /* Update power infos also */
        _update_power_infos();
    }


    void Hardware_Manager::_update_imu_data()
    {
        /* Read IMU */
        *_imu_data.steps = imu.getSteps();
    }


    void Hardware_Manager::_update_power_infos()
    {
        *_power_infos.battery_level_ptr = pmu.batteryLevel();
        *_power_infos.battery_is_charging_ptr = pmu.isCharging();
    }


    void Hardware_Manager::_update_go_sleep()
    {
        /* Check lvgl inactive time */
        if (lv_disp_get_inactive_time(NULL) > _power_manager.auto_sleep_time) {
            _power_manager.power_mode = mode_sleeping;
        }
    
    }


    void Hardware_Manager::_update_power_mode()
    {
        if (_power_manager.power_mode == mode_sleeping) {
            
            ESP_LOGI(TAG, "going sleep...");

            /* Close display */
            disp.setBrightness(1);
            
            /* Setup wakeup pins */
            /* Key Up */
            gpio_reset_pin(GPIO_NUM_0);
            gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);
            gpio_sleep_set_pull_mode(GPIO_NUM_0, GPIO_PULLUP_ONLY);
            gpio_wakeup_enable(GPIO_NUM_0, GPIO_INTR_LOW_LEVEL);
            /* Touch pad */
            gpio_reset_pin(GPIO_NUM_12);
            gpio_set_direction(GPIO_NUM_12, GPIO_MODE_INPUT);
            gpio_sleep_set_pull_mode(GPIO_NUM_12, GPIO_PULLUP_ONLY);
            gpio_wakeup_enable(GPIO_NUM_12, GPIO_INTR_LOW_LEVEL);
            /* IMU */
            gpio_reset_pin(GPIO_NUM_40);
            gpio_set_direction(GPIO_NUM_40, GPIO_MODE_INPUT);
            gpio_sleep_set_pull_mode(GPIO_NUM_40, GPIO_FLOATING);
            gpio_wakeup_enable(GPIO_NUM_40, GPIO_INTR_LOW_LEVEL);

            esp_sleep_enable_gpio_wakeup();

            /* Go to sleep :) */
            esp_light_sleep_start();

            /* ---------------------------------------------------------------- */

            /* Wake up o.O */
            _power_manager.power_mode = mode_normal;

            /* Update data at once */
            _update_rtc_time();
            _update_imu_data();

            /* Tell Mooncake */
            *_system_data.just_wake_up_ptr = true;

            /* Restart display */
            disp.init();
            disp.setBrightness(1);
            disp.setColorDepth(16);

            /* Reset lvgl inactive time */
            lv_disp_trig_activity(NULL);

            /* Update a little bit before display on */
            getMooncake()->update();
            // lv_timer_handler();
            // delay(LV_DISP_DEF_REFR_PERIOD + 1);
            // lv_timer_handler();

            /* Refresh full screen */
            lv_obj_invalidate(lv_scr_act());

            /* Display on */
            disp.setBrightness(200);
        }
    }


    void Hardware_Manager::_update_key_data()
    {
        /* Key Home or Power */
        if (pmu.isKeyPressed()) {
            *_key_data.key_home_ptr = true;
            *_key_data.key_pwr_ptr = true;
        }

        /* Key Up */
        if (btnA.pressed()) {
            *_key_data.key_up_ptr = true;
        }

        /* Key Down */
        if (btnB.pressed()) {
            *_key_data.key_down_ptr = true;
        }
    }


    void Hardware_Manager::setMooncake(MOONCAKE::Mooncake* mooncake)
    {
        if (mooncake == nullptr) {
            ESP_LOGE(TAG, "empty pointer");
            return;
        }
        _mooncake = mooncake;


        /* Get data's pointer in database */

        /* Time */
        _rtc_data.time_ptr = (MOONCAKE::DataTime_t*)getDatabase()->Get(MC_TIME)->addr;

        /* Power infos */
        _power_infos.battery_level_ptr = (uint8_t*)getDatabase()->Get(MC_BATTERY_LEVEL)->addr;
        _power_infos.battery_is_charging_ptr = (bool*)getDatabase()->Get(MC_BATTERY_IS_CHARGING)->addr;

        /* IMU */
        _imu_data.steps = (uint32_t*)getDatabase()->Get(MC_STEPS)->addr;

        /* System data */
        _system_data.just_wake_up_ptr = (bool*)getDatabase()->Get(MC_JUST_WAKEUP)->addr;

        /* Keys */
        _key_data.key_home_ptr = (bool*)getDatabase()->Get(MC_KEY_HOME)->addr;
        _key_data.key_pwr_ptr = (bool*)getDatabase()->Get(MC_KEY_POWER)->addr;
        _key_data.key_up_ptr = (bool*)getDatabase()->Get(MC_KEY_UP)->addr;
        _key_data.key_down_ptr = (bool*)getDatabase()->Get(MC_KEY_DOWN)->addr;
    }


    void Hardware_Manager::init()
    {
        HAL::init();
    }

    
    void Hardware_Manager::update()
    {
        /* Update RTC */
        if ((esp_timer_get_time() - _rtc_data.update_count) > _rtc_data.update_interval) {
            _update_rtc_time();
            _rtc_data.update_count = esp_timer_get_time();
        }
        
        /* Update IMU */
        if ((esp_timer_get_time() - _imu_data.update_count) > _imu_data.update_interval) {
            _update_imu_data();
            _imu_data.update_count = esp_timer_get_time();
        }

        /* Update keys */
        if ((esp_timer_get_time() - _key_data.update_count) > _key_data.update_interval) {
            _update_key_data();
            _key_data.update_count = esp_timer_get_time();
        }


        /* Update power control */
        _update_go_sleep();
        _update_power_mode();

        /* HAL update */
        HAL::update();
    }


    

}
