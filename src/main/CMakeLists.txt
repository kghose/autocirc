idf_component_register(
    SRCS "main.c" "systemstate.c"
    INCLUDE_DIRS "."
    REQUIRES wifi esp_event nvs_flash 
)
