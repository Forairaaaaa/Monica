/**
 * @file simplekv.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-04
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <iostream>
#include <string>
#include <cstring>
#include <vector>


namespace SIMPLEKV {


    struct ValueInfo_t {
        std::string key = "";
        size_t size = 0;
        void* addr = nullptr;


        /* Value convert */
        template<typename T>
        T value() {
            if ((this->addr == nullptr) || (this->size < sizeof(T))) {
                return T();
            }
            return *(T*)this->addr;
        }

    };
    

    class SimpleKV {
        private:
            std::vector<ValueInfo_t> _value_list;
            ValueInfo_t _ret_buffer;

        protected:
            /* Memory API */
            virtual void _free(void* ptr) { free(ptr); }
            virtual void* _malloc(size_t size) { return malloc(size); }
            virtual void* _memcpy(void* __restrict__ dest, const void* __restrict__ src, size_t n) { return memcpy(dest, src, n); }


        public:
            SimpleKV() {}
            ~SimpleKV() { DeleteAll(); }


            inline size_t Size() { return _value_list.size(); }
            std::string GetKey(uint32_t item);
            bool Exist(const char* key);
            size_t MemoryUsage();


            /* Basic data operation */
            int Add(const char* key, void* value, size_t size);
            int Put(const char* key, void* value, size_t size);
            ValueInfo_t* Get(const char* key);
            int Delete(const char* key);
            void DeleteAll();

            
            /* Wrap of type */
            template<typename T>
            inline int Add(const char* key, T value) { return Add(key, (void*)&value, sizeof(T)); }
            template<typename T>
            inline int Put(const char* key, T value) { return Put(key, (void*)&value, sizeof(T)); }
    };


}

