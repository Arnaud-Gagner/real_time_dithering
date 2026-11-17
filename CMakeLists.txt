cmake_minimum_required(VERSION 4.1)

project(dither_filter_rs VERSION 0.1.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# [vcpkg]
find_package(OpenCV CONFIG REQUIRED COMPONENTS core photo video videoio highgui)

set (SOURCES
    src/dither.cpp
    src/io_utils.cpp
    src/main.cpp
)

file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/include/*.h"
)

add_executable(${PROJECT_NAME}
    ${RC}
    ${SOURCES}
    ${HEADERS}
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        opencv_core
        opencv_photo
        opencv_video
        opencv_videoio
        opencv_highgui
)

# [Optimization stuff]
include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)
if(ipo_supported)
    set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
else()
    message(WARNING "IPO (LTO) not supported: ${ipo_error}")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:-O3 -march=native -flto>
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE
        $<$<CONFIG:Release>:/O2 /GL>
    )
    set_target_properties(${PROJECT_NAME} PROPERTIES
        LINK_FLAGS_RELEASE "/LTCG"
    )
endif()

if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wshadow -Wconversion)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
endif()
