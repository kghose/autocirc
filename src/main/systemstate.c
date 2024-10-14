#include <stddef.h>
#include <string.h> //for memcpy

#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

#include "systemstate.h"

static const char *TAG = "systemstate";
SemaphoreHandle_t system_state_mutex = NULL;
SystemState shared_system_state;

void initialize_system_state_mutex() {
  system_state_mutex = xSemaphoreCreateMutex();
}

bool get_system_state(SystemState *system_state) {
  if (system_state_mutex == NULL) {
    ESP_LOGE(TAG, "Use initialize_system_state_mutex in main thread before "
                  "accessing system state.");
    return false;
  }

  if (xSemaphoreTake(system_state_mutex, portMAX_DELAY) == pdTRUE) {
    memcpy(&shared_system_state, system_state, sizeof(SystemState));
    return true;
  } else {
    ESP_LOGW(TAG, "Could not read system state: Timeout trying to get lock.");
    return false;
  }
}

bool set_system_state(SystemState *system_state) {
  if (system_state_mutex == NULL) {
    ESP_LOGE(
        TAG,
        "Use initialize_mutex in main thread before accessing system state.");
    return false;
  }

  if (xSemaphoreTake(system_state_mutex, portMAX_DELAY) == pdTRUE) {
    memcpy(system_state, &shared_system_state, sizeof(SystemState));
    return true;
  } else {
    ESP_LOGW(TAG, "Could not write system state: Timeout trying to get lock.");
    return false;
  }
}
