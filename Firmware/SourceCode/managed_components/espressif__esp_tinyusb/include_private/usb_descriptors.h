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
 * @brief Device descriptor generated from Kconfig
 *
 * This descriptor is used by default.
 * The user can provide his own device descriptor during tinyusb_driver_install() call
 */
extern const tusb_desc_device_t descriptor_dev_kconfig;

/**
 * @brief Array of string descriptors generated from Kconfig
 *
 * This descriptor is used by default.
 * The user can provide his own descriptor during tinyusb_driver_install() call
 */
extern const char *descriptor_str_kconfig[];

/**
 * @brief Configuration descriptor generated from Kconfig
 *
 * This descriptor is used by default.
 * The user can provide his own configuration descriptor during tinyusb_driver_install() call
 */
extern const uint8_t descriptor_cfg_kconfig[];

#ifdef __cplusplus
}
#endif
