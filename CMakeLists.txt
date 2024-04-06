cmake_minimum_required(VERSION 3.12)
project(efg)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "In-source builds are not allowed. Please create a build directory and run CMake from there. Example: mkdir build && cd build && cmake ..")
endif()

execute_process(COMMAND git submodule update --init
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

execute_process(COMMAND git submodule update --init
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/gfx)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/gfx/build)
execute_process(
    COMMAND ${CMAKE_COMMAND} -DGFX_ENABLE_GUI=ON -DGFX_ENABLE_SCENE=ON -S ${CMAKE_SOURCE_DIR}/gfx -B ${CMAKE_SOURCE_DIR}/gfx/build
)