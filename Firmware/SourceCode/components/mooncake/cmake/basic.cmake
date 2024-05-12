# Basic CMakeLists.txt

cmake_minimum_required(VERSION 3.10)
set(CMAKE_C_STANDARD 11)#C11
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Mooncake
file(GLOB_RECURSE MOONCAKE_SRCS
    ${MOONCAKE_ROOT_DIR}/src/app/*.cpp
    ${MOONCAKE_ROOT_DIR}/src/framework/*.cpp
    ${MOONCAKE_ROOT_DIR}/src/simplekv/simplekv.cpp
)
set(MOONCAKE_INCS
    ${MOONCAKE_ROOT_DIR}/src/
    ${MOONCAKE_ROOT_DIR}/src/app/
    ${MOONCAKE_ROOT_DIR}/src/framework/
    ${MOONCAKE_ROOT_DIR}/src/simplekv/
)


# Built-in Apps
file(GLOB_RECURSE BUILTIN_SRCS
    ${MOONCAKE_ROOT_DIR}/src/builtin_apps/*.c
    ${MOONCAKE_ROOT_DIR}/src/builtin_apps/*.cpp
)
set(BUILTIN_INCS
    ${MOONCAKE_ROOT_DIR}/src/builtin_apps/
    ${MOONCAKE_ROOT_DIR}/src/builtin_apps/assets/
    ${MOONCAKE_ROOT_DIR}/src/builtin_apps/boot_anim/
    ${MOONCAKE_ROOT_DIR}/src/builtin_apps/launcher/
    ${MOONCAKE_ROOT_DIR}/src/builtin_apps/wf_spring_wreath/
    ${MOONCAKE_ROOT_DIR}/src/builtin_apps/wf_user_custom/
)



#  Build library
add_library(mooncake
    ${MOONCAKE_ROOT_DIR}/src/mooncake.cpp
    ${MOONCAKE_SRCS}
    ${BUILTIN_SRCS}
)


# Include
target_include_directories(mooncake PUBLIC
    ${MOONCAKE_INCS}
    ${BUILTIN_INCS}
)


# Link
target_link_libraries(mooncake PUBLIC lvgl lvgl::examples lvgl::demos ArduinoJson)
