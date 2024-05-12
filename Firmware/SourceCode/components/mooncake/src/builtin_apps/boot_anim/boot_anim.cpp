/**
 * @file boot_anim.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "boot_anim.h"
#include <lvgl.h>


namespace MOONCAKE {
    namespace BUILTIN_APP {


        void Boot_Anim::onSetup()
        {
            setAppName("BootAnim");
            setAllowBgRunning(false);
        }


        /* Life cycle */
        void Boot_Anim::onCreate()
        {
            printf("[%s] onCreate\n", getAppName().c_str());


            /* Create screen */
            _data.screen = lv_obj_create(NULL);
            lv_scr_load_anim(_data.screen, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
            /* Set background color */
            lv_obj_set_style_bg_color(_data.screen, lv_color_hex(0x000000), LV_STATE_DEFAULT);
            /* Lock scroll */
            lv_obj_set_scroll_dir(_data.screen, LV_DIR_NONE);


            /* Static bg */
            _data.img_bg_static = lv_img_create(_data.screen);
            lv_img_set_src(_data.img_bg_static, _data.img_bg_static_path);
            lv_obj_align(_data.img_bg_static, LV_ALIGN_CENTER, 0, 0);

            /* GIF bg */
            _data.img_bk_gif = lv_gif_create(_data.screen);
            lv_gif_set_src(_data.img_bk_gif, _data.img_bg_gif_path);
            lv_obj_align(_data.img_bk_gif, LV_ALIGN_CENTER, 0, 0);
            

        }


        void Boot_Anim::onResume()
        {
            _data.anim_time_count = lv_tick_get();
        }


        void Boot_Anim::onRunning()
        {
            /* Update lvgl timer */
            lv_timer_handler();

            /* Quit */
            if ((lv_tick_get() - _data.anim_time_count) > _data.anim_time_duration) {
                destroyApp();
            }
        }


        void Boot_Anim::onRunningBG()
        {

        }


        void Boot_Anim::onPause()
        {

        }


        void Boot_Anim::onDestroy()
        {
            printf("[%s] onDestroy\n", getAppName().c_str());
        }


    }
}
