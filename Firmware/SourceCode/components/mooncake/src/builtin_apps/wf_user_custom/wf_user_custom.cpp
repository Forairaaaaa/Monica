/**
 * @file wf_user_custom.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "wf_user_custom.h"
#include "../assets/assets.h"
#include <ArduinoJson.hpp>

/* If not include this, lv_fs acts strange in my wsl linux env */
#include <stdio.h>
#include <stdlib.h>



namespace MOONCAKE {
    namespace BUILTIN_APP {


        void WF_User_Custom::_lvgl_event_cb(lv_event_t* e)
        {
            /* Get event code */
            lv_event_code_t code = lv_event_get_code(e);

            /* Quit */
            #ifndef ESP_PLATFORM
            if (code == LV_EVENT_GESTURE) {
                // printf("gesture\n");

                WF_User_Custom* app = (WF_User_Custom*)lv_event_get_user_data(e);

                if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_BOTTOM) {
                    app->destroyApp();
                }
            }
            #endif

        }


        void WF_User_Custom::_button_event_cb(lv_event_t* e)
        {
            /* Get event code */
            lv_event_code_t code = lv_event_get_code(e);

            if (code == LV_EVENT_CLICKED) {

                lv_obj_t* roller = (lv_obj_t*)lv_obj_get_user_data(lv_event_get_target(e));
                
                char string_buffer[256];
                lv_roller_get_selected_str(roller, string_buffer, 256);

                printf("select %s\n", string_buffer);
                WF_User_Custom* app = (WF_User_Custom*)lv_event_get_user_data(e);
                app->_custom_data.wf_current_using_path = string_buffer;

            }
        }


        void WF_User_Custom::_update_data()
        {
            // printf("666\n");

            std::string string_buffer;
            std::string base_path;

            /* Base path */
            base_path = _custom_data.wf_folder_path;
            base_path += "/";
            base_path += _custom_data.wf_current_using_path;
            base_path += "/";
            base_path += _custom_data.wf_custom_num_asset_path;


            string_buffer = base_path;
            string_buffer += std::to_string((_data.time_ptr->min % 10));
            string_buffer += _custom_data.wf_custom_static_asset_suffix;
            // printf("%s\n", string_buffer.c_str());
            lv_img_set_src(_data.img_clock_min_b, string_buffer.c_str());

            string_buffer = base_path;
            string_buffer += std::to_string((_data.time_ptr->min / 10));
            string_buffer += _custom_data.wf_custom_static_asset_suffix;
            // printf("%s\n", string_buffer.c_str());
            lv_img_set_src(_data.img_clock_min_a, string_buffer.c_str());

            string_buffer = base_path;
            string_buffer += "colon";
            string_buffer += _custom_data.wf_custom_static_asset_suffix;
            // printf("%s\n", string_buffer.c_str());
            lv_img_set_src(_data.img_clock_colon, string_buffer.c_str());

            string_buffer = base_path;
            string_buffer += std::to_string((_data.time_ptr->hour % 10));
            string_buffer += _custom_data.wf_custom_static_asset_suffix;
            // printf("%s\n", string_buffer.c_str());
            lv_img_set_src(_data.img_clock_hour_b, string_buffer.c_str());

            string_buffer = base_path;
            string_buffer += std::to_string((_data.time_ptr->hour / 10));
            string_buffer += _custom_data.wf_custom_static_asset_suffix;
            // printf("%s\n", string_buffer.c_str());
            lv_img_set_src(_data.img_clock_hour_a, string_buffer.c_str());

        }


        bool WF_User_Custom::_choose_watch_face()
        {
            printf("[%s] choose face\n", getAppName().c_str());

            /* Open watch faces folder */
            lv_fs_dir_t dir;
            lv_fs_res_t res;
            res = lv_fs_dir_open(&dir, _custom_data.wf_folder_path);
            if (res != LV_FS_RES_OK) {
                printf("[%s] open %s failed %d\n", getAppName().c_str(), _custom_data.wf_folder_path, res);
                return false;
            }

            /* Read watch faces folders */
            char string_buffer[256];
            std::string wf_custom_path_list = "";
            while (1) {
                /* Open dir */
                res = lv_fs_dir_read(&dir, string_buffer);
                if (res != LV_FS_RES_OK) {
                    printf("[%s] read dir error %d\n", getAppName().c_str(), res);
                    return false;
                }
                if (strlen(string_buffer) == 0) {
                    break;
                }

                /* Add into list without '/' */
                wf_custom_path_list += (string_buffer + 1);
                wf_custom_path_list += "\n";
            }
            /* Remove extra \n */
            wf_custom_path_list = wf_custom_path_list.substr(0, wf_custom_path_list.size() - 1);
            // printf("%s\n", wf_custom_path_list.c_str());

            /* Close folder */
            lv_fs_dir_close(&dir);



            /* Create a panel to select watch face */
            _data.panel_wf_select = lv_obj_create(_data.screen);
            lv_obj_set_width(_data.panel_wf_select, lv_pct(100));
            lv_obj_set_height(_data.panel_wf_select, lv_pct(100));
            lv_obj_set_align(_data.panel_wf_select, LV_ALIGN_CENTER);
            lv_obj_set_style_radius(_data.panel_wf_select, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(_data.panel_wf_select, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(_data.panel_wf_select, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_clear_flag(_data.panel_wf_select, LV_OBJ_FLAG_SCROLLABLE);


            /* Create a label */
            lv_obj_t* ui_Label = lv_label_create(_data.panel_wf_select);
            lv_obj_set_x(ui_Label, 0);
            lv_obj_set_y(ui_Label, -121);
            lv_obj_set_align(ui_Label, LV_ALIGN_CENTER);
            lv_label_set_text(ui_Label, "Watch\nFace :)");
            lv_obj_set_style_text_color(ui_Label, lv_color_hex(0x6269B4), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_opa(ui_Label, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(ui_Label, &ui_font_OpenSansMedium96, LV_PART_MAIN | LV_STATE_DEFAULT);


            /* Create a roller */
            lv_obj_t* ui_Roller = lv_roller_create(_data.panel_wf_select);
            lv_roller_set_options(ui_Roller, wf_custom_path_list.c_str(), LV_ROLLER_MODE_NORMAL);
            lv_obj_set_width(ui_Roller, lv_pct(95));
            lv_obj_set_height(ui_Roller, lv_pct(40));
            lv_obj_set_x(ui_Roller, 0);
            lv_obj_set_y(ui_Roller, 62);
            lv_obj_set_align(ui_Roller, LV_ALIGN_CENTER);
            lv_obj_set_style_text_color(ui_Roller, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(ui_Roller, &ui_font_OpenSansMediumItalic24, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(ui_Roller, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_Roller, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(ui_Roller, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

            lv_obj_set_style_text_font(ui_Roller, &ui_font_OpenSansMediumItalic24, LV_PART_SELECTED | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(ui_Roller, 15, LV_PART_SELECTED | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_Roller, lv_color_hex(0xD45252), LV_PART_SELECTED | LV_STATE_DEFAULT);


            /* Create a button */
            lv_obj_t* ui_Button = lv_btn_create(_data.panel_wf_select);
            lv_obj_set_width(ui_Button, 218);
            lv_obj_set_height(ui_Button, 40);
            lv_obj_set_x(ui_Button, 0);
            lv_obj_set_y(ui_Button, 190);
            lv_obj_set_align(ui_Button, LV_ALIGN_CENTER);
            lv_obj_add_flag(ui_Button, LV_OBJ_FLAG_SCROLL_ON_FOCUS);     /// Flags
            lv_obj_clear_flag(ui_Button, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
            lv_obj_set_style_radius(ui_Button, 15, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(ui_Button, lv_color_hex(0x6669B2), LV_PART_MAIN | LV_STATE_DEFAULT);

            lv_obj_t* ui_Label_button = lv_label_create(ui_Button);
            lv_obj_set_align(ui_Label_button, LV_ALIGN_CENTER);
            lv_label_set_text(ui_Label_button, "Go!");
            lv_obj_set_style_text_font(ui_Label_button, &ui_font_OpenSansMediumItalic24, LV_PART_MAIN | LV_STATE_DEFAULT);

            lv_obj_add_event_cb(ui_Button, _button_event_cb, LV_EVENT_CLICKED, (void*)this);
            lv_obj_set_user_data(ui_Button, (void*)ui_Roller);

            
            return true;
        }


        bool WF_User_Custom::_get_face_description()
        {
            /* Get description */
            std::string string_buffer;
            string_buffer = _custom_data.wf_folder_path;
            string_buffer += "/";
            string_buffer += _custom_data.wf_current_using_path;
            string_buffer += "/";
            string_buffer += _custom_data.wf_custom_description_path;

            /* Read description */
            lv_fs_file_t desc;
            lv_fs_res_t res = lv_fs_open(&desc, string_buffer.c_str(), LV_FS_MODE_RD);
            if(res != LV_FS_RES_OK) {
                printf("[%s] open %s error: %d\n", getAppName().c_str(), string_buffer.c_str(), res);
                return false;
            }

            string_buffer = "";
            char read_buffer = 0;
            uint32_t read_num = 0;
            while (1) {
                res = lv_fs_read(&desc, (void*)&read_buffer, sizeof(read_buffer), &read_num);
                if(res != LV_FS_RES_OK) {
                    printf("[%s] read error: %d\n", getAppName().c_str(), res);
                    return false;
                }

                /* If finish reading */
                if (read_num == 0) {
                    break;
                }

                // printf("%c", read_buffer);
                string_buffer += read_buffer;
            }
            // printf("\n%s\n\n", string_buffer.c_str());

            /* Close description file */
            lv_fs_close(&desc);
            

            /* Deserialization */
            ArduinoJson::DynamicJsonDocument doc(1024);
            ArduinoJson::DeserializationError error = deserializeJson(doc, string_buffer);
            if (error) {
                printf("[%s] deserializeJson error\n", getAppName().c_str());
                return false;
            }


            /* Store data */
            _custom_data.wf_name = doc["name"].as<std::string>();

            if (doc["use_gif_background"].as<std::string>() == "yes") {
                _custom_data.using_gif_bg = true;
            }
            else {
                _custom_data.using_gif_bg = false;
            }

            _custom_data.pos_clock_hour_a.x = doc["pos_clock_hour_a"][0];
            _custom_data.pos_clock_hour_a.y = doc["pos_clock_hour_a"][1];

            _custom_data.pos_clock_hour_b.x = doc["pos_clock_hour_b"][0];
            _custom_data.pos_clock_hour_b.y = doc["pos_clock_hour_b"][1];

            _custom_data.pos_clock_colon.x = doc["pos_clock_colon"][0];
            _custom_data.pos_clock_colon.y = doc["pos_clock_colon"][1];

            _custom_data.pos_clock_min_a.x = doc["pos_clock_min_a"][0];
            _custom_data.pos_clock_min_a.y = doc["pos_clock_min_a"][1];

            _custom_data.pos_clock_min_b.x = doc["pos_clock_min_b"][0];
            _custom_data.pos_clock_min_b.y = doc["pos_clock_min_b"][1];

            printf("%s %d : %d,%d %d,%d %d,%d %d,%d %d,%d\n", 
                _custom_data.wf_name.c_str(),
                _custom_data.using_gif_bg,
                _custom_data.pos_clock_hour_a.x,
                _custom_data.pos_clock_hour_a.y,
                _custom_data.pos_clock_hour_b.x,
                _custom_data.pos_clock_hour_b.y,
                _custom_data.pos_clock_colon.x,
                _custom_data.pos_clock_colon.y,
                _custom_data.pos_clock_min_a.x,
                _custom_data.pos_clock_min_a.y,
                _custom_data.pos_clock_min_b.x,
                _custom_data.pos_clock_min_b.y
            );

            return true;
        }


        bool WF_User_Custom::_create_watch_face()
        {
            /* Get description */
            if (!_get_face_description()) {
                return false;
            }

            /* Delect selector panel */
            lv_obj_del(_data.panel_wf_select);


            /* Create imgs */
            std::string string_buffer;

            /* Base path */
            string_buffer = _custom_data.wf_folder_path;
            string_buffer += "/";
            string_buffer += _custom_data.wf_current_using_path;
            string_buffer += "/";

            /* Background */
            string_buffer += _custom_data.wf_custom_bg_asset_path;
            string_buffer += _custom_data.wf_custom_bg_asset_name;

            if (_custom_data.using_gif_bg) {
                /* GIF */
                string_buffer += _custom_data.wf_custom_gif_asset_suffix;
                printf("%s\n", string_buffer.c_str());

                _data.img_bk_gif = lv_gif_create(_data.screen);
                lv_gif_set_src(_data.img_bk_gif, string_buffer.c_str());
                lv_obj_align(_data.img_bk_gif, LV_ALIGN_CENTER, 0, 0);
            }
            else {
                /* Static */
                string_buffer += _custom_data.wf_custom_static_asset_suffix;
                printf("%s\n", string_buffer.c_str());

                _data.img_bg_static = lv_img_create(_data.screen);
                lv_img_set_src(_data.img_bg_static, string_buffer.c_str());
                lv_obj_align(_data.img_bg_static, LV_ALIGN_CENTER, 0, 0);
            }


            /* Clock */
            _data.img_clock_min_b = lv_img_create(_data.screen);
            lv_obj_align(_data.img_clock_min_b, LV_ALIGN_CENTER, _custom_data.pos_clock_min_b.x, _custom_data.pos_clock_min_b.y);
            _data.img_clock_min_a = lv_img_create(_data.screen);
            lv_obj_align(_data.img_clock_min_a, LV_ALIGN_CENTER, _custom_data.pos_clock_min_a.x, _custom_data.pos_clock_min_a.y);

            _data.img_clock_colon = lv_img_create(_data.screen);
            lv_obj_align(_data.img_clock_colon, LV_ALIGN_CENTER, _custom_data.pos_clock_colon.x, _custom_data.pos_clock_colon.y);

            _data.img_clock_hour_b = lv_img_create(_data.screen);
            lv_obj_align(_data.img_clock_hour_b, LV_ALIGN_CENTER, _custom_data.pos_clock_hour_b.x, _custom_data.pos_clock_hour_b.y);
            _data.img_clock_hour_a = lv_img_create(_data.screen);
            lv_obj_align(_data.img_clock_hour_a, LV_ALIGN_CENTER, _custom_data.pos_clock_hour_a.x, _custom_data.pos_clock_hour_a.y);

            return true;
        }


        void WF_User_Custom::onSetup()
        {
            setAppName("Cutom Watch Face");
            setAllowBgRunning(false);
            setAppIcon((void*)&ui_img_app_icon_hdpi_wf_user_custom_png);
        }


        /* Life cycle */
        void WF_User_Custom::onCreate()
        {
            printf("[%s] onCreate\n", getAppName().c_str());

            /* make data goes default */
            {
                WF_USER_CUSTOM::CustomData_t custom_data;
                _custom_data = custom_data;
                WF_USER_CUSTOM::Data_t data;
                _data = data;
            }

            /* Get data's pointer from database */
            _data.time_ptr = (DataTime_t*)getDatabase()->Get(MC_TIME)->addr;
            _data.just_wake_up_ptr = (bool*)getDatabase()->Get(MC_JUST_WAKEUP)->addr;

            _data.key_home_ptr = (bool*)getDatabase()->Get(MC_KEY_HOME)->addr;
            /* Reset at first */
            *_data.key_home_ptr = false;


            /* Create screen */
            _data.screen = lv_obj_create(NULL);
            lv_scr_load_anim(_data.screen, LV_SCR_LOAD_ANIM_FADE_IN, 50, 0, false);
            /* Set background color */
            lv_obj_set_style_bg_color(_data.screen, lv_color_hex(0x000000), LV_STATE_DEFAULT);
            /* Lock scroll */
            lv_obj_set_scroll_dir(_data.screen, LV_DIR_NONE);
            /* Add event callback */
            lv_obj_add_event_cb(_data.screen, _lvgl_event_cb, LV_EVENT_ALL, (void*)this);


            /* Select watch face */
            if (!_choose_watch_face()) {
                #ifdef ESP_PLATFORM
                /* Go back home with anim will crash */
                lv_obj_t* scr = lv_obj_create(NULL);
                lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
                #endif
                destroyApp();
            }

        }


        void WF_User_Custom::onResume()
        {
            printf("[%s] onResume\n", getAppName().c_str());
        }


        void WF_User_Custom::onRunning()
        {
            /* If is all setup */
            if (_data.running) {

                /* Update data */
                if ((lv_tick_get() - _data.update_count) > _data.update_interval) {
                    _update_data();
                    _data.update_count = lv_tick_get();
                }

                /* If just wake up */
                if (*_data.just_wake_up_ptr) {
                    /* Reset */
                    *_data.just_wake_up_ptr = false;

                    /* Update data at once */
                    _update_data();
                }

                /* If pressed key Home */
                if (*_data.key_home_ptr) {
                    /* Reset */
                    *_data.key_home_ptr = false;

                    /* Quit */
                    #ifdef ESP_PLATFORM
                    /* Go back home with anim will crash */
                    lv_obj_t* scr = lv_obj_create(NULL);
                    lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
                    #endif
                    destroyApp();
                }

            }
            else {
                
                /* If watch face selected */
                if (_custom_data.wf_current_using_path != "null") {
                    _data.running = true;

                    if (!_create_watch_face()) {
                        #ifdef ESP_PLATFORM
                        /* Go back home with anim will crash */
                        lv_obj_t* scr = lv_obj_create(NULL);
                        lv_scr_load_anim(scr, LV_SCR_LOAD_ANIM_NONE, 0, 0, true);
                        #endif
                        destroyApp();
                        return;
                    }
                    _update_data();
                }

            }
        }


        void WF_User_Custom::onRunningBG()
        {

        }


        void WF_User_Custom::onPause()
        {
            printf("[%s] onPause\n", getAppName().c_str());
        }


        void WF_User_Custom::onDestroy()
        {
            printf("[%s] onDestroy\n", getAppName().c_str());
        }



    }
}

