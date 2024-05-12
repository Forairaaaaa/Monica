/**
 * @file app_manager.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "app_register.h"


namespace MOONCAKE {


    enum APPLifecycleEvent_t {
        ON_SETUP = 0,
        ON_CREATE,
        ON_CREATE_PAUSE,
        ON_CREATE_DESTROY,
        ON_RESUME,
        ON_RUNNING,
        ON_RUNNING_BG,
        ON_PAUSE,
        ON_PAUSE_DESTROY,
        ON_DESTROY
    };


    struct APPManager_t {
        APP_BASE* app = nullptr;
        APPLifecycleEvent_t event = ON_SETUP;
    };


    class APP_Manger : public APP_Register {
        private:
            std::vector<APPManager_t> _running_apps;
            APP_BASE* _foreground_app;
            bool _update_first_element;


        public:
            APP_Manger() : 
                _foreground_app(nullptr),
                _update_first_element(false) {}
            ~APP_Manger() = default;


            /* Strat App activity */
            bool startApp(APP_BASE* app);
            inline bool startApp(int id) { return startApp(getApp(id)); }
            inline bool startApp(const char* name) { return startApp(getApp(name)); }

            /* Close App activity and put into backgroud running */
            bool closeApp(APP_BASE* app);
            inline bool closeApp(int id) { return closeApp(getApp(id)); }
            inline bool closeApp(const char* name) { return closeApp(getApp(name)); }

            /* Destroy App activity at once  */
            bool destroyApp(APP_BASE* app);
            inline bool destroyApp(int id) { return destroyApp(getApp(id)); }
            inline bool destroyApp(const char* name) { return destroyApp(getApp(name)); }
            void destroyAllApps();

            bool isAppRunning(APP_BASE* app);
            inline bool isAppRunning(int id) { return isAppRunning(getApp(id)); }
            inline bool isAppRunning(const char* name) { return isAppRunning(getApp(name)); }

            bool isAnyAppRunningFG();


            /* Call this in loop to keep App running */
            void update();

    };


}
