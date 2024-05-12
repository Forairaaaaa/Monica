/**
 * @file sketchpad.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-29
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


        namespace SKETCHPAD {

            struct Data_t {

                lv_obj_t* screen = nullptr;

                lv_obj_t* canvas = nullptr;
                lv_color_t* canvas_buffer = nullptr;
                lv_point_t touchPoint = {-1, -1};
                lv_color_t pen_color = lv_palette_main(LV_PALETTE_GREEN);
                lv_coord_t pen_size = 12;

                bool* key_home_ptr = nullptr;
                
            };

        }


        class Sketchpad : public APP_BASE {
            private:
                SKETCHPAD::Data_t _data;

                /* Color picker, copy from demo widget */
                static void color_changer_create(lv_obj_t * parent);
                static void color_changer_anim_cb(void * var, int32_t v);
                static void color_changer_event_cb(lv_event_t * e);
                static void color_event_cb(lv_event_t * e);

                static void _lvgl_event_cb(lv_event_t* e);
                void _update_drawing();


            public:
                Sketchpad() = default;
                ~Sketchpad() = default;


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

