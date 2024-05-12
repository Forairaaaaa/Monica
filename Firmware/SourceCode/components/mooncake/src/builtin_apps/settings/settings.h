/**
 * @file settings.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-06-02
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


        namespace SETTINGS {

            struct Data_t {

                lv_obj_t* screen = nullptr;
                lv_obj_t* roller_hour = nullptr;
                lv_obj_t* roller_min = nullptr;
                lv_obj_t* roller_month = nullptr;
                lv_obj_t* roller_mday = nullptr;
                lv_obj_t* button_set = nullptr;

                bool* key_home_ptr = nullptr;
                
            };

        }


        class Settings : public APP_BASE {
            private:
                SETTINGS::Data_t _data;

                static void _lvgl_event_cb(lv_event_t* e);
                static void _button_set_event_cb(lv_event_t* e);
                
                void _set_time();


            public:
                Settings() = default;
                ~Settings() = default;


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



