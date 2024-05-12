/**
 * @file app_manager.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "app_manager.h"


namespace MOONCAKE {


    bool APP_Manger::startApp(APP_BASE* app)
    {
        if (app == nullptr) {
            return false;
        }

        /* If already running */
        for (auto iter = _running_apps.begin(); iter != _running_apps.end(); iter++) {
            if (iter->app == app) {

                /* If App is running on foregound  */
                if (iter->event == ON_RUNNING) {
                    return true;
                }

                /* Send event */
                iter->event = ON_RESUME;
                /* Update foreground */
                _foreground_app = app;
                /* Setup flag to update the first element after next update */
                _update_first_element = true;                
                return true;
            }
        }

        /* If not, create a new one */
        APPManager_t new_app;
        new_app.app = app;
        new_app.event = ON_CREATE;
        _running_apps.push_back(new_app);

        /* Update foreground */
        _foreground_app = app;
        /* Setup flag to update the first element after next update */
        _update_first_element = true;  
        return true;
    }


    bool APP_Manger::closeApp(APP_BASE* app)
    {
        if (app == nullptr) {
            return false;
        }

        /* Search in running list */
        for (auto iter = _running_apps.begin(); iter != _running_apps.end(); iter++) {
            if (iter->app == app) {
                
                /* If already running on background */
                if (iter->event == ON_RUNNING_BG) {
                    return false;
                }

                /* If just start */
                if (iter->event == ON_CREATE) {
                    iter->event = ON_CREATE_PAUSE;
                    return true;
                }

                /* If called repeatly or jsut start and destroy */
                if ((iter->event == ON_CREATE_PAUSE) || (iter->event == ON_CREATE_DESTROY)) {
                    return true;
                }

                iter->event = ON_PAUSE;
                return true;
            }
        }
        return false;
    }


    bool APP_Manger::destroyApp(APP_BASE* app)
    {
        if (app == nullptr) {
            return false;
        }

        /* Search in running list */
        for (auto iter = _running_apps.begin(); iter != _running_apps.end(); iter++) {
            if (iter->app == app) {

                /* If just start or start and close */
                if ((iter->event == ON_CREATE) || (iter->event == ON_CREATE_PAUSE)) {
                    iter->event = ON_CREATE_DESTROY;
                    return true;
                }

                /* If called repeatly */
                if (iter->event == ON_CREATE_DESTROY) {
                    return true;
                }

                iter->event = ON_PAUSE_DESTROY;
                return true;
            }
        }
        return false;
    }


    void APP_Manger::destroyAllApps()
    {
        for (auto iter = _running_apps.begin(); iter != _running_apps.end(); iter++) {
            iter->app->onPause();
            iter->app->onDestroy();
        }
        /* Free list */
        _running_apps.clear();
        std::vector<APPManager_t>().swap(_running_apps);
    }


    bool APP_Manger::isAppRunning(APP_BASE* app)
    {
        for (auto iter = _running_apps.begin(); iter != _running_apps.end(); iter++) {
            if (iter->app == app) {
                return true;
            }
        }
        return false;
    }


    bool APP_Manger::isAnyAppRunningFG()
    {
        for (auto iter = _running_apps.begin(); iter != _running_apps.end(); iter++) {
            if (iter->event == ON_RUNNING) {
                return true;
            }
        }
        return false;
    }


    /**
     * @brief FSM to implement lifecycle callbacks
     * 
     */
    void APP_Manger::update()
    {
        /**
         * Because foreground App is set to be the first element, it's life cycle callbacks will be called firstly,
         * to avoid new App's "onCreate" or BG's "onResume" effect FG's "onPause", "onDestroy".
         * 
         * Moreover, "onRunning", "onPause", "onDestroy" will be called at first,
         * and new App's "onCreate" or BG's "onRunningBG", "onResume" will be called next,
         * to implement a clear order.
         * 
        */
        for (auto iter = _running_apps.begin(); iter != _running_apps.end(); ) {
            
            /* On running */
            if (iter->event == ON_RUNNING) {
                /* If is foreground App, keep running */
                if (iter->app == _foreground_app) {
                    iter->app->onRunning();
                }
                /* If not, put back to background */
                else {
                    iter->event = ON_PAUSE;
                }
            }

            /* On running backgound */
            if (iter->event == ON_RUNNING_BG) {
                iter->app->onRunningBG();
            }
            
            /* If App call "destroyApp()" internally */
            if (iter->app->isGoingDestroy()) {
                iter->app->resetGoingDestroyFlag();
                /* Destroy it */
                iter->app->onPause();
                iter->event = ON_DESTROY;
            }

            /* If App call "closeApp()" internally */
            if (iter->app->isGoingClose()) {
                iter->app->resetGoingCloseFlag();
                /* Close it */
                iter->event = ON_PAUSE;
            }

            /* On create pause */
            if (iter->event == ON_CREATE_PAUSE) {
                iter->app->onCreate();
                iter->app->onResume();
                iter->event = ON_PAUSE;
            }
            
            /* On pause */
            if (iter->event == ON_PAUSE) {
                iter->app->onPause();

                /* If allow backgroud running */
                if (iter->app->isAllowBgRunning()) {
                    iter->event = ON_RUNNING_BG;
                }
                else {
                    iter->event = ON_DESTROY;
                }
            }

            /* On create destroy */
            if (iter->event == ON_CREATE_DESTROY) {
                iter->app->onCreate();
                iter->app->onResume();
                iter->event = ON_PAUSE_DESTROY;
            }

            /* On pause destroy */
            if (iter->event == ON_PAUSE_DESTROY) {
                iter->app->onPause();
                iter->event = ON_DESTROY;
            }

            /* On destroy */
            if (iter->event == ON_DESTROY) {
                iter->app->onDestroy();
                /* Kick it out of running list */
                iter = _running_apps.erase(iter);
                continue;
            }

            /* On create */
            if (iter->event == ON_CREATE) {
                iter->app->onCreate();
                iter->event = ON_RESUME;
            }

            /* On resume */
            if (iter->event == ON_RESUME) {
                iter->app->onResume();
                iter->event = ON_RUNNING;
            }

            iter++;
        }
        
        /* If need to update first element of running list */
        if (_update_first_element) {
            _update_first_element = false;
            /* Find FG App */
            for (auto iter = _running_apps.begin(); iter != _running_apps.end(); iter++) {
                if (iter->app == _foreground_app) {
                    /* Swap it to the first one */
                    std::swap(*(_running_apps.begin()), *(iter));
                    break;
                }
            }
        }

    }



    
    


    


}
