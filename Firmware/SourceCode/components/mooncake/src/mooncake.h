/**
 * @file mooncake.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-06
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "system_data_def.h"
#include "framework/framework.h"
#include "builtin_apps/builtin_apps.h"


namespace MOONCAKE {


    class Mooncake : public Framework {
        private:

        public:
            Mooncake() = default;
            ~Mooncake() = default;


            /**
             * @brief Setup data for general usage in database
             * 
             */
            virtual void setupDatabase();


            /**
             * @brief Install builtin Apps,
             * override this to setup your builtin list
             * 
             */
            virtual void installBuiltinApps();

    };


}

