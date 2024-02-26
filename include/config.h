/*************************************************************************
 * Config.h 
 * 
 * @author  Luke 
 * @date    11/12/2023
 * 
 * @details Configuration for ESP32 - Nightfury Project.
 * 
 ************************************************************************/

#ifndef CONFIG_H_
#define CONFIG_H_

#include <stdbool.h>

//=============================================================================
// LED Controller
//-----------------------------------------------------------------------------

// Onboard LED - ARGB
#define CONFIG__LED_ONBOARD_RGB     true

#ifdef CONFIG__LED_ONBOARD_RGB
    #define LED_ONBOARD_RGB_PIN GPIO_NUM_8
#endif // CONFIG__LED_ONBOARD_RGB

//=============================================================================
// WIFI settings
//-----------------------------------------------------------------------------

#define CONFIG__WIFI_SSID       "WIFI_SID"
#define CONFIG__WIFI_PASSWORD   "WIFI_PASSWORD"

#endif // CONFIG_H_