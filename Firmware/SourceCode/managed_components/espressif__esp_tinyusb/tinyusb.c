/*
 * SPDX-FileCopyrightText: 2020-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "sdkconfig.h"
#include "esp_log.h"
#include "esp_check.h"
#include "esp_err.h"
#include "esp_private/periph_ctrl.h"
#include "esp_private/usb_phy.h"
#include "soc/usb_pins.h"
#include "tinyusb.h"
#include "descriptors_control.h"
#include "usb_descriptors.h"
#include "tusb.h"
#include "tusb_tasks.h"

const static char *TAG = "TinyUSB";
static usb_phy_handle_t phy_hdl;

esp_err_t tinyusb_driver_install(const tinyusb_config_t *config)
{
    const tusb_desc_device_t *dev_descriptor;
    const char **string_descriptor;
    int string_descriptor_count = 0;
    const uint8_t *cfg_descriptor;
    ESP_RETURN_ON_FALSE(config, ESP_ERR_INVALID_ARG, TAG, "Config can't be NULL");

    // Configure USB PHY
    usb_phy_config_t phy_conf = {
        .controller = USB_PHY_CTRL_OTG,
        .otg_mode = USB_OTG_MODE_DEVICE,
    };

    // External PHY IOs config
    usb_phy_ext_io_conf_t ext_io_conf = {
        .vp_io_num = USBPHY_VP_NUM,
        .vm_io_num = USBPHY_VM_NUM,
        .rcv_io_num = USBPHY_RCV_NUM,
        .oen_io_num = USBPHY_OEN_NUM,
        .vpo_io_num = USBPHY_VPO_NUM,
        .vmo_io_num = USBPHY_VMO_NUM,
    };
    if (config->external_phy) {
        phy_conf.target = USB_PHY_TARGET_EXT;
        phy_conf.ext_io_conf = &ext_io_conf;
    } else {
        phy_conf.target = USB_PHY_TARGET_INT;
    }

    // OTG IOs config
    const usb_phy_otg_io_conf_t otg_io_conf = USB_PHY_SELF_POWERED_DEVICE(config->vbus_monitor_io);
    if (config->self_powered) {
        phy_conf.otg_io_conf = &otg_io_conf;
    }
    ESP_RETURN_ON_ERROR(usb_new_phy(&phy_conf, &phy_hdl), TAG, "Install USB PHY failed");

    if (config->configuration_descriptor) {
        cfg_descriptor = config->configuration_descriptor;
    } else {
        // Default configuration descriptor is provided only for CDC and MSC classes
#if (CFG_TUD_HID > 0 || CFG_TUD_MIDI > 0 || CFG_TUD_CUSTOM_CLASS > 0 || CFG_TUD_ECM_RNDIS > 0 || CFG_TUD_NCM > 0 || CFG_TUD_DFU > 0 || CFG_TUD_DFU_RUNTIME > 0 || CFG_TUD_BTH > 0)
        ESP_RETURN_ON_FALSE(config->configuration_descriptor, ESP_ERR_INVALID_ARG, TAG, "Configuration descriptor must be provided for this device");
#else
        cfg_descriptor = descriptor_cfg_kconfig;
        ESP_LOGW(TAG, "The device's configuration descriptor is not provided by user, using default.");
#endif
    }

    if (config->string_descriptor) {
        string_descriptor = config->string_descriptor;
        if (config->string_descriptor_count != 0) {
            string_descriptor_count = config->string_descriptor_count;
        } else {
            string_descriptor_count = 8; // Backward compatibility with esp_tinyusb v1.0.0. Do NOT remove!
        }
    } else {
        string_descriptor = descriptor_str_kconfig;
        while (descriptor_str_kconfig[++string_descriptor_count] != NULL);
        ESP_LOGW(TAG, "The device's string descriptor is not provided by user, using default.");
    }

    if (config->device_descriptor) {
        dev_descriptor = config->device_descriptor;
    } else {
        dev_descriptor = &descriptor_dev_kconfig;
        ESP_LOGW(TAG, "The device's device descriptor is not provided by user, using default.");
    }

    tinyusb_set_descriptor(dev_descriptor, string_descriptor, string_descriptor_count, cfg_descriptor);

    ESP_RETURN_ON_FALSE(tusb_init(), ESP_FAIL, TAG, "Init TinyUSB stack failed");
#if !CONFIG_TINYUSB_NO_DEFAULT_TASK
    ESP_RETURN_ON_ERROR(tusb_run_task(), TAG, "Run TinyUSB task failed");
#endif
    ESP_LOGI(TAG, "TinyUSB Driver installed");
    return ESP_OK;
}
