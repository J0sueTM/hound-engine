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
message("-- Created lib directory")

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

project(Hound VERSION 1.0 LANGUAGES C)
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(HOUND_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core/debug.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/video/window.c)

find_package(X11 REQUIRED)
if (HOUND_USE_OPENGL)
  find_package(OpenGL REQUIRED)
  set(HOUND_LIBRARIES X11 X11-xcb ${X11_LIBRARIES} OpenGL::GL)
  set(HOUND_COMPILE_DEFINITIONS "-Wall -Werror -pedantic -D HND_USE_OPENGL")

  message("-- Rendering with OpenGL")
elseif (HOUND_USE_VULKAN)
  find_package(Vulkan REQUIRED)
  set(HOUND_LIBRARIES X11 ${X11_LIBRARIES} Vulkan)
  set(HOUND_COMPILE_DEFINITIONS "-Wall -Werror -pedantic -D HND_USE_VULKAN")

  message("-- Rendering with Vulkan")
else ()
  set(HOUND_LIBRARIES X11 ${X11_LIBRARIES})
  set(HOUND_COMPILE_DEFINITIONS "-Wall -Werror -pedantic -D HND_USE_SOFTWARE")

  message("-- Rendering with Software")
endif ()
set(HOUND_LINK_OPTIONS "-lxcb")

if (BUILD_SHARED_LIBS)
  set(HOUND_BUILD SHARED)

  message("-- Building shared libraries")
elseif ()
  set(HOUND_BUILD STATIC)

  message("-- Building static libraries")
endif ()

add_library(Hound ${HOUND_BUILD} ${HOUND_SRC})
target_link_libraries(Hound ${HOUND_LIBRARIES})

if (NOT HOUND_BUILD_TEST)
  set(${HOUND_COMPILE_DEFINITIONS} "${HOUND_COMPILE_DEFINITIONS} -O2")
  message("-- Using gcc optimization level 2")
endif ()
add_definitions(${HOUND_COMPILE_DEFINITIONS})
add_link_options(${HOUND_LINK_OPTIONS})

if (HOUND_BUILD_TEST)
  add_definitions("-g")
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/)

  message("-- Building Hound's tests")
endif ()
