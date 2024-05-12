/**
 * @file launcher.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "launcher.h"
#include "../../system_data_def.h"
#include "../assets/assets.h"


#define SCROLL_VER          0
#define ICON_ZOOM_LIMIT     32
#define PLAY_WALKING_ANIM   1


namespace MOONCAKE {
    namespace BUILTIN_APP {


        void Launcher::_lvgl_event_cb(lv_event_t* e)
        {
            /* Get event code */
            lv_event_code_t code = lv_event_get_code(e);

            /* Start App */
            if(code == LV_EVENT_SHORT_CLICKED) {
                /* Get framework pointer */
                Framework* framework = (Framework*)lv_event_get_user_data(e);
                /* Get App pointer */
                APP_BASE* app = (APP_BASE*)lv_obj_get_user_data(lv_event_get_target(e));
                /* Start App */
                framework->startApp(app);
            }

            /* Pressed feedback */
            else if (code == LV_EVENT_PRESSED) {
                /* If pressed, smaller Icon */
                lv_img_set_zoom(lv_event_get_target(e), lv_img_get_zoom(lv_event_get_target(e)) - 10);
            }
            else if (code == LV_EVENT_RELEASED) {
                /* If released, set it back */
                lv_img_set_zoom(lv_event_get_target(e), lv_img_get_zoom(lv_event_get_target(e)) + 10);
            }

            /* App infos */
            else if (code == LV_EVENT_LONG_PRESSED) {
                /* Get App pointer */
                APP_BASE* app = (APP_BASE*)lv_obj_get_user_data(lv_event_get_target(e));
                // printf("%s\n", app->getAppName().c_str());

                /* Draw a message box to show App infos */
                static const char * btns[] = {""};
                std::string app_infos;
                app_infos = "Name:   " + app->getAppName() + "\nAllow BG running:   ";
                if (app->isAllowBgRunning()) {
                    app_infos += "Yes";
                }
                else {
                    app_infos += "No";
                }
                lv_obj_t * mbox1 = lv_msgbox_create(NULL, app->getAppName().c_str(), app_infos.c_str(), btns, true);
                lv_obj_center(mbox1);
            }

            /* If scrolling, update Icon zooming */
            else if (code == LV_EVENT_SCROLL) {
                /* Get launcher pointer */
                Launcher* launcher = (Launcher*)lv_event_get_user_data(e);
                launcher->updateAppIconZoom();
            }

        }


        void Launcher::updateAppIconZoom()
        {
            #if SCROLL_VER

            /* Zoom the Icons when reach edge */
            lv_coord_t scroll_bar_y = lv_obj_get_scroll_y(_data.appPanel);
            lv_coord_t zoom_area_half_height = _data.appPanelVer / 4;
            lv_coord_t zoom_area_edge_t = scroll_bar_y + _data.appPanelVer / 4;
            lv_coord_t zoom_area_edge_m = scroll_bar_y + _data.appPanelVer / 2;
            lv_coord_t zoom_area_edge_b = scroll_bar_y + _data.appPanelVer / 4 * 3;
            lv_coord_t icon_y = 0;
            int icon_zoom = 256;
            
            /* Iterate all Icons */
            for (int i = 0; i < lv_obj_get_child_cnt(_data.appPanel); i++) {
                /* Update Icon y */
                icon_y = lv_obj_get_y2(lv_obj_get_child(_data.appPanel, i));
                /* If at not zoom area */
                if ((icon_y >= zoom_area_edge_t) && (icon_y <= zoom_area_edge_b)) {
                    /* Zoom to normal */
                    icon_zoom = 256;
                }
                else {
                    /* Get how far Icon is out of edge */
                    icon_zoom = abs(icon_y - zoom_area_edge_m) - zoom_area_half_height;
                    /* Smaller it */
                    icon_zoom = 256 - icon_zoom;
                    /* If hit limit */
                    if (icon_zoom < ICON_ZOOM_LIMIT) {
                        icon_zoom = ICON_ZOOM_LIMIT;
                    }
                }
                /* Set zoom */
                lv_img_set_zoom(lv_obj_get_child(_data.appPanel, i), icon_zoom);
            }

            #else

            /* Zoom the Icons when reach edge */
            lv_coord_t scroll_bar_y = lv_obj_get_scroll_x(_data.appPanel);
            lv_coord_t zoom_area_half_height = _data.appPanelHor / 4;
            lv_coord_t zoom_area_edge_t = scroll_bar_y + _data.appPanelHor / 4;
            lv_coord_t zoom_area_edge_m = scroll_bar_y + _data.appPanelHor / 2;
            lv_coord_t zoom_area_edge_b = scroll_bar_y + _data.appPanelHor / 4 * 3;
            lv_coord_t icon_y = 0;
            int icon_zoom = 256;
            
            /* Iterate all Icons */
            for (int i = 0; i < lv_obj_get_child_cnt(_data.appPanel); i++) {
                /* Update Icon y */
                icon_y = lv_obj_get_x2(lv_obj_get_child(_data.appPanel, i));
                /* If at not zoom area */
                if ((icon_y >= zoom_area_edge_t) && (icon_y <= zoom_area_edge_b)) {
                    /* Zoom to normal */
                    icon_zoom = 256;
                }
                else {
                    /* Get how far Icon is out of edge */
                    icon_zoom = abs(icon_y - zoom_area_edge_m) - zoom_area_half_height;
                    icon_zoom = icon_zoom / 3 * 2;
                    /* Smaller it */
                    icon_zoom = 256 - icon_zoom;
                    /* If hit limit */
                    if (icon_zoom < ICON_ZOOM_LIMIT) {
                        icon_zoom = ICON_ZOOM_LIMIT;
                    }
                }
                /* Set zoom */
                lv_img_set_zoom(lv_obj_get_child(_data.appPanel, i), icon_zoom);
            }

            #endif
        }


        void Launcher::updateInfos()
        {
            /* Update time */
            if (getDatabase()->Get(MC_TIME)->addr != nullptr) {
                /* Hour */
                snprintf(_data.infoUpdateBuffer, sizeof(_data.infoUpdateBuffer), "%.2d", getDatabase()->Get(MC_TIME)->value<DataTime_t>().hour);
                lv_label_set_text(_data.infoClockHour, _data.infoUpdateBuffer);
                /* Min */
                snprintf(_data.infoUpdateBuffer, sizeof(_data.infoUpdateBuffer), "%.2d", getDatabase()->Get(MC_TIME)->value<DataTime_t>().min);
                lv_label_set_text(_data.infoClockMin, _data.infoUpdateBuffer);
            }

            /* Update Battery */
            if (getDatabase()->Get(MC_BATTERY_LEVEL)->addr != nullptr) {
                /* Level */
                snprintf(_data.infoUpdateBuffer, sizeof(_data.infoUpdateBuffer), "%d%%", getDatabase()->Get(MC_BATTERY_LEVEL)->value<uint8_t>());
                lv_label_set_text(_data.infoBatLevel, _data.infoUpdateBuffer);
            }

            /* Update step counter */
            if (getDatabase()->Get(MC_STEPS)->addr != nullptr) {
                /* Level */
                snprintf(_data.infoUpdateBuffer, sizeof(_data.infoUpdateBuffer), "%ld steps!", getDatabase()->Get(MC_STEPS)->value<uint32_t>());
                lv_label_set_text(_data.infoStepCounter, _data.infoUpdateBuffer);
            }
        }


        void Launcher::_create_app_panel()
        {
            /**
             * @brief App panel
             * 
             */

            /* Set panel size */
            _data.appPanelHor = *_data.dispHor;
            _data.appPanelVer = *_data.dispVer / 2;

            /* Create a panel */
            _data.appPanel = lv_obj_create(_data.screenMain);
            lv_obj_set_size(_data.appPanel, _data.appPanelHor, _data.appPanelVer);
            lv_obj_align(_data.appPanel, LV_ALIGN_BOTTOM_MID, 0, 0);

            /* Ser style */
            lv_obj_set_style_radius(_data.appPanel, 0, LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(_data.appPanel, 0, LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(_data.appPanel, 0, LV_STATE_DEFAULT);

            /* Add scroll flags */
            lv_obj_add_flag(_data.appPanel, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM);
            lv_obj_set_scrollbar_mode(_data.appPanel, LV_SCROLLBAR_MODE_OFF);

            #if SCROLL_VER
            lv_obj_set_scroll_dir(_data.appPanel, LV_DIR_VER);
            #else 
            lv_obj_set_scroll_dir(_data.appPanel, LV_DIR_HOR);
            #endif
            
            lv_obj_add_event_cb(_data.appPanel, _lvgl_event_cb, LV_EVENT_SCROLL, (void*)this);


            /**
             * @brief App bubble pool
             * 
             */

            /* Update bubble config */
            _bubble_cfg.iconColMax = _data.appPanelHor / USING_ICON.header.w;
            _bubble_cfg.iconRowMax = _data.appPanelVer / USING_ICON.header.h;
            _bubble_cfg.iconColNum = (_framework->getAppNum() - 1) / _bubble_cfg.iconRowMax;
            if (((_framework->getAppNum() - 1) % _bubble_cfg.iconRowMax) != 0) {
                _bubble_cfg.iconColNum++;
            }
            _bubble_cfg.iconSpaceX = _data.appPanelHor / _bubble_cfg.iconColMax;
            lv_coord_t gap_between_icon = (_data.appPanelHor - USING_ICON.header.w * _bubble_cfg.iconColMax) / (_bubble_cfg.iconColMax + 1);
            _bubble_cfg.iconSpaceY = USING_ICON.header.h - (gap_between_icon / 2);
            _bubble_cfg.iconXoffset = -(_data.appPanelHor / 2) + (_bubble_cfg.iconSpaceX / 2);
            _bubble_cfg.iconYoffset = -(_data.appPanelVer / 2) + (_bubble_cfg.iconSpaceY / 2) + gap_between_icon;


            int icon_x = 0;
            int icon_y = 0;
            /* Long row first */
            bool is_long_row = true;

            /* Put App Icon into bubble pool */
            for (auto i : _framework->getAppList()) {
                /* If is launcher */
                if (i.app == this) {
                    continue;
                }

                /* Create a object */
                lv_obj_t* app = lv_img_create(_data.appPanel);
                lv_obj_center(app);

                /* If App Icon is not set, use default */
                if (i.app->getAppIcon() == nullptr) {
                    lv_img_set_src(app, &USING_ICON);
                }
                else {
                    lv_img_set_src(app, i.app->getAppIcon());
                }



                /**
                 * @brief Vertical
                 * 
                 */
                #if SCROLL_VER

                /* Put App in hexagon mesh */
                if (!is_long_row) {
                    lv_obj_set_pos(app, icon_x + _bubble_cfg.iconSpaceX / 2 + _bubble_cfg.iconXoffset, icon_y + _bubble_cfg.iconYoffset);
                }
                else {
                    lv_obj_set_pos(app, icon_x + _bubble_cfg.iconXoffset, icon_y + _bubble_cfg.iconYoffset);
                }

                /* Go to next col */
                icon_x += _bubble_cfg.iconSpaceX;

                /* Go to next more Apps row */
                if (!is_long_row && ((icon_x / _bubble_cfg.iconSpaceX) >= (_bubble_cfg.iconColMax - 1))) {
                    is_long_row = true;
                    icon_x = 0;
                    icon_y += _bubble_cfg.iconSpaceY;
                }
                /* Go to next less Apps row */
                else if (is_long_row && ((icon_x / _bubble_cfg.iconSpaceX) >= _bubble_cfg.iconColMax)) { 
                    is_long_row = false;
                    icon_x = 0;
                    icon_y += _bubble_cfg.iconSpaceY;
                }

                /**
                 * @brief Horizontal
                 * 
                 */
                #else

                /* Put App in hexagon mesh */
                if (!is_long_row) {
                    lv_obj_set_pos(app, icon_x + _bubble_cfg.iconSpaceX / 2 + _bubble_cfg.iconXoffset, icon_y + _bubble_cfg.iconYoffset);
                }
                else {
                    lv_obj_set_pos(app, icon_x + _bubble_cfg.iconXoffset, icon_y + _bubble_cfg.iconYoffset);
                }

                /* Go to next col */
                icon_x += _bubble_cfg.iconSpaceX;

                /* Next row */
                if ((icon_x / _bubble_cfg.iconSpaceX) >= _bubble_cfg.iconColNum) {
                    is_long_row = is_long_row ? false : true;
                    printf("%d\n", is_long_row);
                    icon_x = 0;
                    icon_y += _bubble_cfg.iconSpaceY;
                }

                #endif


                /* Set App pointer as user data */
                lv_obj_set_user_data(app, (void*)i.app);
                
                /* Add event callback */
                lv_obj_add_flag(app, LV_OBJ_FLAG_CLICKABLE);
                lv_obj_set_style_img_recolor(app, lv_color_hex(0x000000), LV_STATE_PRESSED);
                lv_obj_set_style_img_recolor_opa(app, 50, LV_STATE_PRESSED);
                lv_obj_add_event_cb(app, _lvgl_event_cb, LV_EVENT_ALL, (void*)_framework);
            }

            /* Hit an event to update icon zoom once */
            #if SCROLL_VER
            lv_obj_scroll_to_y(_data.appPanel, 1, LV_ANIM_OFF);
            #else
            lv_obj_scroll_to_x(_data.appPanel, 1, LV_ANIM_OFF);
            #endif
        }


        void Launcher::_create_info_panel()
        {
            /* Set size */
            _data.infoPanelHor = *_data.dispHor;
            _data.infoPanelVer = *_data.dispVer / 2;

            /* Create info panel */
            _data.infoPanel = lv_obj_create(_data.screenMain);
            lv_obj_set_size(_data.infoPanel, _data.infoPanelHor, _data.infoPanelVer);
            lv_obj_align(_data.infoPanel, LV_ALIGN_TOP_MID, 0, 0);

            /* Ser style */
            lv_obj_set_style_radius(_data.infoPanel, 0, LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(_data.infoPanel, 0, LV_STATE_DEFAULT);
            lv_obj_set_style_border_opa(_data.infoPanel, 0, LV_STATE_DEFAULT);

            /* Add scroll flags */
            lv_obj_add_flag(_data.infoPanel, LV_OBJ_FLAG_SCROLL_ELASTIC | LV_OBJ_FLAG_SCROLL_MOMENTUM | LV_OBJ_FLAG_SCROLL_ONE);
            lv_obj_set_scrollbar_mode(_data.infoPanel, LV_SCROLLBAR_MODE_OFF);
            lv_obj_set_scroll_dir(_data.infoPanel, LV_DIR_NONE);


            /* Walking man anim */
            #if PLAY_WALKING_ANIM
            lv_obj_t* walking_man = lv_animimg_create(_data.infoPanel);
            lv_obj_align(walking_man, LV_ALIGN_CENTER, lv_pct(3), lv_pct(2));
            lv_animimg_set_src(walking_man, (const void**)anim_lc_walking, NUM_ANIM_LC_WALKING);
            lv_animimg_set_duration(walking_man, 1209);
            lv_animimg_set_repeat_count(walking_man, LV_ANIM_REPEAT_INFINITE);
            lv_animimg_start(walking_man);
            #else
            lv_obj_t* walking_man = lv_img_create(_data.infoPanel);
            lv_obj_align(walking_man, LV_ALIGN_CENTER, lv_pct(3), lv_pct(2));
            lv_img_set_src(walking_man, (const void*)&ui_img_icon_lc_walking_00_png);
            #endif

            /* Clock */
            _data.infoClockHour = lv_label_create(_data.infoPanel);
            lv_obj_set_x(_data.infoClockHour, lv_pct(-34));
            lv_obj_set_y(_data.infoClockHour, lv_pct(-25));
            lv_obj_set_align(_data.infoClockHour, LV_ALIGN_CENTER);
            lv_label_set_text(_data.infoClockHour, "23");
            lv_obj_set_style_text_color(_data.infoClockHour, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(_data.infoClockHour, &ui_font_OpenSansMedium96, LV_PART_MAIN | LV_STATE_DEFAULT);
            
            _data.infoClockMin = lv_label_create(_data.infoPanel);
            lv_obj_set_x(_data.infoClockMin, lv_pct(-34));
            lv_obj_set_y(_data.infoClockMin, lv_pct(25));
            lv_obj_set_align(_data.infoClockMin, LV_ALIGN_CENTER);
            lv_label_set_text(_data.infoClockMin, "32");
            lv_obj_set_style_text_color(_data.infoClockMin, lv_color_hex(0xBEBEBE), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(_data.infoClockMin, &ui_font_OpenSansMedium96, LV_PART_MAIN | LV_STATE_DEFAULT);


            /* Step number */
            _data.infoStepCounter = lv_label_create(_data.infoPanel);
            lv_obj_set_x(_data.infoStepCounter, lv_pct(33));
            lv_obj_set_y(_data.infoStepCounter, lv_pct(35));
            lv_obj_set_align(_data.infoStepCounter, LV_ALIGN_CENTER);
            lv_label_set_text(_data.infoStepCounter, "2366 steps!");
            lv_obj_set_style_text_color(_data.infoStepCounter, lv_color_hex(0xBEBEBE), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(_data.infoStepCounter, &ui_font_OpenSansMediumItalic24, LV_PART_MAIN | LV_STATE_DEFAULT);


            /* Battery */
            _data.infoBatIcon = lv_img_create(_data.infoPanel);
            lv_img_set_src(_data.infoBatIcon, &ui_img_icon_battery_png);
            lv_obj_set_x(_data.infoBatIcon, lv_pct(28));
            lv_obj_set_y(_data.infoBatIcon, lv_pct(-13));
            lv_obj_set_align(_data.infoBatIcon, LV_ALIGN_CENTER);

            _data.infoBatLevel = lv_label_create(_data.infoPanel);
            lv_obj_set_x(_data.infoBatLevel, lv_pct(43));
            lv_obj_set_y(_data.infoBatLevel, lv_pct(-12));
            lv_obj_set_align(_data.infoBatLevel, LV_ALIGN_CENTER);
            lv_label_set_text(_data.infoBatLevel, "96%");
            lv_obj_set_style_text_color(_data.infoBatLevel, lv_color_hex(0xBEBEBE), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(_data.infoBatLevel, &ui_font_OpenSansMediumItalic24, LV_PART_MAIN | LV_STATE_DEFAULT);

            
            /* Wifi */
            _data.infoWifiIcon = lv_img_create(_data.infoPanel);
            lv_img_set_src(_data.infoWifiIcon, &ui_img_icon_wifi_off_png);
            lv_obj_set_x(_data.infoWifiIcon, lv_pct(28));
            lv_obj_set_y(_data.infoWifiIcon, lv_pct(5));
            lv_obj_set_align(_data.infoWifiIcon, LV_ALIGN_CENTER);


            /* BLE */
            _data.infoBleIcon = lv_img_create(_data.infoPanel);
            lv_img_set_src(_data.infoBleIcon, &ui_img_icon_ble_off_png);
            lv_obj_set_x(_data.infoBleIcon, lv_pct(38));
            lv_obj_set_y(_data.infoBleIcon, lv_pct(5));
            lv_obj_set_align(_data.infoBleIcon, LV_ALIGN_CENTER);


            /* Notification */
            _data.infoNoteIcon = lv_img_create(_data.infoPanel);
            lv_img_set_src(_data.infoNoteIcon, &ui_img_icon_note_on_png);
            lv_obj_set_x(_data.infoNoteIcon, lv_pct(48));
            lv_obj_set_y(_data.infoNoteIcon, lv_pct(5));
            lv_obj_set_align(_data.infoNoteIcon, LV_ALIGN_CENTER);
        }


        void Launcher::onSetup()
        {
            setAppName("Launcher");
            setAllowBgRunning(true);

            /* Get framework control */
            _framework = (Framework*)getUserData();
        }


        /* Life cycle */
        void Launcher::onCreate()
        {
            printf("[%s] onCreate\n", getAppName().c_str());

            /* Get hardware infos from database */
            _data.dispHor = (int16_t*)getDatabase()->Get(MC_DISP_HOR)->addr;
            _data.dispVer = (int16_t*)getDatabase()->Get(MC_DISP_VER)->addr;

            /* Get display mode */
            if (*_data.dispHor < *_data.dispVer) {
                _data.dispModePortrait = true;
            }


            /* Crete main screen */
            _data.screenMain = lv_obj_create(NULL);
            /* Set background color */
            lv_obj_set_style_bg_color(_data.screenMain, _config.backGroundColor, LV_STATE_DEFAULT);
            /* Set backgound img */
            if (_config.backGroundImg != nullptr) {
                lv_obj_set_style_bg_img_src(_data.screenMain, _config.backGroundImg, 0);
            }


            /* Create panels */
            _create_info_panel();
            _create_app_panel();
        }


        void Launcher::onResume()
        {
            printf("[%s] onResume\n", getAppName().c_str());


            
            /* Load main screen and delete last one */
            if (lv_scr_act() != _data.screenMain) {
                lv_scr_load_anim(_data.screenMain, LV_SCR_LOAD_ANIM_MOVE_BOTTOM, 50, 0, true);
            }

        }


        void Launcher::onRunning()
        {
            // printf("[%s] onRunning\n", getAppName().c_str());


            /* Update infos */
            if ((_data.infoUpdateTickCount == 0) || ((lv_tick_get() - _data.infoUpdateTickCount) > _config.infoUpdateInterval)) {
                updateInfos();
                _data.infoUpdateTickCount = lv_tick_get();
            }
            

        }


        void Launcher::onRunningBG()
        {
            // printf("[%s] onRunningBG\n", getAppName().c_str());

            /* If no App is running on foreground */
            if (!_framework->isAnyAppRunningFG()) {
                _framework->startApp(this);
            }
        }


        void Launcher::onPause()
        {
            printf("[%s] onPause\n", getAppName().c_str());
        }


        void Launcher::onDestroy()
        {
            printf("[%s] onDestroy\n", getAppName().c_str());
        }


    }
}
