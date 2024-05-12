/**
 * @file app_register.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-07
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "app.h"


namespace MOONCAKE {


    struct APPList_t {
        APP_BASE* app = nullptr;
        int id = -1;
    };


    class APP_Register {
        private:
            int _id;
            

        protected:
            std::vector<APPList_t> _app_list;


        public:
            APP_Register() : _id(0) {}
            ~APP_Register() = default;


            /**
             * @brief Install App into register 
             * 
             * @param app App pointer 
             * @param database System database
             * @param userData Custom user data
             * @return int App ID
             */
            int install(APP_BASE* app, SIMPLEKV::SimpleKV* database, void* userData = nullptr);
            bool uninstall(APP_BASE* app);
            inline bool uninstall(int id) { return uninstall(getApp(id)); }
            inline bool uninstall(const char* name) { return uninstall(getApp(name)); }


            /* Basic API */
            inline std::vector<APPList_t> getAppList() { return _app_list; }
            inline uint16_t getAppNum() { return (uint16_t)_app_list.size(); }
            int getAppID(APP_BASE* app);
            int getAppID(const char* name);
            APP_BASE* getApp(int id);
            APP_BASE* getApp(const char* name);

    };


}

