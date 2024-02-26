/*************************************************************************
 * ledController.c
 * 
 * @author  Luke 
 * @date    12/12/2023
 * 
 * @details Led controller is a module for controlling LEDs 
 * 
 * @note    Currently supports. 
 *              - Single color LED 
 * 
 ************************************************************************/

//=============================================================================
// Interfaces
//-----------------------------------------------------------------------------
#include "ledController.h"

// Posix modules 
#include <string.h>

// Espressif Modules 
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/freeRTOS.h"
#include "freertos/task.h"

// Project Modules 
#include "config.h"

//=============================================================================
// Private Types
//-----------------------------------------------------------------------------
static const char *TAG = "led_controller";

#define DEV_BOARD_LED_PIN   LED_ONBOARD_PIN

//=============================================================================
// Private functions
//-----------------------------------------------------------------------------


/*******************************************************************************
 * ledController_init
 * 
 * @brief Initialization code for the new component. 
 * 
 * @returns none
 ******************************************************************************/
void LedController_init(void)
{
    ESP_LOGI(TAG,"Led controller module Init");

    #ifdef CONFIG__LED_ONBOARD_RGB
        // No implementation for RGB LED
    #endif
}

/*******************************************************************************
 * ledController_task
 * 
 * @brief Executes LED tasks.
 * 
 * @returns none
 ******************************************************************************/
void LedController_task(void *pvParameter)
{
    while (true)
    {
        #ifdef CONFIG__LED_ONBOARD_RGB
            // No implementation for RGB LED
        #endif

        // TODO: Better implementation for this is to not use 
        // vTaskDelay for LED toggling - We should setup system timers to do 
        // This instead. look at freerots/timers.h 
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}