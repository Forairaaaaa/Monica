# Basic CMakeLists.txt

set(SIMPLEKV_SRCS
    "simplekv.cpp"
)

add_library(simplekv ${SIMPLEKV_SRCS})

target_include_directories(simplekv PUBLIC
    "."
)
