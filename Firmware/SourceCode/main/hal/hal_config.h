/**
 * @file hal_config.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#pragma once


/* I2C peripheral */
#define HAL_PIN_I2C_PORT    0
#define HAL_PIN_I2C_SCL     10
#define HAL_PIN_I2C_SDA     11

/* Touch pad */
#define HAL_PIN_TP_RST      3
#define HAL_PIN_TP_INTR     12

/* PMU AXP2101 */
#define HAL_PIN_AXP_INTR    14

/* RTC PCF8563 */
#define HAL_PIN_RTC_INTR    41

/* IMU BMI270 */
#define HAL_PIN_IMU_INTR1   38
#define HAL_PIN_IMU_INTR2   40

/* Buzzer */
#define HAL_PIN_BUZZER      45

/* Mic */
#define HAL_PIN_MIC         42
