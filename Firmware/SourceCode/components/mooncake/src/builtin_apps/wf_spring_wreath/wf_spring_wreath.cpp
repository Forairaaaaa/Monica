/**
 * @file wf_spring_wreath.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "wf_spring_wreath.h"
#include "../assets/assets.h"


/* Works like shit on esp32 */
#define RANDOM_ROTATION 0


namespace MOONCAKE {
    namespace BUILTIN_APP {


        void WF_Spring_Wreath::_lvgl_event_cb(lv_event_t* e)
        {
            /* Get event code */
            lv_event_code_t code = lv_event_get_code(e);

            /* Play anim */
            if (code == LV_EVENT_CLICKED) {
                // printf("clicked\n");

                WF_Spring_Wreath* app = (WF_Spring_Wreath*)lv_event_get_user_data(e);

                #if RANDOM_ROTATION
                /* Random angle */
                if (!app->_anim.isPlaying()) {
                    lv_img_set_angle(app->_anim._img, (int16_t)std::rand());
                }
                #endif
                app->_anim.startPlaying();
            }

            /* Quit */
            #ifndef ESP_PLATFORM
            else if (code == LV_EVENT_GESTURE) {
                // printf("gesture\n");

                WF_Spring_Wreath* app = (WF_Spring_Wreath*)lv_event_get_user_data(e);

                if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_BOTTOM) {
                    app->destroyApp();
                }
            }
            #endif
        }


        void WF_Spring_Wreath::_update_anim()
        {
            /* Update anim */
            if (_anim.isPlaying() && ((lv_tick_get() - _anim._update_count) > _anim._update_interval)) {
                
                lv_img_set_src(_anim._img, anim_spring_wreath[_anim._index]);

                /* Next frame */
                _anim._index++;
                if (_anim._index > (NUM_ANIM_SPRING_WREATH - 1)) {
                    _anim._index = 0;
                    _anim.stopPlaying();
                }
                _anim._update_count = lv_tick_get();
            }
            
            /* If just wake up */
            if (*_data.just_wake_up_ptr) {
                /* Reset */
                *_data.just_wake_up_ptr = false;

                /* Update data at once */
                _update_data();

                #if RANDOM_ROTATION
                /* Random angle */
                lv_img_set_angle(_anim._img, (int16_t)std::rand());
                #endif

                /* Play from start */
                _anim.setIndex(0);
                _anim.startPlaying();
                /* Update at once */
                lv_img_set_src(_anim._img, anim_spring_wreath[0]);
            }

            /* If pressed key Home */
            if (*_data.key_home_ptr) {
                /* Reset */
                *_data.key_home_ptr = false;

                /* Quit */
                destroyApp();
            }
        }


        void WF_Spring_Wreath::_update_data()
        {
            /* Clock */
            snprintf(_data.string_buffer, sizeof(_data.string_buffer), "%02d:%02d", _data.time_ptr->hour, _data.time_ptr->min);
            lv_label_set_text(_data.label_clock, _data.string_buffer);

            /* Infos */
            snprintf(_data.string_buffer, sizeof(_data.string_buffer), "~~~~~~~~~~~~~~~~\n%d/%d %s.  %ld steps",
                    _data.time_ptr->mon + 1,
                    _data.time_ptr->mday,
                    _data.day_of_week[_data.time_ptr->wday],
                    *_data.step_counter_ptr);
            lv_label_set_text(_data.label_infos, _data.string_buffer);

            // printf("666\n");
        }


        void WF_Spring_Wreath::onSetup()
        {
            setAppName("Spring Wreath");
            setAllowBgRunning(false);
            setAppIcon((void*)&ui_img_app_icon_hdpi_wf_spring_wreath_png);
        }


        /* Life cycle */
        void WF_Spring_Wreath::onCreate()
        {
            printf("[%s] onCreate\n", getAppName().c_str());


            /* Get data's pointer from database */
            _data.time_ptr = (DataTime_t*)getDatabase()->Get(MC_TIME)->addr;
            _data.step_counter_ptr = (uint32_t*)getDatabase()->Get(MC_STEPS)->addr;
            _data.just_wake_up_ptr = (bool*)getDatabase()->Get(MC_JUST_WAKEUP)->addr;
            _data.key_home_ptr = (bool*)getDatabase()->Get(MC_KEY_HOME)->addr;
            /* Reset at first */
            *_data.key_home_ptr = false;


            /* Create screen */
            _data.screen = lv_obj_create(NULL);
            lv_scr_load_anim(_data.screen, LV_SCR_LOAD_ANIM_FADE_IN, 50, 0, false);
            /* Set background color */
            lv_obj_set_style_bg_color(_data.screen, lv_color_hex(0x000000), LV_STATE_DEFAULT);
            /* Add event callback */
            lv_obj_add_event_cb(_data.screen, _lvgl_event_cb, LV_EVENT_ALL, (void*)this);


            /* Init anim */
            _anim._img = lv_img_create(_data.screen);
            lv_obj_align(_anim._img, LV_ALIGN_CENTER, 0, 0);
            _anim.setInterval(37);
            _anim.setIndex(0);
            _anim.startPlaying();


            /* Clock */
            _data.label_clock = lv_label_create(_data.screen);
            lv_obj_set_x(_data.label_clock, 0);
            lv_obj_set_y(_data.label_clock, -32);
            lv_obj_set_align(_data.label_clock, LV_ALIGN_CENTER);
            lv_label_set_text(_data.label_clock, "23:32");
            lv_obj_set_style_text_color(_data.label_clock, lv_color_hex(0xEFF2A5), LV_PART_MAIN);
            lv_obj_set_style_text_font(_data.label_clock, &ui_font_OpenSansMedium96, LV_PART_MAIN);


            /* infos */
            _data.label_infos = lv_label_create(_data.screen);
            lv_obj_set_x(_data.label_infos, 0);
            lv_obj_set_y(_data.label_infos, 26);
            lv_obj_set_align(_data.label_infos, LV_ALIGN_CENTER);
            lv_label_set_text(_data.label_infos, "~~~~~~~~~~~~~~~~\n3/7 Mon.  2333 steps");
            lv_obj_set_style_text_color(_data.label_infos, lv_color_hex(0xEFF2A5), LV_PART_MAIN);
            lv_obj_set_style_text_font(_data.label_infos, &ui_font_OpenSansMediumItalic24, LV_PART_MAIN);
        }


        void WF_Spring_Wreath::onResume()
        {
            printf("[%s] onResume\n", getAppName().c_str());

        }


        void WF_Spring_Wreath::onRunning()
        {
            /* Anim */
            _update_anim();

            /* Data */
            if ((lv_tick_get() - _data.update_count) > _data.update_interval) {
                _update_data();
                _data.update_count = lv_tick_get();
            }
        }


        void WF_Spring_Wreath::onRunningBG()
        {

        }


        void WF_Spring_Wreath::onPause()
        {
            printf("[%s] onPause\n", getAppName().c_str());
        }


        void WF_Spring_Wreath::onDestroy()
        {
            printf("[%s] onDestroy\n", getAppName().c_str());
        }



    }
}
