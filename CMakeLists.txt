cmake_minimum_required(VERSION 3.17)
project(DataStructureLabSession4)
include(CTest)
enable_testing()

file(GLOB_RECURSE SRC_FILES src/*.cpp)
include_directories(GLOB_RECURSE include)
link_directories(GLOB_RECURSE lib)
if (MSVC)
    set(CMAKE_CXX_FLAGS "/utf-8 /std:c++latest /EHsc")
    set(CMAKE_CXX_FLAGS_DEBUG "/Od")
    set(CMAKE_CXX_FLAGS_RELEASE "/O2")
elseif (CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-std=c++20 -Wall")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
else ()
    set(CMAKE_CXX_FLAGS "-std=c++20 -Wall")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -ggdb")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif ()
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/release)
add_executable(DataStructureLabSession4 ${SRC_FILES})
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)