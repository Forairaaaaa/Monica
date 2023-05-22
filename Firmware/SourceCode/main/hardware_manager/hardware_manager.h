/**
 * @file hardware_manager.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "../hal/hal.h"
#include "mooncake/src/simplekv/simplekv.h"
#include "mooncake/src/system_data_def.h"


namespace HM {


    struct RtcData_t {
        /* update time every 1s */
        int64_t update_interval = 1000000;
        int64_t update_count = 0;
        MOONCAKE::DataTime_t* ptr_in_db = nullptr;
        tm rtc_time;
    };


    struct PowerInfos_t {
        uint8_t* ptr_battery_level = nullptr;
        bool* ptr_battery_is_charging = nullptr;
    };


    class Hardware_Manager : public HAL {
        private:
            SIMPLEKV::SimpleKV* _database;
            RtcData_t _rtc_data;
            PowerInfos_t _power_infos;


        public:
            Hardware_Manager() : _database(nullptr) {}
            ~Hardware_Manager() = default;


            /**
             * @brief Set database, set before running update
             * 
             * @param database which mooncake is using 
             */
            void setDatabase(SIMPLEKV::SimpleKV* database);
            inline SIMPLEKV::SimpleKV* getDatabase() { return _database; }

            void init();
            void update();




            void update_rtc_time();
            void update_power_infos();
            void update_go_sleep();



    };

}