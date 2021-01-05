INCLUDE_DIRECTORIES(
    ${FASTAHACK_INCLUDE}
	${CXXOPTS_INCLUDE}
	${ZLIB_INCLUDE}
	${HTSLIB_INCLUDE}
	${TABIX_INCLUDE}
	${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # clang Doesnt use pthread
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

set(CVM_UTIL_SOURCES
  utils/Utility.cpp
  utils/Params.cpp
  utils/gzstream.cpp
)

set(CVM_VCF_SOURCES
  vcf/Index.cpp
  vcf/VCF.cpp
  vcf/Variant.cpp
)

set(CVM_REFERENCE_SOURCES
  reference/Reference.cpp
)

add_library(cvm_core STATIC
  ${CVM_UTIL_SOURCES}
  ${CVM_VCF_SOURCES}
  ${CVM_REFERENCE_SOURCES}
)

SET(CORE_LIB cvm_core CACHE STRING "THE CORE LIBRARY") #add graphite_core and graphite_utils to the list of core libs

TARGET_LINK_LIBRARIES(${CORE_LIB}
  ${TABIX_LIB}
  ${HTSLIB_LIB}
  ${ZLIB_LIBRARY}
  ${FASTAHACK_LIB}
)

add_dependencies(${CORE_LIB} ${CVM_EXTERNAL_PROJECT})