/**
 * @file sketchpad.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-29
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "sketchpad.h"
#include "../assets/assets.h"


namespace MOONCAKE {
    namespace BUILTIN_APP {

        
        /* Color picker, copy from demo widget */
        void Sketchpad::color_changer_create(lv_obj_t * parent)
        {
            static lv_palette_t palette[] = {
                LV_PALETTE_BLUE, LV_PALETTE_GREEN, LV_PALETTE_BLUE_GREY,  LV_PALETTE_ORANGE,
                LV_PALETTE_RED, LV_PALETTE_PURPLE, LV_PALETTE_TEAL, _LV_PALETTE_LAST
            };

            lv_obj_t * color_cont = lv_obj_create(parent);
            lv_obj_remove_style_all(color_cont);
            lv_obj_set_flex_flow(color_cont, LV_FLEX_FLOW_ROW);
            lv_obj_set_flex_align(color_cont, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
            lv_obj_add_flag(color_cont, LV_OBJ_FLAG_FLOATING);

            lv_obj_set_style_bg_color(color_cont, lv_color_hex(0x232323), 0);
            lv_obj_set_style_pad_right(color_cont, LV_DPX(55), 0);
            lv_obj_set_style_bg_opa(color_cont, LV_OPA_COVER, 0);
            lv_obj_set_style_radius(color_cont, LV_RADIUS_CIRCLE, 0);

            lv_obj_set_size(color_cont, LV_DPX(60), LV_DPX(60));

            lv_obj_align(color_cont, LV_ALIGN_BOTTOM_RIGHT, - LV_DPX(10),  - LV_DPX(10));

            uint32_t i;
            for(i = 0; palette[i] != _LV_PALETTE_LAST; i++) {
                lv_obj_t * c = lv_btn_create(color_cont);
                lv_obj_set_style_bg_color(c, lv_palette_main(palette[i]), 0);
                lv_obj_set_style_radius(c, LV_RADIUS_CIRCLE, 0);
                lv_obj_set_style_opa(c, LV_OPA_TRANSP, 0);
                lv_obj_set_size(c, 20, 20);
                lv_obj_add_event_cb(c, color_event_cb, LV_EVENT_ALL, &palette[i]);
                lv_obj_clear_flag(c, LV_OBJ_FLAG_SCROLL_ON_FOCUS);
            }

            lv_obj_t * btn = lv_btn_create(parent);
            lv_obj_add_flag(btn, LV_OBJ_FLAG_FLOATING | LV_OBJ_FLAG_CLICKABLE);
            // lv_obj_set_style_bg_color(btn, lv_color_white(), LV_STATE_CHECKED);
            lv_obj_set_style_bg_color(btn, lv_color_hex(0x232323), 0);
            lv_obj_set_style_pad_all(btn, 10, 0);
            lv_obj_set_style_radius(btn, LV_RADIUS_CIRCLE, 0);
            lv_obj_add_event_cb(btn, color_changer_event_cb, LV_EVENT_ALL, color_cont);
            lv_obj_set_style_shadow_width(btn, 0, 0);
            lv_obj_set_style_bg_img_src(btn, LV_SYMBOL_TINT, 0);

            lv_obj_set_size(btn, LV_DPX(50), LV_DPX(50));
            lv_obj_align(btn, LV_ALIGN_BOTTOM_RIGHT, -LV_DPX(15), -LV_DPX(15));
        }


        void Sketchpad::color_changer_anim_cb(void * var, int32_t v)
        {
            lv_obj_t * obj = (lv_obj_t *)var;
            lv_coord_t max_w = lv_obj_get_width(lv_obj_get_parent(obj)) - LV_DPX(20);
            lv_coord_t w;

            w = lv_map(v, 0, 256, LV_DPX(60), max_w);
            lv_obj_set_width(obj, w);
            lv_obj_align(obj, LV_ALIGN_BOTTOM_RIGHT, - LV_DPX(10),  - LV_DPX(10));

            if(v > LV_OPA_COVER) v = LV_OPA_COVER;

            uint32_t i;
            for(i = 0; i < lv_obj_get_child_cnt(obj); i++) {
                lv_obj_set_style_opa(lv_obj_get_child(obj, i), v, 0);
            }

        }


        void Sketchpad::color_changer_event_cb(lv_event_t * e)
        {
            if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
                lv_obj_t * color_cont = (lv_obj_t *)lv_event_get_user_data(e);
                if(lv_obj_get_width(color_cont) < LV_HOR_RES / 2) {
                    lv_anim_t a;
                    lv_anim_init(&a);
                    lv_anim_set_var(&a, color_cont);
                    lv_anim_set_exec_cb(&a, color_changer_anim_cb);
                    lv_anim_set_values(&a, 0, 256);
                    lv_anim_set_time(&a, 200);
                    lv_anim_start(&a);
                }
                else {
                    lv_anim_t a;
                    lv_anim_init(&a);
                    lv_anim_set_var(&a, color_cont);
                    lv_anim_set_exec_cb(&a, color_changer_anim_cb);
                    lv_anim_set_values(&a, 256, 0);
                    lv_anim_set_time(&a, 200);
                    lv_anim_start(&a);
                }
            }
        }


        void Sketchpad::color_event_cb(lv_event_t * e)
        {
            lv_event_code_t code = lv_event_get_code(e);
            lv_obj_t* scr = lv_obj_get_screen(lv_event_get_target(e));
            Sketchpad* app = (Sketchpad*)lv_obj_get_user_data(scr);

            if(code == LV_EVENT_CLICKED) {
                lv_palette_t * palette_primary = (lv_palette_t *)lv_event_get_user_data(e);
               
                app->_data.pen_color = lv_palette_main(*palette_primary);
            }
        }


        void Sketchpad::_lvgl_event_cb(lv_event_t* e)
        {
            /* Get event code */
            lv_event_code_t code = lv_event_get_code(e);

            /* Quit */
            #ifndef ESP_PLATFORM
            if (code == LV_EVENT_GESTURE) {
                // printf("gesture\n");

                Sketchpad* app = (Sketchpad*)lv_event_get_user_data(e);

                if (lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_BOTTOM) {
                    app->destroyApp();
                }
            }
            #endif

            /* Drawing */
            if (code == LV_EVENT_PRESSING) {
                Sketchpad* app = (Sketchpad*)lv_event_get_user_data(e);
                app->_update_drawing();
            }
            
        }


        void Sketchpad::_update_drawing()
        {
            lv_indev_get_point(lv_indev_get_act(), &_data.touchPoint);
            // printf("%d %d\n", touchPoint.x, touchPoint.y);

            lv_draw_rect_dsc_t rect;
            lv_draw_rect_dsc_init(&rect);
            rect.radius = LV_RADIUS_CIRCLE;
            rect.bg_color = _data.pen_color;
            lv_canvas_draw_rect(_data.canvas, _data.touchPoint.x, _data.touchPoint.y, _data.pen_size, _data.pen_size, &rect);

        }


        void Sketchpad::onSetup()
        {
            setAppName("Sketchpad");
            setAllowBgRunning(false);
            setAppIcon((void*)&ui_img_app_icon_hdpi_sketchpad_png);
        }


        /* Life cycle */
        void Sketchpad::onCreate()
        {
            printf("[%s] onCreate\n", getAppName().c_str());


            /* Reset data to default */
            {
                SKETCHPAD::Data_t data;
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


            /* Create canvas buffer */
            // static lv_color_t cbuf[LV_CANVAS_BUF_SIZE_TRUE_COLOR(CANVAS_WIDTH, CANVAS_HEIGHT)];
            _data.canvas_buffer = new lv_color_t[LV_CANVAS_BUF_SIZE_TRUE_COLOR(lv_obj_get_width(_data.screen), lv_obj_get_height(_data.screen))];
            printf("[%s] alloc buffer %d x %d\n", getAppName().c_str(), lv_obj_get_width(_data.screen), lv_obj_get_height(_data.screen));

            /*Create a canvas and initialize its palette*/
            _data.canvas = lv_canvas_create(_data.screen);
            lv_canvas_set_buffer(_data.canvas, _data.canvas_buffer, lv_obj_get_width(_data.screen), lv_obj_get_height(_data.screen), LV_IMG_CF_TRUE_COLOR);
            lv_obj_center(_data.canvas);
            lv_canvas_fill_bg(_data.canvas, lv_color_hex(0x000000), LV_OPA_COVER);

            /* Create a color picker */
            color_changer_create(_data.screen);

        }


        void Sketchpad::onResume()
        {
            printf("[%s] onResume\n", getAppName().c_str());
        }


        void Sketchpad::onRunning()
        {
            /* If pressed key Home */
            if (*_data.key_home_ptr) {
                /* Reset */
                *_data.key_home_ptr = false;

                /* Quit */
                destroyApp();
            }
        }


        void Sketchpad::onRunningBG()
        {

        }


        void Sketchpad::onPause()
        {
            printf("[%s] onPause\n", getAppName().c_str());
        }


        void Sketchpad::onDestroy()
        {
            printf("[%s] onDestroy\n", getAppName().c_str());
            
            /* Free buffer */
            /* Stop lvgl using that buffer before freeing */
            lv_canvas_set_buffer(_data.canvas, _data.canvas_buffer, 0, 0, LV_IMG_CF_TRUE_COLOR);
            delete [] _data.canvas_buffer;
        }


    }
}
