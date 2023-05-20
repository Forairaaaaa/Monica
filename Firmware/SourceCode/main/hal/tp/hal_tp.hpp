/**
 * @file hal_tp.hpp
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once
#include <driver/i2c.h>
#include <esp_log.h>
#include <cstring>


namespace FT3168 {


    static const char* TAG = "FT3168";


    struct Config_t {
        int pin_scl     = -1;
        int pin_sda     = -1;
        int pin_rst     = -1;
        int pin_int     = -1;
        bool pull_up_en = false;

        i2c_port_t i2c_port = 0;
        uint32_t clk_speed = 100000;

        uint8_t dev_addr = 0x38;
    };


    struct TouchPoint_t {
        uint8_t event = 0;
        uint8_t id = 0;
        int x = -1;
        int y = -1;
    };


    class tp_ft3168 {
        private:
            Config_t _cfg;
            bool _inited;
            uint8_t _data_buffer[7];


            inline esp_err_t _writrReg(uint8_t reg, uint8_t data)
            {
                _data_buffer[0] = reg;
                _data_buffer[1] = data; 
                return i2c_master_write_to_device(_cfg.i2c_port, _cfg.dev_addr, _data_buffer, 2, portMAX_DELAY);
            }


            inline esp_err_t _readReg(uint8_t reg, uint8_t readSize)
            {
                /* Store data into buffer */
                return i2c_master_write_read_device(_cfg.i2c_port, _cfg.dev_addr, &reg, 1, _data_buffer, readSize, portMAX_DELAY);
            }


            inline void _initSetup()
            {
                /* 关闭监视器模式 */
                _writrReg(0x86, 0x00);
                /* 触摸阈值 */
                _writrReg(0x80, 0x7D);
            }


        public:
            tp_ft3168(): _inited(false) {}
            ~tp_ft3168() {}

            
            /* Config */
            inline Config_t config() { return _cfg; }
            inline void config(const Config_t& cfg) { _cfg = cfg; }
            inline void setPin(const int& sda, const int& scl, const int& rst, const int& intr)
            {
                _cfg.pin_sda = sda;
                _cfg.pin_scl = scl;
                _cfg.pin_rst = rst;
                _cfg.pin_int = intr;
            }


            inline bool init(const int& sda, const int& scl, const int& rst = -1, const int& intr = -1, const bool& initI2c = true, const uint32_t& speed = 100000)
            {
                _cfg.clk_speed = speed;
                setPin(sda, scl, rst, intr);
                return init(initI2c);
            }


            inline bool init(bool initI2c)
            {
                if (_inited) {
                    return true;
                }
                
                if (initI2c) {
                    if (!i2cInit()) {
                        return false;
                    }
                }

                gpioInit();

                _initSetup();

                ESP_LOGD(TAG, "Init success");
                _inited = true;
                return true;
            }


            inline bool i2cInit()
            {
                ESP_LOGD(TAG, "init i2c");

                esp_err_t ret;

                /* I2C config */
                i2c_config_t conf;
                memset(&conf, 0, sizeof(i2c_config_t));
                conf.mode = I2C_MODE_MASTER;
                conf.sda_io_num = _cfg.pin_sda;
                conf.scl_io_num = _cfg.pin_scl;
                conf.master.clk_speed = _cfg.clk_speed;
                if (_cfg.pull_up_en) {
                    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
                    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
                }
                else {
                    conf.sda_pullup_en = GPIO_PULLUP_DISABLE;
                    conf.scl_pullup_en = GPIO_PULLUP_DISABLE;
                }
                conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;
                ret = i2c_param_config(_cfg.i2c_port, &conf);
                if (ret != ESP_OK) {
                    ESP_LOGD(TAG, "I2C config failed");
                    return false;
                }

                /* Install driver */
                ret = i2c_driver_install(_cfg.i2c_port, I2C_MODE_MASTER, 0, 0, 0);
                if (ret != ESP_OK) {
                    ESP_LOGD(TAG, "I2C driver install failed");
                    return false;
                }
                return true;
            }


            /* Setup gpio and reset */
            inline void gpioInit()
            {
                ESP_LOGD(TAG, "setup gpio");

                if (_cfg.pin_int > 0) {
                    gpio_reset_pin((gpio_num_t)_cfg.pin_int);
                    gpio_set_direction((gpio_num_t)_cfg.pin_int, GPIO_MODE_INPUT);
                }

                if (_cfg.pin_rst > 0) {
                    gpio_reset_pin((gpio_num_t)_cfg.pin_rst);
                    gpio_set_direction((gpio_num_t)_cfg.pin_rst, GPIO_MODE_OUTPUT);
                    gpio_set_pull_mode((gpio_num_t)_cfg.pin_rst, GPIO_PULLUP_ONLY);
                    
                    /* Reset */
                    gpio_set_level((gpio_num_t)_cfg.pin_rst, 0);
                    vTaskDelay(pdMS_TO_TICKS(10));
                    gpio_set_level((gpio_num_t)_cfg.pin_rst, 1);
                    vTaskDelay(pdMS_TO_TICKS(10));
                }
            }


            inline void deInit(bool deInitI2c = false)
            {
                _inited = false;
                if (deInitI2c) {
                    i2c_driver_delete(_cfg.i2c_port);
                }
            }


            inline bool isTouched()
            {
                if (_cfg.pin_int > 0) {
                    return (gpio_get_level((gpio_num_t)_cfg.pin_int) == 0) ? true : false;
                }
                _readReg(0x00, 7);
                return (_data_buffer[2] != 0x00) ? true : false;
            }


            inline void getTouchRaw(TouchPoint_t& tp)
            {
                tp.x = -1;
                tp.y = -1;

                if (_cfg.pin_int > 0) {
                    if (gpio_get_level((gpio_num_t)_cfg.pin_int) != 0) {
                        return;
                    }
                }

                /* Start reading from 0x00 */
                _readReg(0x00, 7);
                if (_data_buffer[2] != 0x00) {
                    tp.event = (_data_buffer[3] >> 6) & 0x03;
                    tp.x     = ((_data_buffer[3] & 0x0F) << 8) | _data_buffer[4];
                    tp.id    = (_data_buffer[5] >> 4) & 0x0F;
                    tp.y     = ((_data_buffer[5] & 0x0F) << 8) | _data_buffer[6];
                }
            }

    };


}

