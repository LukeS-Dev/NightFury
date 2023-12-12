idf_component_register(
    SRCS "src/ledController.c"
    INCLUDE_DIRS "inc" "src"
    PRIV_REQUIRES
        driver
    )