# CMakeLists.txt of BMI270 component

# Source files directories
set(BMI270_SRC_DIRS
    src/
    src/utilities/BMI270-Sensor-API
    src/utilities/BMM150-Sensor-API
)

# Head files directories
set(BMI270_INC_DIRS
    src/
)

# Public component requirement
set(BMI270_REQUIRES
    "driver"
    "esp_timer"
)

# Private component requirement
set(BMI270_PRIV_REQUIRES
)

# Register component
idf_component_register(SRC_DIRS ${BMI270_SRC_DIRS}
                  INCLUDE_DIRS ${BMI270_INC_DIRS}
                  REQUIRES ${BMI270_REQUIRES}
                  PRIV_REQUIRES ${BMI270_PRIV_REQUIRES}
)