/**
 * @file framework.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "framework.h"
#include "../builtin_apps/builtin_apps.h"
#include "../system_data_def.h"


namespace MOONCAKE {


    bool Framework::_run_boot_anim()
    {
        /* If boot anim is not set */
        bool using_builtin_bootAnim = false;
        if (_config.bootAnim == nullptr) {
            using_builtin_bootAnim = true;
            _config.bootAnim = new BUILTIN_APP::Boot_Anim;
            if (_config.bootAnim == nullptr) {
                return false;
            }
        }
        /* Run Anim until finished */
        APP_Register::install(_config.bootAnim, nullptr, nullptr);
        startApp(_config.bootAnim);
        while (1) {
            APP_Manger::update();
            if (!isAppRunning(_config.bootAnim)) {
                break;
            }
        }
        APP_Register::uninstall(_config.bootAnim);
        if (using_builtin_bootAnim) {
            delete _config.bootAnim;
            _config.bootAnim = nullptr;
        }
        return true;
    }


    void Framework::_system_data_init()
    {
        /* Clear database */
        _config.database->DeleteAll();

        /* Navigation */
        #ifdef MC_KEY_MENU
        _config.database->Add<bool>(MC_KEY_MENU, bool(false));
        _config.database->Add<bool>(MC_KEY_HOME, bool(false));
        _config.database->Add<bool>(MC_KEY_BACK, bool(false));
        _config.database->Add<bool>(MC_KEY_POWER, bool(false));
        _config.database->Add<bool>(MC_KEY_UP, bool(false));
        _config.database->Add<bool>(MC_KEY_DOWN, bool(false));
        #endif

        /* Time */
        #ifdef MC_TIME
        _config.database->Add<DataTime_t>(MC_TIME, DataTime_t());
        _config.database->Add<bool>(MC_TIME_JSUT_SET, bool(false));
        #endif

        /* Hardware */
        #ifdef MC_DISP_HOR

        /* Display */
        _config.database->Add<int16_t>(MC_DISP_HOR, _config.displayHor);
        _config.database->Add<int16_t>(MC_DISP_VER, _config.displayVer);
        _config.database->Add<uint8_t>(MC_DISP_BRIGHTNESS, uint8_t(100));

        /* System tick */
        _config.database->Add<uint32_t>(MC_SYSTEM_TICKS, uint32_t(0));

        /* Power */
        _config.database->Add<uint8_t>(MC_BATTERY_LEVEL, uint8_t(100));
        _config.database->Add<bool>(MC_BATTERY_IS_CHARGING, bool(false));

        /* Wireless */
        _config.database->Add<bool>(MC_WIFI_IS_CONNECTED, bool(false));
        _config.database->Add<bool>(MC_BLE_IS_CONNECTED, bool(false));

        /* Notification */
        _config.database->Add<bool>(MC_NOTIFICATION_IS_ON, bool(false));

        /* Steps */
        _config.database->Add<uint32_t>(MC_STEPS, uint32_t(2333));

        /* Flag of just wake up from sleep */
        _config.database->Add<bool>(MC_JUST_WAKEUP, bool(false));

        #endif
    }


    bool Framework::init()
    {
        if (_inited) {
            return false;
        }

        /* Clear running Apps */
        destroyAllApps();

        /* If database is set */
        if (_config.database != nullptr) {
            _using_builtin_database = false;
        }
        else {
            _config.database = new SIMPLEKV::SimpleKV;
            if (_config.database == nullptr) {
                return false;
            }
        }

        /* Init basic system data */
        _system_data_init();

        /* Boot anim */
        if (_config.playBootAnim) {
            if (!_run_boot_anim()) {
                return false;
            }
        }
        
        /* If dont use launcher */
        if (!_config.useLauncher) {
            _inited = true;
            return true;
        }

        /* If launcher is set */
        if (_config.launcher != nullptr) {
            _using_builtin_launcher = false;
        }
        else {
            _config.launcher = new BUILTIN_APP::Launcher;
            if (_config.launcher == nullptr) {
                return false;
            }
        }
        
        /* Install launcher, and pass framework's pointer as userdata */
        APP_Register::install(_config.launcher, _config.database, (void*)this);
        /* Start launcher */
        startApp(_config.launcher);

        _inited = true;
        return true;
    }


    int Framework::install(APP_BASE* app, void* userData)
    {
        if (!_inited) {
            return -1;
        }
        return APP_Register::install(app, _config.database, userData);
    }


    void Framework::update()
    {
        /* Update App manager */
        APP_Manger::update();
    }
    

}
