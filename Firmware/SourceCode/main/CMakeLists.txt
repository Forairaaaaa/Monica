
# Hal
file(GLOB_RECURSE HAL_SRCS
    hal/*.c
    hal/*.cpp
)
set(HAL_INCS
    hal/
)

# Apps
file(GLOB_RECURSE USER_APPS_SRCS
    user_apps/*.c
    user_apps/*.cpp
)
set(USER_APPS_INCS
    user_apps/
)


idf_component_register(SRCS "monica.cpp" ${HAL_SRCS} ${USER_APPS_SRCS}
                    INCLUDE_DIRS "." ${HAL_INCS} ${USER_APPS_INCS})
