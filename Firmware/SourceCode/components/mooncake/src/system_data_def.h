/**
 * @file system_data_def.h
 * @author Forairaaaaa
 * @brief 
 * @version 0.1
 * @date 2023-05-09
 * 
 * @copyright Copyright (c) 2023
 * 
 * Use define to unify baisc data's key in database(SimpleKV)
 * Framework will add them into database while init
 * It won't if no define
 */
#pragma once
#include <stdint.h>



/* Navigation */
#if 1
#define MC_KEY_MENU             "_KEY_MENU"     // bool
#define MC_KEY_HOME             "_KEY_HOME"     // bool
#define MC_KEY_BACK             "_KEY_BACK"     // bool
#define MC_KEY_POWER            "_KEY_PWR"      // bool
#define MC_KEY_UP               "_KEY_UP"       // bool
#define MC_KEY_DOWN             "_KEY_DOWN"     // bool
#endif


/* Time */
#if 1
#define MC_TIME                 "_TIME"         // MOONCAKE::DataTime_t
namespace MOONCAKE {
    struct DataTime_t {
        uint8_t sec = 0;		/* Seconds.	[0-60] (1 leap second) */
        uint8_t min = 32;		/* Minutes.	[0-59] */
        uint8_t hour = 23;		/* Hours.	[0-23] */
        uint8_t mday = 4;		/* Day.		[1-31] */
        uint8_t mon = 6;		/* Month.	[0-11] */
        uint8_t wday = 0;		/* Day of week.	[0-6] */
        int year = 99;			/* Year	- 1900.  */
        long int gmtoff = 0;	/* Seconds east of UTC */
        bool wasSet = false;    /* Flag to set time*/
    };
}
#define MC_TIME_JSUT_SET        "_TIME_SET"     // bool
#endif


/* Hardware */
#if 1
/* Display */
#define MC_DISP_HOR             "_DIS_HOR"      // int16_t
#define MC_DISP_VER             "_DIS_VER"      // int16_t
#define MC_DISP_BRIGHTNESS      "_DIS_BRI"      // uint8_t


/* System tick */
#define MC_SYSTEM_TICKS         "_SYS_TICK"     // uint32_t


/* Power */
#define MC_BATTERY_LEVEL        "_BAT_LV"       // uint8_t
#define MC_BATTERY_IS_CHARGING  "_BAT_CHG"      // bool


/* Wireless */
#define MC_WIFI_IS_CONNECTED    "_WIFI_CN"      // bool
#define MC_BLE_IS_CONNECTED     "_BLE_CN"       // bool


/* Notification */
#define MC_NOTIFICATION_IS_ON   "_NOTE_ON"      // bool


/* Setps */
#define MC_STEPS                "_STEPS"        // uint32_t


/* Flag of just wake up from sleep */
#define MC_JUST_WAKEUP          "_WAKE_UP"      // bool

#endif
