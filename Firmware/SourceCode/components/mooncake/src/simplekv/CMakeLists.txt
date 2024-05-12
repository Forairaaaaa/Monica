
set(SIMPLEKV_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR})

if (ESP_PLATFORM)
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/esp.cmake)
else()
    include(${CMAKE_CURRENT_LIST_DIR}/cmake/basic.cmake)
endif()
