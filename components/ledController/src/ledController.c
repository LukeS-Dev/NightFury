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

#ifdef CONFIG__LED_ONBOARD_SINGLE
    static uint8_t ledState = 0;
#endif

//=============================================================================
// Private functions
//-----------------------------------------------------------------------------
#ifdef CONFIG__LED_ONBOARD_SINGLE
    static void led_single_init();
    static void led_single_task();
#endif // LED_ONBOARD_SINGLE 

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
    
    #ifdef CONFIG__LED_ONBOARD_SINGLE
        led_single_init();
    #endif

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
        #ifdef CONFIG__LED_ONBOARD_SINGLE
            led_single_task();
        #endif

        #ifdef CONFIG__LED_ONBOARD_RGB
            // No implementation for RGB LED
        #endif
    }
}

#ifdef CONFIG__LED_ONBOARD_SINGLE
/*******************************************************************************
 * led_single_task
 * 
 * @brief Blinks a single colour LED 
 * 
 * @returns none
 ******************************************************************************/
static void led_single_task()
{
    // TODO: Expose this so that other tasks can "Control the LED State" 
    // This task becomes an Indication LED - controllable by other modules 
    // Rather than a static LED 

    ledState = !ledState; // Toggle LED State
    gpio_set_level(DEV_BOARD_LED_PIN, ledState);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}

/*******************************************************************************
 * led_single_init
 * 
 * @brief Initializes a single colour LED  
 * 
 * @returns none
 ******************************************************************************/
static void led_single_init()
{
    gpio_reset_pin(DEV_BOARD_LED_PIN);
    gpio_set_direction(DEV_BOARD_LED_PIN,GPIO_MODE_OUTPUT);
}

#endif //CONFIG__LED_ONBOARD_SINGLE