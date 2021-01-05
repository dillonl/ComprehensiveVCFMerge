SET_PROPERTY(DIRECTORY PROPERTY "EP_BASE" ${ep_base})

SET(HTSLIB_PROJECT htslib_project CACHE INTERNAL "htslib project name")
SET(HTSLIB_DIR ${CMAKE_BINARY_DIR}/externals/htslib CACHE INTERNAL "htslib project directory")
SET(HTSLIB_LIB)
ExternalProject_Add(${HTSLIB_PROJECT}
	GIT_REPOSITORY https://github.com/dillonl/htslib.git
	GIT_TAG 831c05b184360880ce77be013f22ab2b62b7e481 #lock in the commit id so we don't this doesn't break in the future
	DEPENDS ${ZLIB_PROJECT}
	INSTALL_COMMAND ""
	UPDATE_COMMAND ""
	PREFIX ${HTSLIB_DIR}
    CMAKE_CACHE_ARGS
	    -DCMAKE_BUILD_TYPE:STRING=Debug
        -DCMAKE_C_COMPILER:STRING=${CMAKE_C_COMPILER}
        -DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}
		-DZLIB_LIBRARY_PATH:PATH=${ZLIB_LIBRARY_PATH}
		-DZLIB_INCLUDE:PATH=${ZLIB_INCLUDE}
)

ExternalProject_Get_Property(${HTSLIB_PROJECT} INSTALL_DIR)
ExternalProject_Get_Property(${HTSLIB_PROJECT} SOURCE_DIR)
ExternalProject_Get_Property(${HTSLIB_PROJECT} BINARY_DIR)

SET(HTSLIB_LIB ${BINARY_DIR}/core/libhtslib_core.a CACHE INTERNAL "HTSLib library file")
SET(HTSLIB_INCLUDE ${SOURCE_DIR}/core CACHE INTERNAL "HTSLIB Include")
#SET(TABIX_LIB ${BINARY_DIR}/externals/tabix/src/tabix_project-build/libtabix.a CACHE INTERNAL "TABIX library file")