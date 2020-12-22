SET_PROPERTY(DIRECTORY PROPERTY "EP_BASE" ${ep_base})

SET(CXXOPTS_PROJECT cxxopts_project CACHE INTERNAL "cxxopts project name")
SET(CXXOPTS_DIR ${CMAKE_BINARY_DIR}/externals/cxxopts CACHE INTERNAL "cxxopts project directory")
ExternalProject_Add(${CXXOPTS_PROJECT}
	GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
	GIT_TAG 6c946a413ad8917ca1737953a9b406501b0b68f6 #lock in the commit id so we don't this doesn't break in the future
	INSTALL_COMMAND ""
	BUILD_COMMAND ""
    UPDATE_COMMAND ""
	CONFIGURE_COMMAND ""
	PREFIX ${CXXOPTS_DIR}
    CMAKE_CACHE_ARGS
        -DCMAKE_C_COMPILER:STRING=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}
)

ExternalProject_Get_Property(${CXXOPTS_PROJECT} INSTALL_DIR)
ExternalProject_Get_Property(${CXXOPTS_PROJECT} SOURCE_DIR)
ExternalProject_Get_Property(${CXXOPTS_PROJECT} BINARY_DIR)

SET(CXXOPTS_INCLUDE ${SOURCE_DIR}/src CACHE INTERNAL "cxxopts Include")
