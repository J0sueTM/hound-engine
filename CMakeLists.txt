# file CMakeFiles.txt
# author Josué Teodoro Moreira <teodoro.josue@protonmail.ch>
# date August 01, 2021
#
# Copyright (C) Josué Teodoro Moreira
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

cmake_minimum_required(VERSION 3.20)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
message("Created lib directory")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

project(Hound VERSION 1.0 LANGUAGES C)
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(HOUND_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core/debug.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/video/window.c)

find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)

if (BUILD_SHARED_LIBS)
  set(HOUND_BUILD SHARED)
elseif ()
  set(HOUND_BUILD STATIC)
endif ()
add_library(Hound ${HOUND_BUILD} ${HOUND_SRC})
add_definitions("-Wall -Werror -pedantic")
add_link_options("-lxcb")
target_link_libraries(Hound OpenGL::GL X11 ${X11_LIBRARIES})

if (HOUND_BUILD_TEST)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/)
endif ()
