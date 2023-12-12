/*************************************************************************
 * ledController.c
 * 
 * @author  Luke 
 * @date    12/12/2023
 * 
 * @details 
 * 
 ************************************************************************/

// This interface
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

// Private types
#define DEV_BOARD_LED_PIN GPIO_NUM_2
static uint8_t ledState = 0;

static const char *TAG = "led_controller";

// Private functions

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
    gpio_reset_pin(DEV_BOARD_LED_PIN);
    gpio_set_direction(DEV_BOARD_LED_PIN,GPIO_MODE_OUTPUT);
}


/*******************************************************************************
 * ledController_task
 * 
 * @brief Blinks Onboard LED
 * 
 * @returns none
 ******************************************************************************/
void LedController_task(void *pvParameter)
{
    while (true)
    {
        ledState = !ledState; // Toggle LED State
        gpio_set_level(DEV_BOARD_LED_PIN, ledState);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}