/**
 * @file infos_manager.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "hardware_manager.h"
#include <esp_log.h>
#include <esp_timer.h>


namespace USER_APPS {
    namespace HM {

        void Hardware_Manager::_rtc_update()
        {
            if ((esp_timer_get_time() - _data.time_update_count) > _data.time_update_interval) {
                
                /* Read RTC */
                _hal->rtc.getTime(_data.time_rtc);
                // printf("%02d:%02d:%02d %d-%d-%d-%d\n",
                //     _data.time_rtc.tm_hour, _data.time_rtc.tm_min, _data.time_rtc.tm_sec,
                //     _data.time_rtc.tm_year, _data.time_rtc.tm_mon + 1, _data.time_rtc.tm_mday, _data.time_rtc.tm_wday);

                /* Write into database */
                _data.time_db_ptr->hour = _data.time_rtc.tm_hour;
                _data.time_db_ptr->min = _data.time_rtc.tm_min;
                _data.time_db_ptr->sec = _data.time_rtc.tm_sec;
                _data.time_db_ptr->year = _data.time_rtc.tm_year - 1900;
                _data.time_db_ptr->mon = _data.time_rtc.tm_mon;
                _data.time_db_ptr->mday = _data.time_rtc.tm_mday;
                _data.time_db_ptr->wday = _data.time_rtc.tm_wday;

                _data.time_update_count = esp_timer_get_time();
            }
        }


        void Hardware_Manager::_power_info_update()
        {
            *_data.battery_level_ptr = _hal->pmu.batteryLevel();
            *_data.battery_is_charging_ptr = _hal->pmu.isCharging();
        }

    }
}
