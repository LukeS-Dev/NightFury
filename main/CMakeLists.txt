idf_component_register(
    SRCS "nightfury.c"
    INCLUDE_DIRS "."
    REQUIRES
    # ESP32 Components
        freertos
        esp_common
        nvs_flash
    # Project Components
        wifiController
    )
