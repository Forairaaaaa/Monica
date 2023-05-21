/**
 * @file hardware_manager.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <mooncake/src/app/app.h>
#include <mooncake/src/system_data_def.h>
#include <hal/hal.h>
#include <ctime>


namespace USER_APPS {
    namespace HM {

        struct HardwareManagerData_t {
            /* RTC, update time every 1s */
            int64_t time_update_interval = 1000000;
            int64_t time_update_count = 0;
            MOONCAKE::DataTime_t* time_db_ptr = nullptr;
            tm time_rtc;

            /* Power */
            uint8_t* battery_level_ptr = nullptr;
            bool* battery_is_charging_ptr = nullptr;

            /* Display, check every 30ms */
            int64_t display_update_interval = 30000;
            int64_t display_update_count = 0;
            bool is_disp_on = true;
        };


        class Hardware_Manager : public MOONCAKE::APP_BASE {
                private:
                    HAL* _hal;
                    HardwareManagerData_t _data;


                    void _rtc_update();
                    void _power_info_update();
                    void _disp_update();
                    

                public:
                    Hardware_Manager() : _hal(nullptr) {}
                    ~Hardware_Manager() = default;


                    /**
                     * @brief Lifecycle callbacks for derived to override
                     * 
                     */
                    /* Setup App configs, called when App "install()" */
                    void onSetup();

                    /* Life cycle */
                    void onCreate();
                    void onResume();
                    void onRunning();
                    void onRunningBG();
                    void onPause();
                    void onDestroy();
                
            };

    }

}
