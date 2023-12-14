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

// Onboard LED - Single colour 
#define CONFIG__LED_ONBOARD_SINGLE  true

#ifdef  CONFIG__LED_ONBOARD_SINGLE
    #define LED_ONBOARD_PIN GPIO_NUM_2
#endif // LED_ONBOARD_SINGLE

// Onboard LED - ARGB
#define CONFIG__LED_ONBOARD_RGB     false

#ifdef CONFIG__LED_ONBOARD_RGB
    #define LED_ONBOARD_RGB_PIN GPIO_NUM_8
#endif // CONFIG__LED_ONBOARD_RGB

#endif // CONFIG_H_