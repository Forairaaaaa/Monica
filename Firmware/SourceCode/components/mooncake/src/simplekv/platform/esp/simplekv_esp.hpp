/**
 * @file simplekv_esp.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include "../../simplekv.h"
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>


namespace SIMPLEKV {


    class SimpleKV_ESP : public SimpleKV {

        private:
            bool _use_psram;
            int _ret_buffer;
            SemaphoreHandle_t _semaphore_mutex;
            

            /* malloc overide */
            inline void* _malloc(size_t size)
            {
                if (_use_psram) {
                    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM);
                }
                return malloc(size);
            }


        public:
            SimpleKV_ESP(bool usePSRAM = false)
            {
                _use_psram = usePSRAM;
                _semaphore_mutex = xSemaphoreCreateMutex();
            }
            ~SimpleKV_ESP() {}

            inline bool UsingPSRAM() { return _use_psram; }


            /* Take mutex, block other accesser */
            inline void StartAccess(TickType_t xTicksToWait = portMAX_DELAY)
            {
                xSemaphoreTake(_semaphore_mutex, xTicksToWait);
            }

            /* Release mutex */
            inline void EndAccess()
            {
                xSemaphoreGive(_semaphore_mutex);
            }

            
            /* Overide with mutex */
            inline int Add(const char* key, void* value, size_t size, TickType_t xTicksToWait = portMAX_DELAY)
            {
                StartAccess(xTicksToWait);
                _ret_buffer = SimpleKV::Add(key, value, size);
                EndAccess();
                return _ret_buffer;
            }

            inline int Put(const char* key, void* value, size_t size, TickType_t xTicksToWait = portMAX_DELAY)
            {
                StartAccess(xTicksToWait);
                _ret_buffer = SimpleKV::Put(key, value, size);
                EndAccess();
                return _ret_buffer;
            }

            /* Wrap of type */
            template<typename T>
            inline int Add(const char* key, T value) { return Add(key, (void*)&value, sizeof(T)); }
            template<typename T>
            inline int Put(const char* key, T value) { return Put(key, (void*)&value, sizeof(T)); }

    };

}
