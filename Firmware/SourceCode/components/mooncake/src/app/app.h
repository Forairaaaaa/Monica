/**
 * @file app.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "../simplekv/simplekv.h"


namespace MOONCAKE {


    /* Inherit and override "Life cycle" to create your own App */
    class APP_BASE {
        private:
            std::string _name;
            void* _user_data;
            void* _icon_addr;
            bool _allow_bg_running;
            bool _go_close;
            bool _go_destroy;
            SIMPLEKV::SimpleKV* _database;


        protected:
            /* App internal API */

            /**
             * @brief Set the App Name
             * 
             * @param name 
             */
            inline void setAppName(const std::string& name) { _name = name; }

            /**
             * @brief Set the App Icon
             * 
             * @param icon 
             */
            inline void setAppIcon(void* icon) { _icon_addr = icon; }

            /**
             * @brief Set if is App running background after closed
             * 
             * @param allow 
             */
            inline void setAllowBgRunning(bool allow) { _allow_bg_running = allow; }

            /**
             * @brief Close App 
             * 
             */
            inline void closeApp() { _go_close = true; }

            /**
             * @brief Destroy App, not going background
             * 
             */
            inline void destroyApp() { _go_destroy = true; }

            /**
             * @brief Get database
             * 
             * @return SIMPLEKV::SimpleKV* 
             */
            inline SIMPLEKV::SimpleKV* getDatabase() { return _database; }

            /**
             * @brief Get user data
             * 
             * @return void* 
             */
            inline void* getUserData() { return _user_data; };


        public:
            APP_BASE() :
                _name(""),
                _user_data(nullptr),
                _icon_addr(nullptr),
                _allow_bg_running(false),
                _go_close(false),
                _go_destroy(false),
                _database(nullptr) {}
            virtual ~APP_BASE() {}


            /* API for App manager */
            inline void setDatabase(SIMPLEKV::SimpleKV* db) { _database = db; }
            inline void setUserData(void* userData) { _user_data = userData; }

            
            /* Basic API */
            inline std::string getAppName() { return _name; }
            inline void* getAppIcon() { return _icon_addr; }
            inline bool isAllowBgRunning() { return _allow_bg_running; }
            inline bool isGoingClose() { return _go_close; }
            inline bool isGoingDestroy() { return _go_destroy; }
            inline void resetGoingCloseFlag() { _go_close = false; }
            inline void resetGoingDestroyFlag() { _go_destroy = false; }


            /**
             * @brief Lifecycle callbacks for derived to override
             * 
             */
            /* Setup App configs, called when App "install()" */
            virtual void onSetup() {}

            /* Life cycle */
            virtual void onCreate() {}
            virtual void onResume() {}
            virtual void onRunning() {}
            virtual void onRunningBG() {}
            virtual void onPause() {}
            virtual void onDestroy() {}

    };


}
