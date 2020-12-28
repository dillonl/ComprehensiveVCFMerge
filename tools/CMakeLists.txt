set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/)


set(CVM_TOOLS_SOURCES
	cvm.cpp
)

INCLUDE_DIRECTORIES(
  ${ZLIB_INCLUDE}
  ${FASTAHACK_INCLUDE}
  ${CXXOPTS_INCLUDE}
  ${PROJECT_SOURCE_DIR}/core
)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang") # clang Doesnt use pthread
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
endif()

#set(CMAKE_BUILD_TYPE Debug) # uncomment this when you need a debug build

add_executable(cvm
  ${CVM_TOOLS_SOURCES}
)

# link libraries to executables
target_link_libraries(cvm
  ${CORE_LIB}
)

add_dependencies(cvm ${CVM_EXTERNAL_PROJECT})
#install(TARGETS cvm DESTINATION bin)