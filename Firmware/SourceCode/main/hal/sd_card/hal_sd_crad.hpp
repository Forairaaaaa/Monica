/**
 * @file hal_sd_crad.hpp
 * @author Forairaaaaa
 * @brief Init SD card and file system for lvgl's file r/w
 * @version 0.1
 * @date 2023-05-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */
/* https://github.com/espressif/esp-idf/blob/master/examples/storage/sd_card/sdmmc/main/sd_card_example_main.c */
#pragma once
#include "../hal_config.h"
#include <iostream>
#include <string>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <esp_vfs_fat.h>
#include <sdmmc_cmd.h>
#include <driver/sdmmc_host.h>


namespace SD_CARD {


    static const char* TAG = "SD";


    struct Config_t {
        int CD = HAL_PIN_SD_CD;
        int CLK = HAL_PIN_SD_CLK;
        int CMD = HAL_PIN_SD_CMD;
        int D0 = HAL_PIN_SD_D0;
        int D1 = HAL_PIN_SD_D1;
        int D2 = HAL_PIN_SD_D2;
        int D3 = HAL_PIN_SD_D3;

        std::string mountPoint = "/sdcard";
        bool formatIfMountFailed = true;
    };


    class SD_Card {
        private:
            Config_t _config;
            bool _available;


        public:
            SD_Card() : _available(false) {}
            ~SD_Card() = default;


            inline void config(Config_t cfg) { _config = cfg; }
            inline Config_t config(void) { return _config; }


            inline bool isAvailable() { return _available; }



            inline void init()
            {
                esp_err_t ret;

                // Options for mounting the filesystem.
                // If format_if_mount_failed is set to true, SD card will be partitioned and
                // formatted in case when mounting fails.
                esp_vfs_fat_sdmmc_mount_config_t mount_config;
                mount_config.format_if_mount_failed = _config.formatIfMountFailed;
                mount_config.max_files = 5;
                mount_config.allocation_unit_size = 16 * 1024;
                mount_config.disk_status_check_enable = false;
                sdmmc_card_t *card;
                ESP_LOGI(TAG, "Initializing SD card");

                // Use settings defined above to initialize SD card and mount FAT filesystem.
                // Note: esp_vfs_fat_sdmmc/sdspi_mount is all-in-one convenience functions.
                // Please check its source code and implement error recovery when developing
                // production applications.

                ESP_LOGI(TAG, "Using SDMMC peripheral");

                // By default, SD card frequency is initialized to SDMMC_FREQ_DEFAULT (20MHz)
                // For setting a specific frequency, use host.max_freq_khz (range 400kHz - 40MHz for SDMMC)
                // Example: for fixed frequency of 10MHz, use host.max_freq_khz = 10000;
                sdmmc_host_t host = SDMMC_HOST_DEFAULT();

                // This initializes the slot without card detect (CD) and write protect (WP) signals.
                // Modify slot_config.gpio_cd and slot_config.gpio_wp if your board has these signals.
                sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();

                // Set bus width to use:
                slot_config.width = 4;

                // On chips where the GPIOs used for SD card can be configured, set them in
                // the slot_config structure:
                slot_config.clk = (gpio_num_t)_config.CLK;
                slot_config.cmd = (gpio_num_t)_config.CMD;
                slot_config.d0 = (gpio_num_t)_config.D0;
                slot_config.d1 = (gpio_num_t)_config.D1;
                slot_config.d2 = (gpio_num_t)_config.D2;
                slot_config.d3 = (gpio_num_t)_config.D3;
                // slot_config.cd = (gpio_num_t)_config.CD;

                ESP_LOGI(TAG, "Mounting filesystem");
                ret = esp_vfs_fat_sdmmc_mount(_config.mountPoint.c_str(), &host, &slot_config, &mount_config, &card);

                if (ret != ESP_OK) {
                    if (ret == ESP_FAIL) {
                        ESP_LOGE(TAG, "Failed to mount filesystem. "
                                "If you want the card to be formatted, set the EXAMPLE_FORMAT_IF_MOUNT_FAILED menuconfig option.");
                    } else {
                        ESP_LOGE(TAG, "Failed to initialize the card (%s). "
                                "Make sure SD card lines have pull-up resistors in place.", esp_err_to_name(ret));
                    }
                    return;
                }
                ESP_LOGI(TAG, "Filesystem mounted");

                // Card has been initialized, print its properties
                sdmmc_card_print_info(stdout, card);


                _available = true;


                // /* Test */
                // std::string path = "/sdcard/nihao.txt";

                // ESP_LOGI(TAG, "Opening file %s", path.c_str());
                // FILE *f = fopen(path.c_str(), "w");
                // if (f == NULL) {
                //     ESP_LOGE(TAG, "Failed to open file for writing");
                    
                // }
                // fprintf(f, "nihao ????????????");
                // fclose(f);
                // ESP_LOGI(TAG, "File written");
            }

    };


}

