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


    void Hardware_Manager::setDatabase(SIMPLEKV::SimpleKV* database)
    {
        if (database == nullptr) {
            ESP_LOGE(TAG, "empty database");
            return;
        }
        _database = database;


        /* Get data's pointer in database */

        /* Time */
        _rtc_data.ptr_in_db = (MOONCAKE::DataTime_t*)getDatabase()->Get(MC_TIME)->addr;

        /* Power infos */
        _power_infos.ptr_battery_level = (uint8_t*)getDatabase()->Get(MC_BATTERY_LEVEL)->addr;
        _power_infos.ptr_battery_is_charging = (bool*)getDatabase()->Get(MC_BATTERY_IS_CHARGING)->addr;

        /* IMU */
        _imu_data.steps = (uint32_t*)getDatabase()->Get(MC_STEPS)->addr;
    }


    void Hardware_Manager::init()
    {
        HAL::init();
    }

    
    void Hardware_Manager::update()
    {

        update_rtc_time();
        update_imu_data();

        // update_go_sleep();
        // update_power_mode();



        /* HAL update */
        HAL::update();
    }


    void Hardware_Manager::update_rtc_time()
    {
        if ((esp_timer_get_time() - _rtc_data.update_count) > _rtc_data.update_interval) {

            /* Read RTC */
            rtc.getTime(_rtc_data.rtc_time);
            // printf("%02d:%02d:%02d %d-%d-%d-%d\n",
            //     _rtc_data.rtc_time.tm_hour, _rtc_data.rtc_time.tm_min, _rtc_data.rtc_time.tm_sec,
            //     _rtc_data.rtc_time.tm_year, _rtc_data.rtc_time.tm_mon + 1, _rtc_data.rtc_time.tm_mday, _rtc_data.rtc_time.tm_wday);

            /* Write into database */
            _rtc_data.ptr_in_db->hour = _rtc_data.rtc_time.tm_hour;
            _rtc_data.ptr_in_db->min = _rtc_data.rtc_time.tm_min;
            _rtc_data.ptr_in_db->sec = _rtc_data.rtc_time.tm_sec;
            _rtc_data.ptr_in_db->year = _rtc_data.rtc_time.tm_year - 1900;
            _rtc_data.ptr_in_db->mon = _rtc_data.rtc_time.tm_mon;
            _rtc_data.ptr_in_db->mday = _rtc_data.rtc_time.tm_mday;
            _rtc_data.ptr_in_db->wday = _rtc_data.rtc_time.tm_wday;

            /* Update power infos also */
            update_power_infos();

            /* Reset time count */
            _rtc_data.update_count = esp_timer_get_time();
        }
    }


    void Hardware_Manager::update_imu_data()
    {
        if ((esp_timer_get_time() - _imu_data.update_count) > _imu_data.update_interval) {

            /* Read IMU */
            *_imu_data.steps = imu.getSteps();

            /* Reset time count */
            _imu_data.update_count = esp_timer_get_time();
        }
    }


    void Hardware_Manager::update_power_infos()
    {
        *_power_infos.ptr_battery_level = pmu.batteryLevel();
        *_power_infos.ptr_battery_is_charging = pmu.isCharging();
    }


    void Hardware_Manager::update_go_sleep()
    {
        /* Check lvgl inactive time */
        if (lv_disp_get_inactive_time(NULL) > _power_manager.auto_sleep_time) {
            _power_manager.power_mode = mode_sleeping;
        }
        


    }


    void Hardware_Manager::update_power_mode()
    {
        if (_power_manager.power_mode == mode_sleeping) {


            disp.setBrightness(1);





            ESP_LOGI(TAG, "going sleep...");


            gpio_reset_pin(GPIO_NUM_0);
            gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);
            gpio_sleep_set_pull_mode(GPIO_NUM_0, GPIO_PULLUP_ONLY);
            gpio_wakeup_enable(GPIO_NUM_0, GPIO_INTR_LOW_LEVEL);
            
            gpio_reset_pin(GPIO_NUM_12);
            gpio_set_direction(GPIO_NUM_12, GPIO_MODE_INPUT);
            gpio_sleep_set_pull_mode(GPIO_NUM_12, GPIO_PULLUP_ONLY);
            gpio_wakeup_enable(GPIO_NUM_12, GPIO_INTR_LOW_LEVEL);

            gpio_reset_pin(GPIO_NUM_40);
            gpio_set_direction(GPIO_NUM_40, GPIO_MODE_INPUT);
            gpio_sleep_set_pull_mode(GPIO_NUM_40, GPIO_FLOATING);
            gpio_wakeup_enable(GPIO_NUM_40, GPIO_INTR_LOW_LEVEL);


            esp_sleep_enable_gpio_wakeup();

            esp_light_sleep_start();







            





            // buzz.tone(4000, 100);




            /* Wake up */

            _power_manager.power_mode = mode_normal;

            


            /* Reset screen */
            disp.init();
            disp.setColorDepth(16);
            disp.setBrightness(200);


            /* Reset lvgl inactive time */
            lv_disp_trig_activity(NULL);
            /* Refresh full screen */
            lv_obj_invalidate(lv_scr_act());

        }
    }

}
