# Set the minimum version of CMake that can be used
# To find the cmake version run
# $ cmake --version
cmake_minimum_required(VERSION 3.5)


# try conditional compilation
#include(CheckCXXCompilerFlag)
#CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
#CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)

#if(COMPILER_SUPPORTS_CXX11)#
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#elseif(COMPILER_SUPPORTS_CXX0X)#
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
#else()
#    message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
#endif()

#set_property(TARGET prog PROPERTY CXX_STANDARD 11)
#set_property(TARGET prog PROPERTY CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 20)

# Set a default build type if none was specified
# Example command to set other CMAKE_BUILD_TYPE: cmake .. -DCMAKE_BUILD_TYPE=Release
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()


# Set the project name
project(AES VERSION 1.0)



#Generate the static library from the library source
add_library(static_lib STATIC src/AES.cpp src/encrypt.cpp src/decrypt.cpp src/trace.cpp src/util.cpp)

target_include_directories(static_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)


#Add excutable with above sources
add_executable(${PROJECT_NAME} src/main.cpp)

#link with static lib
target_link_libraries(${PROJECT_NAME} PRIVATE static_lib)


# set the C++ standard to the appropriate standard for using auto
#target_compile_features(${PROJECT_NAME} PUBLIC cxx_auto_type)

# Print the list of known compile features for this version of CMake
#message("List of compile features: ${CMAKE_CXX_COMPILE_FEATURES}")
