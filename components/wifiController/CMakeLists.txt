idf_component_register(
    SRCS "src/wifiController.c"
    INCLUDE_DIRS "inc" "src"
    PRIV_REQUIRES 
        "esp_wifi"
    )

