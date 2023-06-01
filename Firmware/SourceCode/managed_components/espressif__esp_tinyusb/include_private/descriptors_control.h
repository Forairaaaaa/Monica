/*
 * SPDX-FileCopyrightText: 2020-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include "tusb.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Set descriptors for this driver
 *
 * @attention All descriptors passed to this function must exist for the duration of USB device lifetime
 *
 * @param[in] dev_desc Device descriptor
 * @param[in] str_desc Pointer to array of UTF-8 strings
 * @param[in] str_desc_count Number of descriptors in str_desc
 * @param[in] cfg_desc Configuration descriptor
 */
void tinyusb_set_descriptor(const tusb_desc_device_t *dev_desc, const char **str_desc, int str_desc_count, const uint8_t *cfg_desc);

/**
 * @brief Set specific string descriptor
 *
 * @attention The descriptor passed to this function must exist for the duration of USB device lifetime
 *
 * @param[in] str     UTF-8 string
 * @param[in] str_idx String descriptor index
 */
void tinyusb_set_str_descriptor(const char *str, int str_idx);

#ifdef __cplusplus
}
#endif
