/**
 * @file boot_anim.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "../../app/app.h"
#include <lvgl.h>


namespace MOONCAKE {
    namespace BUILTIN_APP {


        namespace BOOT_ANIM {

            struct Data_t {
                
                /* Path to the boot anim assets */
                const char* img_bg_static_path = "A:sdcard/boot_anim/background.png";
                const char* img_bg_gif_path = "A:sdcard/boot_anim/background.gif";

                lv_obj_t* screen = nullptr;
                lv_obj_t* img_bg_static = nullptr;
                lv_obj_t* img_bk_gif = nullptr;

                uint32_t anim_time_duration = 2000;
                uint32_t anim_time_count = 0;

            };

        }

    
        class Boot_Anim : public APP_BASE {
            private:
                BOOT_ANIM::Data_t _data;


            public:
                Boot_Anim() = default;
                ~Boot_Anim() = default;


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
