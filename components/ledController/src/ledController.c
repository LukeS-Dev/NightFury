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
#include "led_strip.h"

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

#define DEV_BOARD_LED_PIN   LED_ONBOARD_RGB_PIN

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

#ifdef CONFIG__LED_ONBOARD_RGB 
    // static void led_test_init();
    static void led_test_fade_init();
#endif // LED_ONBOARD_TEST

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
        //led_single_init();
    #endif

    #ifdef CONFIG__LED_ONBOARD_RGB
        // No implementation for RGB LED
    #endif
    // led_test_init();
    led_test_fade_init();
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
        #if CONFIG__LED_ONBOARD_SINGLE
            //led_single_task();
        #endif

        #ifdef CONFIG__LED_ONBOARD_RGB
            // No implementation for RGB LED
        #endif

        // TODO: Better implementation for this is to not use 
        // vTaskDelay for LED toggling - We should setup system timers to do 
        // This instead. look at freerots/timers.h 
        vTaskDelay(1000/portTICK_PERIOD_MS);
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



static void led_test_init()
{

    led_strip_handle_t led_strip;

    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = DEV_BOARD_LED_PIN, // The GPIO that connected to the LED strip's data line
        .max_leds = 1, // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812, // LED strip model
        .flags.invert_out = false, // whether to invert the output signal (useful when your hardware has a level inverter)
    };

    led_strip_rmt_config_t rmt_config = {
    #if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
        .rmt_channel = 0,
    #else
        .clk_src = RMT_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false, // whether to enable the DMA feature
    #endif
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));

    // gpio_reset_pin(DEV_BOARD_LED_PIN);
    // gpio_set_direction(DEV_BOARD_LED_PIN, GPIO_MODE_OUTPUT);
    while (true) {
        led_strip_set_pixel(
            led_strip,
            0, 
            254,
            254,
            254
        );

        vTaskDelay(1000/portTICK_PERIOD_MS);
        led_strip_refresh(led_strip);
        
        led_strip_set_pixel(
            led_strip,
            0, 
            254,
            0,
            0
        );
        vTaskDelay(1000/portTICK_PERIOD_MS);
        led_strip_refresh(led_strip);

        led_strip_set_pixel(
            led_strip,
            0, 
            0,
            254,
            0
        );
        vTaskDelay(1000/portTICK_PERIOD_MS);


        led_strip_refresh(led_strip);
    };
    // esp_err_t err = led_strip_set_pixel (
    //     led_strip,
    //     0, 
    //     254,
    //     254,
    //     254
    // );

    led_strip_refresh(led_strip);
}



static void led_test_fade_init()
{

    led_strip_handle_t led_strip;

    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = DEV_BOARD_LED_PIN, // The GPIO that connected to the LED strip's data line
        .max_leds = 1, // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812, // LED strip model
        .flags.invert_out = false, // whether to invert the output signal (useful when your hardware has a level inverter)
    };

    led_strip_rmt_config_t rmt_config = {
    #if ESP_IDF_VERSION < ESP_IDF_VERSION_VAL(5, 0, 0)
        .rmt_channel = 0,
    #else
        .clk_src = RMT_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false, // whether to enable the DMA feature
    #endif
    };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));

    int counter_red = 0;
    int counter_green = 0;
    int counter_blue = 0;
    int counter_direction = 0;

    while (true) 
    {
        if (counter_direction == 0 && counter_red <254) 
        {
            counter_direction = 0;
            led_strip_set_pixel(
                led_strip,
                0, 
                counter_red,
                counter_green,
                counter_blue
            );
        

            counter_red += 1;
            counter_green += 1;
            counter_blue += 1;

        } 
        else if (counter_direction == 1 && counter_red > 0)
        {
            led_strip_set_pixel(
                led_strip,
                0, 
                counter_red,
                counter_green,
                counter_blue
            );
        
            
            counter_red -= 1;
            counter_green -= 1;
            counter_blue -= 1;
        }
        else if (counter_red == 0) 
        {
            counter_direction = 0;
        }
        else if (counter_red == 254) 
        {
            counter_direction = 1;
        }
        ESP_LOGI(TAG,"Red Colour Pixel : %d", counter_red);
        // ESP_LOGI(counter_red, "RED Color Pixel Counter")
        led_strip_refresh(led_strip);
        vTaskDelay(10/portTICK_PERIOD_MS);
    };
    // esp_err_t err = led_strip_set_pixel (
    //     led_strip,
    //     0, 
    //     254,
    //     254,
    //     254
    // );

    
}
#endif //CONFIG__LED_ONBOARD_SINGLE