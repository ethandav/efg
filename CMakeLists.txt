cmake_minimum_required(VERSION 3.12)
project(efg)

execute_process(COMMAND git submodule update --init
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})


set(GFX_ENABLE_GUI ON CACHE BOOL "Enable GUI for gfx" FORCE)
set(GFX_ENABLE_SCENE ON CACHE BOOL "Enable Scene for gfx" FORCE)

add_subdirectory(${CMAKE_SOURCE_DIR}/gfx)