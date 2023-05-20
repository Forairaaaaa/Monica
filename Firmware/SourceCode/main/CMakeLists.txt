
# Hal
file(GLOB_RECURSE HAL_SRCS
    hal/*.c
    hal/*.cpp
)
set(HAL_INCS
    hal/
)


idf_component_register(SRCS "monica.cpp" ${HAL_SRCS}
                    INCLUDE_DIRS "." ${HAL_INCS})
