idf_component_register(
    SRCS "src/ledController.c"
    INCLUDE_DIRS
        "inc" 
        "src"
        "../../include"
    PRIV_REQUIRES
        driver
        espressif__led_strip
    )