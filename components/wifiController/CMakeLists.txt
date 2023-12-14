idf_component_register(
    SRCS "src/wifiController.c"
    INCLUDE_DIRS 
        "inc" 
        "src"
        "../../include"
    PRIV_REQUIRES 
        "esp_wifi"
    )

