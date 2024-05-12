/**
 * @file wf_spring_wreath.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "../../app/app.h"
#include "../../system_data_def.h"
#include <lvgl.h>


namespace MOONCAKE {
    namespace BUILTIN_APP {


        namespace WF_SPRING_WREATH {

            struct Anim_t {
                lv_obj_t* _img = nullptr;
                uint8_t _index = 0;
                uint32_t _update_count = 0;
                uint32_t _update_interval = 100;
                bool _playing = false;

                inline bool isPlaying() { return _playing; }
                inline void startPlaying() { _playing = true; }
                inline void stopPlaying() { _playing = false; }
                inline void setIndex(uint8_t index) { _index = index; }
                inline void setInterval(uint32_t interval) { _update_interval = interval; }
            };

            struct Data_t {
                /* Update data in 1Hz */
                uint32_t update_interval = 1000;
                uint32_t update_count = 0;

                lv_obj_t* screen = nullptr;
                lv_obj_t* label_clock = nullptr;
                lv_obj_t* label_infos = nullptr;

                DataTime_t* time_ptr = nullptr;
                uint32_t* step_counter_ptr = nullptr;
                bool* just_wake_up_ptr = nullptr;
                bool* key_home_ptr = nullptr;

                const char* day_of_week[7] = {"Sun", "Mon", "Tues", "Wed", "Thur", "Fri", "Sat"};
                char string_buffer[48] = ""; 
            };

        }


        class WF_Spring_Wreath : public APP_BASE {
            private:
                WF_SPRING_WREATH::Data_t _data;
                WF_SPRING_WREATH::Anim_t _anim;

                static void _lvgl_event_cb(lv_event_t* e);
                void _update_anim();
                void _update_data();


            public:
                WF_Spring_Wreath() = default;
                ~WF_Spring_Wreath() = default;


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
