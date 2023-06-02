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
#include <mooncake.h>


namespace HM {


    struct RtcData_t {
        /* update time in 1Hz */
        int64_t update_interval = 1000000;
        int64_t update_count = 0;
        MOONCAKE::DataTime_t* time_ptr = nullptr;
        bool* time_just_set_ptr = nullptr;
        tm rtc_time;
    };


    struct PowerInfos_t {
        uint8_t* battery_level_ptr = nullptr;
        bool* battery_is_charging_ptr = nullptr;
    };


    struct ImuData_t {
        /* update IMU data in 50Hz */
        int64_t update_interval = 20;
        int64_t update_count = 0;
        uint32_t* steps = nullptr;
    };


    enum PowerMode_t {
        mode_normal = 0,
        mode_going_sleep,
        mode_sleeping
    };


    struct SystemData_t {
        bool* just_wake_up_ptr = nullptr;
    };


    struct PowerManager_t {
        PowerMode_t power_mode = mode_normal;

        uint32_t auto_sleep_time = 8000;
    };


    struct KeyData_t {
        /* update key in 100Hz */
        int64_t update_interval = 10000;
        int64_t update_count = 0;

        bool* key_home_ptr = nullptr;
        bool* key_pwr_ptr = nullptr;
        bool* key_up_ptr = nullptr;
        bool* key_down_ptr = nullptr;
    };


    class Hardware_Manager : public HAL {
        private:
            MOONCAKE::Mooncake* _mooncake;

            RtcData_t _rtc_data;
            PowerInfos_t _power_infos;
            PowerManager_t _power_manager;
            ImuData_t _imu_data;
            SystemData_t _system_data;
            KeyData_t _key_data;


            void _update_rtc_time();
            void _update_imu_data();
            void _update_power_infos();

            void _update_go_sleep();
            void _update_power_mode();

            void _update_key_data();


        public:
            Hardware_Manager() : _mooncake(nullptr) {}
            ~Hardware_Manager() = default;


            /**
             * @brief Set Mooncake that hardware manager works for, set after Mooncake's init
             * 
             * @param mooncake 
             */
            void setMooncake(MOONCAKE::Mooncake* mooncake);
            inline MOONCAKE::Mooncake* getMooncake(void) { return _mooncake; }
            inline SIMPLEKV::SimpleKV* getDatabase() { return _mooncake->getDatabase(); }


            /**
             * @brief Init hardware manager
             * 
             */
            void init();


            /**
             * @brief Call this in loop to keep hardware manager running
             * 
             */
            void update();

    };

}