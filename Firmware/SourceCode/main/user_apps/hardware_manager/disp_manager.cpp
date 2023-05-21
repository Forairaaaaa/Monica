/**
 * @file disp_manager.cpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-21
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "hardware_manager.h"
#include <esp_log.h>
#include <esp_timer.h>


namespace USER_APPS {
    namespace HM {

        void Hardware_Manager::_disp_update()
        {
            if ((esp_timer_get_time() - _data.display_update_count) > _data.display_update_interval) {
                
                /* Read power key */
                if (_hal->pmu.isKeyPressed()) {

                    if (_data.is_disp_on) {
                        _hal->disp.setBrightness(0);
                    }
                    else {
                        _hal->disp.setBrightness(200);
                    }
                    

                    _hal->isSleeping(_data.is_disp_on);
                    
                    _data.is_disp_on = !_data.is_disp_on;
                    
                }

                _data.display_update_count = esp_timer_get_time();
            }
        }

    }
}
