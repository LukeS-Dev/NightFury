/*************************************************************************
 * Nightfury.c 
 * 
 * @author  Luke 
 * @date    11/12/2023
 * 
 * @details Entry code for ESP32
 * 
 ************************************************************************/

// POSIX defines
#include <stdio.h>
#include <stdbool.h>

// Espressif Modules 
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/freeRTOS.h"
#include "freertos/task.h"

// Components
#include "wifiController.h"

// Private methods
static void nvs_init(void);

// Private types
static const char *TAG = "nightfury_main";

/*******************************************************************************
 * app_init
 * 
 * @brief initialization code for components.
 * 
 * @returns none
 ******************************************************************************/
void app_init(void)
{
    // Wifi Controls
    WifiController_init();
}

/*******************************************************************************
 * app_main
 * 
 * @brief main entry for code.
 * 
 * @returns none
 ******************************************************************************/
void app_main(void)
{
    /* ESP-IDF components require non-volatile storage - nvs
    init must be called first before application starts.*/
    nvs_init();

    // Initialize application code
    app_init();

    // TODO : Migrate project structure to FreeRTOS based implementation.
    while(true)
    {
        // Kick watchdog every 1000 ms - Keeps application alive 
        // TODO: When freeRTOS tasking has been implemented - we need to remove this code
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/*******************************************************************************
 * nvs_init
 * 
 * @brief initializes non-volatile memory for the ESP_32. 
 * 
 * @note ESP-IDF standard modules often use the nvs_flash - this should be one of
 * the first modules to be called in initialization.
 * 
 * @returns none
 ******************************************************************************/
static void nvs_init(void)
{
    // Initialize Non-volatile flash
    esp_err_t ret = nvs_flash_init();

    // Flash erase - If NVS module is updated or if NVS section is full.
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK( ret );
}