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
static void wifiController_start_wifi_station(void);
static void wifi_connection_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

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
    wifiController_start_wifi_station();
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
    vTaskDelay(2000 / portTICK_PERIOD_MS);
    wifiController_scan_ap();

    while(true)
    {
        ESP_LOGI(TAG,"Wifi Task Alive...");
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}

/*******************************************************************************
 * wifi_connection_event_handler
 * 
 * @brief 
 * 
 * @param 
 * 
 * @returns none
 ******************************************************************************/
static void wifi_connection_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) 
    {
        esp_wifi_connect();
        ESP_LOGI(TAG,"WiFi Started, Attempting to connect to SSID: %s", CONFIG__WIFI_SSID);
    } 
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_CONNECTED) 
    {
        ESP_LOGI(TAG,"WiFi Connected to SSID: %s", CONFIG__WIFI_SSID);
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) 
    {
        esp_wifi_connect();
        ESP_LOGI(TAG,"Failed to connect to SSID: %s", CONFIG__WIFI_SSID);
    } 
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) 
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    }
}

/*******************************************************************************
 * wifiController_start_wifi_station
 * 
 * @brief Configures the Wifi Controller in Station mode - Station mode allows
 * the ESP32 to peform AP scans, connect to wifi Access points (SSID, PASS), and
 * transmit data over the access point.
 * 
 * @returns none
 ******************************************************************************/
static void wifiController_start_wifi_station(void)
{
    // Initialize network stack 
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_t *sta_netif = esp_netif_create_default_wifi_sta();
    assert(sta_netif);

    // Use default wifi config settings.
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    
    // Register Wifi Events
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_connection_event_handler,
                                                        NULL,
                                                        &instance_any_id));

    // Register Events for IP
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_connection_event_handler,
                                                        NULL,
                                                        &instance_got_ip));

    // Set wifi configuration 
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = CONFIG__WIFI_SSID,
            .password = CONFIG__WIFI_PASSWORD,
        },
    };

    // Start WiFi
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start());

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
    // Create WiFi Record list
    uint16_t channelsToScan = DEFAULT_SCAN_LIST_SIZE;
    wifi_ap_record_t ap_info[DEFAULT_SCAN_LIST_SIZE];
    uint16_t ap_count = 0;
    memset(ap_info, 0, sizeof(ap_info));

    // Start WiFi scan.
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