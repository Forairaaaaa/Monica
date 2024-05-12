/**
 * @file mooncake.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-27
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "mooncake.h"


namespace MOONCAKE {


    void Mooncake::setupDatabase()
    {
        
    }


    void Mooncake::installBuiltinApps()
    {
        APP_BASE* app_ptr = nullptr;

        /* User custom watch face */
        app_ptr = new BUILTIN_APP::WF_User_Custom;
        install(app_ptr, nullptr);

        /* Default anim watch face */
        app_ptr = new BUILTIN_APP::WF_Spring_Wreath;
        install(app_ptr, nullptr);

        /* Sketchpad */
        app_ptr = new BUILTIN_APP::Sketchpad;
        install(app_ptr, nullptr);
        
        /* Settings */
        app_ptr = new BUILTIN_APP::Settings;
        install(app_ptr, nullptr);
    }


}

