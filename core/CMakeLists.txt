INCLUDE_DIRECTORIES(
    ${FASTAHACK_INCLUDE}
	${CXXOPTS_INCLUDE}
	${ZLIB_INCLUDE}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # clang Doesnt use pthread
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

set(CVM_UTIL_SOURCES
  utils/Params.cpp
  utils/gzstream.cpp
)

set(CVM_VCF_SOURCES
  vcf/Index.cpp
)

add_library(cvm_core STATIC
  ${CVM_UTIL_SOURCES}
  ${CVM_VCF_SOURCES}
)

SET(CORE_LIB cvm_core CACHE STRING "THE CORE LIBRARY") #add graphite_core and graphite_utils to the list of core libs

add_dependencies(${CORE_LIB} ${CVM_EXTERNAL_PROJECT})