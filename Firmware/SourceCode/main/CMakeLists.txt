
# Hal
file(GLOB_RECURSE HAL_SRCS
    hal/*.c
    hal/*.cpp
)
set(HAL_INCS
    hal/
)

# Hardware manager
file(GLOB_RECURSE HM_SRCS
    hardware_manager/*.c
    hardware_manager/*.cpp
)
set(HM_INCS
    hardware_manager/
)

# Apps
file(GLOB_RECURSE USER_APPS_SRCS
    user_apps/*.c
    user_apps/*.cpp
)
set(USER_APPS_INCS
    user_apps/
)


idf_component_register(SRCS "monica.cpp" ${HAL_SRCS} ${USER_APPS_SRCS} ${HM_SRCS}
                    INCLUDE_DIRS "." ${HAL_INCS} ${USER_APPS_INCS} ${HM_INCS})
