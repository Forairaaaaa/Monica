# CMakeLists.txt of SIMPLEKV component

# Source files directories
set(SIMPLEKV_SRC_DIRS
    ${SIMPLEKV_ROOT_DIR}
    ${SIMPLEKV_ROOT_DIR}/platform/esp
)

# Head files directories
set(SIMPLEKV_INC_DIRS
    ${SIMPLEKV_ROOT_DIR}
    ${SIMPLEKV_ROOT_DIR}/platform/esp
)

# Public component requirement
set(SIMPLEKV_REQUIRES
)

# Private component requirement
set(SIMPLEKV_PRIV_REQUIRES
)

# Register component
idf_component_register(SRC_DIRS ${SIMPLEKV_SRC_DIRS}
                  INCLUDE_DIRS ${SIMPLEKV_INC_DIRS}
                  REQUIRES ${SIMPLEKV_REQUIRES}
                  PRIV_REQUIRES ${SIMPLEKV_PRIV_REQUIRES}
)
