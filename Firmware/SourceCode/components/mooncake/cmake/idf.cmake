# CMakeLists.txt of MOONCAKE component

# Mooncake
file(GLOB_RECURSE MOONCAKE_SRCS
    ${MOONCAKE_ROOT_DIR}/src/app/*.cpp
    ${MOONCAKE_ROOT_DIR}/src/framework/*.cpp
    ${MOONCAKE_ROOT_DIR}/src/simplekv/simplekv.cpp
)
set(MOONCAKE_INC_DIRS
    ${MOONCAKE_ROOT_DIR}/src/
    ${MOONCAKE_ROOT_DIR}/src/app/
    ${MOONCAKE_ROOT_DIR}/src/framework/
    ${MOONCAKE_ROOT_DIR}/src/simplekv/
)

# Built-in Apps
file(GLOB_RECURSE BUILTIN_APPS_SRCS
    ${MOONCAKE_ROOT_DIR}/src/builtin_apps/*.c
    ${MOONCAKE_ROOT_DIR}/src/builtin_apps/*.cpp
)
set(MOONCAKE_BUILTIN_APPS_INC_DIRS
    ${MOONCAKE_ROOT_DIR}/src/builtin_apps/
    ${MOONCAKE_ROOT_DIR}/src/builtin_apps/boot_anim/
    ${MOONCAKE_ROOT_DIR}/src/builtin_apps/launcher/
    ${MOONCAKE_ROOT_DIR}/src/builtin_apps/wf_spring_wreath/
    ${MOONCAKE_ROOT_DIR}/src/builtin_apps/wf_user_custom/
    ${MOONCAKE_ROOT_DIR}/src/builtin_apps/assets/
)

# Public component requirement
set(MOONCAKE_REQUIRES
    lvgl ArduinoJson
)

# Private component requirement
set(MOONCAKE_PRIV_REQUIRES
)

# Register component
idf_component_register(SRCS ${MOONCAKE_SRCS} ${BUILTIN_APPS_SRCS} ${MOONCAKE_ROOT_DIR}/src/mooncake.cpp
                #   SRC_DIRS ${MOONCAKE_SRC_DIRS}
                  INCLUDE_DIRS ${MOONCAKE_INC_DIRS} ${MOONCAKE_BUILTIN_APPS_INC_DIRS}
                  REQUIRES ${MOONCAKE_REQUIRES}
                  PRIV_REQUIRES ${MOONCAKE_PRIV_REQUIRES}
)
