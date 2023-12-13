/*************************************************************************
 * Wifi_controller.c
 * 
 * @author  Luke 
 * @date    11/12/2023
 * 
 * @details WifiController - WiFi controller 
 * 
 ************************************************************************/



//=============================================================================
// Interfaces
//-----------------------------------------------------------------------------
#include "wifiController.h"

// Posix modules
#include <string.h>

// Espressif Modules 
#include "esp_wifi.h"
#include "esp_log.h"
#include "freertos/freeRTOS.h"
#include "freertos/task.h"

// Project Modules
#include "config.h"

//=============================================================================
// Private Types
//-----------------------------------------------------------------------------
static const char *TAG = "wifi_controller";

#define DEFAULT_SCAN_LIST_SIZE 0x0F // Up to 16 SSIDs in scan.

//=============================================================================
// Private functions
//-----------------------------------------------------------------------------
static void wifiController_scan_ap(void);

/*******************************************************************************
 * WifiController_init
 * 
 * @brief Intializes the Wifi Controller 
 * 
 * @returns none
 ******************************************************************************/
void WifiController_init(void)
{
    ESP_LOGI(TAG,"Wifi Module Init");
    // Scan for access points.
    wifiController_scan_ap();
}

/*******************************************************************************
 * WifiController_task
 * 
 * @brief Task/Thread of the wifi controller
 * 
 * @param pvParamter - Task parameters (freeRTOS standard)
 * 
 * @returns none
 ******************************************************************************/
void WifiController_task(void *pvParameter)
{
    while(true)
    {
        ESP_LOGI(TAG,"Wifi Task Alive...");
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

/*******************************************************************************
 * wifiController_scan_ap
 * 
 * @brief function that scans the Access points 
 * 
 * @returns none
 ******************************************************************************/
static void wifiController_scan_ap(void)
{
    // Initialize network stack 
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    // Use default wifi config
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Initialize Wifi records.
    uint16_t channelsToScan = DEFAULT_SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));

    // Set wifi to station mode & start scanning.
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    esp_wifi_scan_start(NULL, true);

    // Load access point     
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&channelsToScan, ap_info));
    ESP_ERROR_CHECK(esp_wifi_scan_get_ap_num(&ap_count));

    ESP_LOGI(TAG, "Access points Scanned: %u | Max Access points : %u", ap_count, channelsToScan);
    
    // Print Scanned network SSID and RSSI 
    for (int i = 0; i < channelsToScan; i++) 
    {
        ESP_LOGI(TAG, "=============================");
        ESP_LOGI(TAG, "AP   : %d", i);
        ESP_LOGI(TAG, "SSID : %s", ap_info[i].ssid);
        ESP_LOGI(TAG, "RSSI : %d", ap_info[i].rssi);
        ESP_LOGI(TAG, "=============================");
    }
}