/*************************************************************************
 * Wifi_controller.h 
 * 
 * @author  Luke 
 * @date    11/12/2023
 * 
 * @details Interface for wifiController.c
 * 
 ************************************************************************/

#ifndef WIFI_CONTROLLER_H_
#define WIFI_CONTROLLER_H_

void WifiController_init(void);
void WifiController_task(void *pvParameter);

#endif // WIFI_CONTROLLER_H_