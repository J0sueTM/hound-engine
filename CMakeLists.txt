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

# Directories
file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
message("-- Created lib directory")

# Compiler
if (WIN32 AND NOT MSVC)
  set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
  set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
else ()
  set(CMAKE_C_COMPILER gcc)
  set(CMAKE_CXX_COMPILER g++)
endif ()

# Project
project(Hound VERSION "0.1.0" LANGUAGES C)

# Libraries
find_package(OpenGL REQUIRED)
if (WIN32)
  set(HOUND_LIBRARIES opengl32)
  set(HOUND_LINK_OPTIONS "-lgdi32")
else ()
  find_package(X11 REQUIRED)

  set(HOUND_LIBRARIES X11 X11-xcb ${X11_LIBRARIES} OpenGL::GL)
  set(HOUND_LINK_OPTIONS "-lxcb")
endif ()

# Build configuration
set(HOUND_COMPILE_DEFINITIONS "-Wall -Werror -pedantic")
if (HOUND_DEBUG)
  if (NOT MSVC)
    add_definitions("-g")
  endif ()

  if (MSVC)
    set(HOUND_COMPILE_DEFINITIONS "${HOUND_COMPILE_DEFINITIONS} /DEBUG /D HND_DEBUG")
  else ()
    set(HOUND_COMPILE_DEFINITIONS "${HOUND_COMPILE_DEFINITIONS} -D HND_DEBUG")
  endif ()

  message("-- Building debug configuration")
else ()
  message("-- Building release configuration")
endif ()

# OS
if (WIN32)
  set(HOUND_OS "win32")

  if (MSVC)
    set(HOUND_COMPILE_DEFINITIONS "${HOUND_COMPILE_DEFINITIONS} /D HND_WIN32")
  else ()
    set(HOUND_COMPILE_DEFINITIONS "${HOUND_COMPILE_DEFINITIONS} -D HND_WIN32")
  endif ()
  
  message("-- Building for windows")
else ()
  set(HOUND_OS "linux")
  set(HOUND_COMPILE_DEFINITIONS "${HOUND_COMPILE_DEFINITIONS} -D HND_LINUX")

  message("-- Bulding for linux")
endif ()

# Source
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(HOUND_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core/debug.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core/event/${HOUND_OS}_event.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/video/window/${HOUND_OS}_window.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/video/renderer/common_renderer.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/video/renderer/${HOUND_OS}_renderer.c)

if (BUILD_SHARED_LIBS)
  set(HOUND_BUILD SHARED)

  message("-- Building shared libraries")
else ()
  set(HOUND_BUILD STATIC)

  message("-- Building static libraries")
endif ()

# Library
add_library(Hound ${HOUND_BUILD} ${HOUND_SRC})
target_link_libraries(Hound ${HOUND_LIBRARIES})

add_definitions(${HOUND_COMPILE_DEFINITIONS})
add_link_options(${HOUND_LINK_OPTIONS})

# Tests
if (HOUND_BUILD_TEST)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/)

  message("-- Building Hound's tests")
endif ()
