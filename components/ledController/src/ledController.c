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
#include "led_strip.h"
#include "esp_random.h"

#define BLINK_GPIO GPIO_NUM_8
static led_strip_handle_t led_strip;

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

#ifdef CONFIG__LED_ONBOARD_RGB
    static void led_RGB_init();
    void set_random_rgb_colour(void);
    void set_strobe_mode(void);
#endif //CONFIG__LED_ONBOARD_RGB

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
        led_RGB_init();
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
        #if CONFIG__LED_ONBOARD_SINGLE
            //led_single_task();
        #endif

        #ifdef CONFIG__LED_ONBOARD_RGB
            // No implementation for RGB LED
            set_random_rgb_colour();
            // set_strobe_mode();
        #endif

        // TODO: Better implementation for this is to not use 
        // vTaskDelay for LED toggling - We should setup system timers to do 
        // This instead. look at freerots/timers.h 
        vTaskDelay(500/portTICK_PERIOD_MS);
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

#endif //CONFIG__LED_ONBOARD_SINGLE

int generate_random_int() {
    unsigned int seed = esp_random();
    return seed % 255;
}

static uint8_t led_red_value;

#ifdef CONFIG__LED_ONBOARD_RGB
static void led_RGB_init()
{ 
    led_red_value = 0;
    /* LED strip initialization with the GPIO and pixels number*/
    led_strip_config_t strip_config = {
        .strip_gpio_num = BLINK_GPIO, // The GPIO that connected to the LED strip's data line
        .max_leds = 1, // The number of LEDs in the strip,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB, // Pixel format of your LED strip
        .led_model = LED_MODEL_WS2812, // LED strip model
        .flags.invert_out = false, // whether to invert the output signal (useful when your hardware has a level inverter)
    };

    led_strip_rmt_config_t rmt_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT, // different clock source can lead to different power consumption
        .resolution_hz = 10 * 1000 * 1000, // 10MHz
        .flags.with_dma = false, // whether to enable the DMA feature
    };

    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));
}

void set_smooth_red(void)
{
    led_strip_set_pixel(led_strip, 0, led_red_value, 0, 0);
    led_strip_refresh(led_strip);
}

void set_random_rgb_colour(void)
{
    int red_value = generate_random_int();
    int green_value = generate_random_int();
    int blue_value = generate_random_int();

    ESP_LOGI(TAG, "Creating new RGB Value (%d,%d,%d)", red_value, green_value, blue_value);

    led_strip_set_pixel(led_strip, 0, red_value, green_value, blue_value);
    led_strip_refresh(led_strip);
}

void set_strobe_mode(void)
{
    led_strip_set_pixel(led_strip, 0, 0, 0, 0);
    led_strip_refresh(led_strip);

    vTaskDelay(50/portTICK_PERIOD_MS);

    led_strip_set_pixel(led_strip, 0, 254, 254, 254);
    led_strip_refresh(led_strip);
}


#endif //CONFIG__LED_ONBOARD_RGB
