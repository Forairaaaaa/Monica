/**
 * @file wf_user_custom.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "../../app/app.h"
#include "../../system_data_def.h"
#include <lvgl.h>
#include <iostream>
#include <string>
#include <vector>


namespace MOONCAKE {
    namespace BUILTIN_APP {


        namespace WF_USER_CUSTOM {

            /*
            * [File structure]:

                watch_faces/
                ├── watch_faces.json                [Watch-faces description]
                ├── wf_a
                │   ├── background                  [Back fround assets]
                │   ├── number                      [Clock number assets]
                │   └── face.json                   [Custom-watch-face description]
                └── wf_b
                    ├── background
                    ├── number
                    └── face.json


            * [Watch-faces description]: contains the waatch faces's path

                {
                    "watch_faces_path" : [
                        "wf_a",
                        "wf_b"
                    ]
                }


            * [Custom-watch-face description]

                {
                    "name" : "Watch face A",
                    "use_gif_background" : "yes",
                    "pos_clock_hour_a" : [-120, 0],
                    "pos_clock_hour_b" : [-60, 0],
                    "pos_clock_colon" : [0, 0],
                    "pos_clock_min_a" : [60, 0],
                    "pos_clock_min_b" : [120, 0]
                }

            */

           
            struct Pos_t {
                int x = 0;
                int y = 0;
            };

            struct CustomData_t {
                const char* wf_folder_path = "A:sdcard/watch_faces";
                const char* wf_custom_description_path = "face.json";
                const char* wf_custom_bg_asset_path = "background/";
                const char* wf_custom_num_asset_path = "number/";
                const char* wf_custom_static_asset_suffix = ".png";
                const char* wf_custom_gif_asset_suffix = ".gif";
                const char* wf_custom_bg_asset_name = "background";


                std::string wf_current_using_path = "null";
                std::string wf_name;
                bool using_gif_bg = false;

                Pos_t pos_clock_hour_a;
                Pos_t pos_clock_hour_b;
                Pos_t pos_clock_colon;
                Pos_t pos_clock_min_a;
                Pos_t pos_clock_min_b;
            };


            struct Data_t {
                /* Update data in 1Hz */
                uint32_t update_interval = 1000;
                uint32_t update_count = 0;
                bool running = false;

                lv_obj_t* screen = nullptr;
                lv_obj_t* panel_wf_select = nullptr;
                lv_obj_t* img_bg_static = nullptr;
                lv_obj_t* img_bk_gif = nullptr;
                lv_obj_t* img_clock_hour_a = nullptr;
                lv_obj_t* img_clock_hour_b = nullptr;
                lv_obj_t* img_clock_colon = nullptr;
                lv_obj_t* img_clock_min_a = nullptr;
                lv_obj_t* img_clock_min_b = nullptr;

                DataTime_t* time_ptr = nullptr;
                bool* just_wake_up_ptr = nullptr;
                bool* key_home_ptr = nullptr;
            };

        }


        class WF_User_Custom : public APP_BASE {
            private:
                WF_USER_CUSTOM::CustomData_t _custom_data;
                WF_USER_CUSTOM::Data_t _data;

                static void _lvgl_event_cb(lv_event_t* e);
                static void _roller_event_cb(lv_event_t* e);
                static void _button_event_cb(lv_event_t* e);
                void _update_data();

                bool _choose_watch_face();
                bool _create_watch_face();
                bool _get_face_description();



            public:
                WF_User_Custom() = default;
                ~WF_User_Custom() = default;


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
