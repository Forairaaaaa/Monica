/**
 * @file hardware_manager.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "hardware_manager.h"


namespace USER_APPS {
    namespace HM {

        void Hardware_Manager::onSetup()
        {
            setAppName("Hardware Manager");
            setAllowBgRunning(true);

            /* Get hal pointer */
            _hal = (HAL*)getUserData();
        }


        void Hardware_Manager::onCreate()
        {
            /* Get time data pointer from database */
            _data.time_db_ptr = (MOONCAKE::DataTime_t*)getDatabase()->Get(MC_TIME)->addr;

            /* Get power data pointer from database */
            _data.battery_level_ptr = (uint8_t*)getDatabase()->Get(MC_BATTERY_LEVEL)->addr;
            _data.battery_is_charging_ptr = (bool*)getDatabase()->Get(MC_BATTERY_IS_CHARGING)->addr;
        }


        void Hardware_Manager::onResume()
        {
            /* Run on background */
            closeApp();
        }


        void Hardware_Manager::onRunning()
        {
            _disp_update();

            if (!_data.is_disp_on) {
                return;
            }

            /* Data update */
            _rtc_update();
            _power_info_update();
        }


        void Hardware_Manager::onRunningBG()
        {
            onRunning();
        }


        void Hardware_Manager::onPause()
        {

        }


        void Hardware_Manager::onDestroy()
        {

        }

    }
}
