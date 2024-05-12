/**
 * @file settings.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-06-02
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "settings.h"
#include "../assets/assets.h"


namespace MOONCAKE {
    namespace BUILTIN_APP {


        void Settings::_lvgl_event_cb(lv_event_t* e)
        {
            /* Get event code */
            lv_event_code_t code = lv_event_get_code(e);

            /* Quit */
            #ifndef ESP_PLATFORM
            if (code == LV_EVENT_GESTURE) {
                // printf("gesture\n");

                Settings* app = (Settings*)lv_event_get_user_data(e);

                if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_BOTTOM) {
                    app->destroyApp();
                }
            }
            #endif
            
        }


        void Settings::_button_set_event_cb(lv_event_t* e)
        {
            /* Get event code */
            lv_event_code_t code = lv_event_get_code(e);

            if (code == LV_EVENT_CLICKED) {
                
                Settings* app = (Settings*)lv_event_get_user_data(e);
                app->_set_time();
                
            }
        }


        void Settings::_set_time()
        {
            /* Cpoy current time from database */
            DataTime_t time_buffer = getDatabase()->Get(MC_TIME)->value<DataTime_t>();

            /* Get Hour */
            time_buffer.hour = lv_roller_get_selected(_data.roller_hour);

            /* Get Min */
            time_buffer.min = lv_roller_get_selected(_data.roller_min);

            /* Get Month */
            time_buffer.mon = lv_roller_get_selected(_data.roller_month);

            /* Get Mday */
            time_buffer.mday = lv_roller_get_selected(_data.roller_mday) + 1;

            /* Get Year(o.O) */
            time_buffer.year = 2023 - 1900;

            /* Get Wday(o.O) */
            time_buffer.wday = 6;

            /* Reset Sec */
            time_buffer.sec = 0;


            printf("set time to: %d:%d:%d %d,%d/%d/%d\n", 
                    time_buffer.hour,
                    time_buffer.min,
                    time_buffer.sec,
                    time_buffer.wday,
                    time_buffer.year,
                    time_buffer.mon,
                    time_buffer.mday);

            
            /* Push into database */
            getDatabase()->Put<DataTime_t>(MC_TIME, time_buffer);

            /* Set flag */
            getDatabase()->Put<bool>(MC_TIME_JSUT_SET, true);

        }


        void Settings::onSetup()
        {
            setAppName("Settings");
            setAllowBgRunning(false);
            setAppIcon((void*)&ui_img_app_icon_hdpi_settings_png);
        }


        /* Life cycle */
        void Settings::onCreate()
        {
            printf("[%s] onCreate\n", getAppName().c_str());


            /* Reset data to default */
            {
                SETTINGS::Data_t data;
                _data = data;
            }


            /* Get data's pointer from database */
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
            /* Set user data */
            lv_obj_set_user_data(_data.screen, this);


            /* Create setting widgets */
            lv_obj_t* label_hour = lv_label_create(_data.screen);
            lv_obj_set_x(label_hour, -95);
            lv_obj_set_y(label_hour, -114);
            lv_obj_set_align(label_hour, LV_ALIGN_CENTER);
            lv_label_set_text(label_hour, "- Hour -");
            lv_obj_set_style_text_font(label_hour, &ui_font_OpenSansMediumItalic24, LV_PART_MAIN | LV_STATE_DEFAULT);


            lv_obj_t* label_min = lv_label_create(_data.screen);
            lv_obj_set_x(label_min, 95);
            lv_obj_set_y(label_min, -114);
            lv_obj_set_align(label_min, LV_ALIGN_CENTER);
            lv_label_set_text(label_min, "- Min -");
            lv_obj_set_style_text_font(label_min, &ui_font_OpenSansMediumItalic24, LV_PART_MAIN | LV_STATE_DEFAULT);


            lv_obj_t* label_banner = lv_label_create(_data.screen);
            lv_obj_set_x(label_banner, 0);
            lv_obj_set_y(label_banner, -170);
            lv_obj_set_align(label_banner, LV_ALIGN_CENTER);
            lv_label_set_text(label_banner, "Time :)");
            lv_obj_set_style_text_color(label_banner, lv_color_hex(0x6269B4), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(label_banner, &ui_font_OpenSansMedium96, LV_PART_MAIN | LV_STATE_DEFAULT);


            lv_obj_t* label_month = lv_label_create(_data.screen);
            lv_obj_set_x(label_month, -90);
            lv_obj_set_y(label_month, 32);
            lv_obj_set_align(label_month, LV_ALIGN_CENTER);
            lv_label_set_text(label_month, "- Month -");
            lv_obj_set_style_text_font(label_month, &ui_font_OpenSansMediumItalic24, LV_PART_MAIN | LV_STATE_DEFAULT);


            lv_obj_t* label_mday = lv_label_create(_data.screen);
            lv_obj_set_x(label_mday, 90);
            lv_obj_set_y(label_mday, 32);
            lv_obj_set_align(label_mday, LV_ALIGN_CENTER);
            lv_label_set_text(label_mday, "- Day -");
            lv_obj_set_style_text_font(label_mday, &ui_font_OpenSansMediumItalic24, LV_PART_MAIN | LV_STATE_DEFAULT);



            _data.roller_hour = lv_roller_create(_data.screen);
            lv_roller_set_options(_data.roller_hour,
                                "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23", LV_ROLLER_MODE_NORMAL);
            lv_obj_set_width(_data.roller_hour, lv_pct(45));
            lv_obj_set_height(_data.roller_hour, lv_pct(25));
            lv_obj_set_x(_data.roller_hour, -95);
            lv_obj_set_y(_data.roller_hour, -45);
            lv_obj_set_align(_data.roller_hour, LV_ALIGN_CENTER);
            lv_obj_set_style_text_color(_data.roller_hour, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(_data.roller_hour, &ui_font_OpenSansMediumItalic24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(_data.roller_hour, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(_data.roller_hour, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(_data.roller_hour, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

            lv_obj_set_style_text_font(_data.roller_hour, &ui_font_OpenSansMediumItalic24, LV_PART_SELECTED | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(_data.roller_hour, 15, LV_PART_SELECTED | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(_data.roller_hour, lv_color_hex(0xD45252), LV_PART_SELECTED | LV_STATE_DEFAULT);


            _data.roller_min = lv_roller_create(_data.screen);
            lv_roller_set_options(_data.roller_min,
                                "0\n1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59",
                                LV_ROLLER_MODE_NORMAL);
            lv_obj_set_width(_data.roller_min, lv_pct(45));
            lv_obj_set_height(_data.roller_min, lv_pct(25));
            lv_obj_set_x(_data.roller_min, 95);
            lv_obj_set_y(_data.roller_min, -45);
            lv_obj_set_align(_data.roller_min, LV_ALIGN_CENTER);
            lv_obj_set_style_text_color(_data.roller_min, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(_data.roller_min, &ui_font_OpenSansMediumItalic24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(_data.roller_min, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(_data.roller_min, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(_data.roller_min, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

            lv_obj_set_style_text_font(_data.roller_min, &ui_font_OpenSansMediumItalic24, LV_PART_SELECTED | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(_data.roller_min, 15, LV_PART_SELECTED | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(_data.roller_min, lv_color_hex(0xD45252), LV_PART_SELECTED | LV_STATE_DEFAULT);


            _data.roller_month = lv_roller_create(_data.screen);
            lv_roller_set_options(_data.roller_month, "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12", LV_ROLLER_MODE_NORMAL);
            lv_obj_set_width(_data.roller_month, lv_pct(45));
            lv_obj_set_height(_data.roller_month, lv_pct(25));
            lv_obj_set_x(_data.roller_month, -95);
            lv_obj_set_y(_data.roller_month, 100);
            lv_obj_set_align(_data.roller_month, LV_ALIGN_CENTER);
            lv_obj_set_style_text_color(_data.roller_month, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(_data.roller_month, &ui_font_OpenSansMediumItalic24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(_data.roller_month, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(_data.roller_month, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(_data.roller_month, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

            lv_obj_set_style_text_font(_data.roller_month, &ui_font_OpenSansMediumItalic24, LV_PART_SELECTED | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(_data.roller_month, 15, LV_PART_SELECTED | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(_data.roller_month, lv_color_hex(0xD45252), LV_PART_SELECTED | LV_STATE_DEFAULT);


            _data.roller_mday = lv_roller_create(_data.screen);
            lv_roller_set_options(_data.roller_mday,
                                "1\n2\n3\n4\n5\n6\n7\n8\n9\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31",
                                LV_ROLLER_MODE_NORMAL);
            lv_obj_set_width(_data.roller_mday, lv_pct(45));
            lv_obj_set_height(_data.roller_mday, lv_pct(25));
            lv_obj_set_x(_data.roller_mday, 95);
            lv_obj_set_y(_data.roller_mday, 103);
            lv_obj_set_align(_data.roller_mday, LV_ALIGN_CENTER);
            lv_obj_set_style_text_color(_data.roller_mday, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(_data.roller_mday, &ui_font_OpenSansMediumItalic24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(_data.roller_mday, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(_data.roller_mday, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(_data.roller_mday, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

            lv_obj_set_style_text_font(_data.roller_mday, &ui_font_OpenSansMediumItalic24, LV_PART_SELECTED | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(_data.roller_mday, 15, LV_PART_SELECTED | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(_data.roller_mday, lv_color_hex(0xD45252), LV_PART_SELECTED | LV_STATE_DEFAULT);


            _data.button_set = lv_btn_create(_data.screen);
            lv_obj_set_width(_data.button_set, 218);
            lv_obj_set_height(_data.button_set, 40);
            lv_obj_set_x(_data.button_set, 0);
            lv_obj_set_y(_data.button_set, 190);
            lv_obj_set_align(_data.button_set, LV_ALIGN_CENTER);
            lv_obj_add_flag(_data.button_set, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
            lv_obj_clear_flag(_data.button_set, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
            lv_obj_set_style_radius(_data.button_set, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(_data.button_set, lv_color_hex(0x6669B2), LV_PART_MAIN | LV_STATE_DEFAULT);

            lv_obj_t* label_button = lv_label_create(_data.button_set);
            lv_obj_set_align(label_button, LV_ALIGN_CENTER);
            lv_label_set_text(label_button, "Set!");
            lv_obj_set_style_text_font(label_button, &ui_font_OpenSansMediumItalic24, LV_PART_MAIN | LV_STATE_DEFAULT);

            /* Add event callback */
            lv_obj_add_event_cb(_data.button_set, _button_set_event_cb, LV_EVENT_CLICKED, (void*)this);
        }


        void Settings::onResume()
        {
            printf("[%s] onResume\n", getAppName().c_str());
        }


        void Settings::onRunning()
        {
            /* If pressed key Home */
            if (*_data.key_home_ptr) {
                /* Reset */
                *_data.key_home_ptr = false;

                /* Quit */
                destroyApp();
            }
        }


        void Settings::onRunningBG()
        {

        }


        void Settings::onPause()
        {
            printf("[%s] onPause\n", getAppName().c_str());
        }


        void Settings::onDestroy()
        {
            printf("[%s] onDestroy\n", getAppName().c_str());
        }


    }
}
