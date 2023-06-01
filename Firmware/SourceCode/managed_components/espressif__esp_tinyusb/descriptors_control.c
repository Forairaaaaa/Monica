/*
 * SPDX-FileCopyrightText: 2020-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <string.h>
#include "esp_log.h"
#include "descriptors_control.h"

#define USB_STRING_DESCRIPTOR_ARRAY_SIZE 8 // Max 8 string descriptors for a device. LANGID, Manufacturer, Product, Serial number + 4 user defined
#define MAX_DESC_BUF_SIZE 32               // Max length of string descriptor (can be extended, USB supports lengths up to 255 bytes)

static const char *TAG = "tusb_desc";
static const tusb_desc_device_t *s_device_descriptor;
static const uint8_t *s_configuration_descriptor;
static const char *s_str_descriptor[USB_STRING_DESCRIPTOR_ARRAY_SIZE]; // Array of pointers (strings). Statically allocated, can be made dynamic in the future.

// =============================================================================
// CALLBACKS
// =============================================================================

/**
 * @brief Invoked when received GET DEVICE DESCRIPTOR.
 * Descriptor contents must exist long enough for transfer to complete
 *
 * @return Pointer to device descriptor
 */
uint8_t const *tud_descriptor_device_cb(void)
{
    return (uint8_t const *)s_device_descriptor;
}

/**
 * @brief Invoked when received GET CONFIGURATION DESCRIPTOR.
 * Descriptor contents must exist long enough for transfer to complete
 *
 * @param[in] index Index of required configuration
 * @return Pointer to configuration descriptor
 */
uint8_t const *tud_descriptor_configuration_cb(uint8_t index)
{
    (void)index; // Unused, this driver supports only 1 configuration
    return s_configuration_descriptor;
}

/**
 * @brief Invoked when received GET STRING DESCRIPTOR request
 *
 * @param[in] index   Index of required descriptor
 * @param[in] langid  Language of the descriptor
 * @return Pointer to UTF-16 string descriptor
 */
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    (void) langid; // Unused, this driver supports only one language in string descriptors
    uint8_t chr_count;
    static uint16_t _desc_str[MAX_DESC_BUF_SIZE];

    if (index == 0) {
        memcpy(&_desc_str[1], s_str_descriptor[0], 2);
        chr_count = 1;
    } else {
        if (index >= USB_STRING_DESCRIPTOR_ARRAY_SIZE) {
            ESP_LOGW(TAG, "String index (%u) is out of bounds, check your string descriptor", index);
            return NULL;
        }

        if (s_str_descriptor[index] == NULL) {
            ESP_LOGW(TAG, "String index (%u) points to NULL, check your string descriptor", index);
            return NULL;
        }

        const char *str = s_str_descriptor[index];
        chr_count = strnlen(str, MAX_DESC_BUF_SIZE - 1); // Buffer len - header

        // Convert ASCII string into UTF-16
        for (uint8_t i = 0; i < chr_count; i++) {
            _desc_str[1 + i] = str[i];
        }
    }

    // First byte is length in bytes (including header), second byte is descriptor type (TUSB_DESC_STRING)
    _desc_str[0] = (TUSB_DESC_STRING << 8 ) | (2 * chr_count + 2);

    return _desc_str;
}

// =============================================================================
// Driver functions
// =============================================================================

void tinyusb_set_descriptor(const tusb_desc_device_t *dev_desc, const char **str_desc, int str_desc_count, const uint8_t *cfg_desc)
{
    assert(dev_desc && str_desc && cfg_desc);
    assert(str_desc_count <= USB_STRING_DESCRIPTOR_ARRAY_SIZE);

    ESP_LOGI(TAG, "\n"
             "┌─────────────────────────────────┐\n"
             "│  USB Device Descriptor Summary  │\n"
             "├───────────────────┬─────────────┤\n"
             "│bDeviceClass       │ %-4u        │\n"
             "├───────────────────┼─────────────┤\n"
             "│bDeviceSubClass    │ %-4u        │\n"
             "├───────────────────┼─────────────┤\n"
             "│bDeviceProtocol    │ %-4u        │\n"
             "├───────────────────┼─────────────┤\n"
             "│bMaxPacketSize0    │ %-4u        │\n"
             "├───────────────────┼─────────────┤\n"
             "│idVendor           │ %-#10x  │\n"
             "├───────────────────┼─────────────┤\n"
             "│idProduct          │ %-#10x  │\n"
             "├───────────────────┼─────────────┤\n"
             "│bcdDevice          │ %-#10x  │\n"
             "├───────────────────┼─────────────┤\n"
             "│iManufacturer      │ %-#10x  │\n"
             "├───────────────────┼─────────────┤\n"
             "│iProduct           │ %-#10x  │\n"
             "├───────────────────┼─────────────┤\n"
             "│iSerialNumber      │ %-#10x  │\n"
             "├───────────────────┼─────────────┤\n"
             "│bNumConfigurations │ %-#10x  │\n"
             "└───────────────────┴─────────────┘",
             dev_desc->bDeviceClass, dev_desc->bDeviceSubClass,
             dev_desc->bDeviceProtocol, dev_desc->bMaxPacketSize0,
             dev_desc->idVendor, dev_desc->idProduct, dev_desc->bcdDevice,
             dev_desc->iManufacturer, dev_desc->iProduct, dev_desc->iSerialNumber,
             dev_desc->bNumConfigurations);

    // Save passed descriptors
    s_device_descriptor = dev_desc;
    s_configuration_descriptor = cfg_desc;
    memcpy(s_str_descriptor, str_desc, str_desc_count * sizeof(str_desc[0]));
}

void tinyusb_set_str_descriptor(const char *str, int str_idx)
{
    assert(str_idx < USB_STRING_DESCRIPTOR_ARRAY_SIZE);
    s_str_descriptor[str_idx] = str;
}
