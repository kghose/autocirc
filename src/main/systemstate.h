#include <stdbool.h>

typedef struct {
  int32_t anti_freeze_temp_c;
  int32_t system_switchover_temp_c;
} UserSettings;

typedef struct {
  bool thermostat;
  bool circulatory_failure;
  int32_t outside_temp_c;
  int32_t pipe_temp_c;
} OilHeatState;

typedef struct {} HeatPumpState;

typedef struct {
  UserSettings user_settings;
  OilHeatState oil_heat_state;
  HeatPumpState heat_pump_state;
} SystemState;

void initialize_system_state_mutex();
bool get_system_state(SystemState *system_state);
bool set_system_state(SystemState *system_state);
