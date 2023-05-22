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
#include <esp_log.h>


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
    }


    void Hardware_Manager::init()
    {
        HAL::init();
    }

    
    void Hardware_Manager::update()
    {

        update_rtc_time();



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


    void Hardware_Manager::update_power_infos()
    {
        *_power_infos.ptr_battery_level = pmu.batteryLevel();
        *_power_infos.ptr_battery_is_charging = pmu.isCharging();
    }

}
