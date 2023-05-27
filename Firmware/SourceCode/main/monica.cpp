/**
 * @file monica.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-18
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <mooncake.h>
#include "user_apps/user_apps.h"
#include "hardware_manager/hardware_manager.h"
#include "user_apps/public_assets/assets.h"

#include <lv_examples.h>


static MOONCAKE::Framework mooncake_ui;
static HM::Hardware_Manager hardware_manager;





int btn_value = 0;

class AppTest : public MOONCAKE::APP_BASE {
    private:

        lv_obj_t* screen;
        int bbb1;
        int bbb2;

        uint32_t ticks;

        static void event_handler(lv_event_t * e)
        {
            lv_event_code_t code = lv_event_get_code(e);

            if(code == LV_EVENT_CLICKED) {

                btn_value = *(int*)lv_event_get_user_data(e);
                
            }

        }


    public:
        AppTest(const char* name, void* icon = nullptr) {
            setAppName(name);
            setAppIcon(icon);
        }

        void onSetup() {
        }

        /* Life cycle */
        void onCreate() {
            printf("[%s] onCreate\n", getAppName().c_str());

            setAllowBgRunning(false);

        }
        void onResume() {
            printf("[%s] onResume\n", getAppName().c_str());


            btn_value = 0;


            screen = lv_obj_create(NULL);
            lv_scr_load_anim(screen, LV_SCR_LOAD_ANIM_MOVE_TOP, 50, 0, false);
            

            lv_obj_t * label;

            lv_obj_t * btn1 = lv_btn_create(screen);
            bbb1 = 1;
            lv_obj_add_event_cb(btn1, event_handler, LV_EVENT_ALL, (void*)&bbb1);
            lv_obj_align(btn1, LV_ALIGN_CENTER, 0, -40);

            label = lv_label_create(btn1);
            // lv_obj_add_flag(btn1, LV_OBJ_FLAG_CHECKABLE);
            lv_obj_add_state(btn1, LV_STATE_CHECKED);
            lv_label_set_text(label, "Quit App");
            lv_obj_center(label);



            lv_obj_t * btn2 = lv_btn_create(screen);
            bbb2 = 2;
            lv_obj_add_event_cb(btn2, event_handler, LV_EVENT_ALL, (void*)&bbb2);
            lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 40);

            label = lv_label_create(btn2);
            lv_label_set_text(label, "Run background");
            lv_obj_center(label);



        }
        void onRunning() {
            // printf("[%s] onRunning\n", getAppName().c_str());

            if ((lv_tick_get() - ticks) > 1000) {
                printf("[%s] onRunning\n", getAppName().c_str());
                ticks = lv_tick_get();
            }


            // printf("%d\n", btn_value);
            if (btn_value == 1) {
                setAllowBgRunning(false);
                destroyApp();
            }
            else if (btn_value == 2) {
                setAllowBgRunning(true);
                closeApp();
            }



        }   
        void onRunningBG() {
            // printf("[%s] onRunningBG\n", getAppName().c_str());

            if ((lv_tick_get() - ticks) > 1000) {
                printf("[%s] onRunningBG\n", getAppName().c_str());
                ticks = lv_tick_get();
            }
        }
        void onPause() {
            printf("[%s] onPause\n", getAppName().c_str());
        }
        void onDestroy() {
            printf("[%s] onDestroy\n", getAppName().c_str());
        }
};






extern "C" void app_main(void)
{


    /* Hardware init */
    hardware_manager.init();





    // static const char* anim_imgs[] = {
    //     "A:sdcard/watch_faces/flowers_1/flower_01.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_02.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_03.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_04.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_05.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_06.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_07.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_08.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_09.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_10.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_11.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_12.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_13.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_14.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_15.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_16.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_17.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_18.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_19.bin",
    //     "A:sdcard/watch_faces/flowers_1/flower_20.bin",
    // };



    // lv_obj_t * animimg0 = lv_animimg_create(lv_scr_act());
    // lv_obj_center(animimg0);
    // lv_animimg_set_src(animimg0, (const void **) anim_imgs, 20);
    // lv_animimg_set_duration(animimg0, 1000);
    // lv_animimg_set_repeat_count(animimg0, LV_ANIM_REPEAT_INFINITE);
    // lv_animimg_start(animimg0);









    // lv_example_rlottie_1();


    // extern const uint8_t lv_example_rlottie_approve[];
    // lv_obj_t * lottie = lv_rlottie_create_from_raw(lv_scr_act(), 100, 100, (const char *)lv_example_rlottie_approve);
    // lv_obj_center(lottie);









    // while (1) {
    //     lv_timer_handler();
    //     vTaskDelay(5);
    // }






    

    // /* UI framwork init */
    mooncake_ui.setDisplay(hardware_manager.disp.width(), hardware_manager.disp.height());
    mooncake_ui.init();

    /* Set to same database */
    hardware_manager.setDatabase(mooncake_ui.getDatabase());






    /* Install Apps */
    MOONCAKE::APP_BASE* app_ptr = nullptr;

    app_ptr = new AppTest("111", (void*)&ui_img_app_icon_hdpi_badminton_png);
    mooncake_ui.install(app_ptr);
    app_ptr = new AppTest("222", (void*)&ui_img_app_icon_hdpi_birdhead_png);
    mooncake_ui.install(app_ptr);
    app_ptr = new AppTest("333", (void*)&ui_img_app_icon_hdpi_boxing_png);
    mooncake_ui.install(app_ptr);
    app_ptr = new AppTest("444", (void*)&ui_img_app_icon_hdpi_camera_png);
    mooncake_ui.install(app_ptr);
    app_ptr = new AppTest("555", (void*)&ui_img_app_icon_hdpi_canvas_png);
    mooncake_ui.install(app_ptr);
    app_ptr = new AppTest("666", (void*)&ui_img_app_icon_hdpi_cheers_png);
    mooncake_ui.install(app_ptr);
    app_ptr = new AppTest("777", (void*)&ui_img_app_icon_hdpi_location_png);
    mooncake_ui.install(app_ptr);
    app_ptr = new AppTest("888", (void*)&ui_img_app_icon_hdpi_music_png);
    mooncake_ui.install(app_ptr);
    app_ptr = new AppTest("999", (void*)&ui_img_app_icon_hdpi_pingpong_png);
    mooncake_ui.install(app_ptr);
    app_ptr = new AppTest("1010", (void*)&ui_img_app_icon_hdpi_weather_png);
    mooncake_ui.install(app_ptr);









    while (1) {
        hardware_manager.update();
        mooncake_ui.update();
        vTaskDelay(pdMS_TO_TICKS(2));
    }

}
