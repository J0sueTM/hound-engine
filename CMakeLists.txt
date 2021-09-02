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

if (WIN32 AND NOT MSVC)
  set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
  set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
else ()
  set(CMAKE_C_COMPILER gcc)
  set(CMAKE_CXX_COMPILER g++)
endif ()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib)

project(Hound VERSION "0.1.0" LANGUAGES C)

if (NOT WIN32)
  find_package(X11 REQUIRED)
endif ()

if (HOUND_USE_VULKAN)
  find_package(Vulkan REQUIRED)
  if (WIN32)
    set(HOUND_LIBRARIES Vulkan::Vulkan)
  else ()
    set(HOUND_LIBRARIES X11 ${X11_LIBRARIES} vulkan)
  endif ()

  set(HOUND_BACKEND "vulkan")
  if (MSVC)
    set(HOUND_COMPILE_DEFINITIONS "/D HND_USE_VULKAN")
  else ()
    set(HOUND_COMPILE_DEFINITIONS "-Wall -Werror -pedantic -D HND_USE_VULKAN")
  endif ()

  message("-- Rendering with Vulkan")
else ()
  find_package(OpenGL REQUIRED)
  if (WIN32)
    set(HOUND_LIBRARIES opengl32)
  else ()
    set(HOUND_LIBRARIES X11 X11-xcb ${X11_LIBRARIES} OpenGL::GL)
  endif ()

  set(HOUND_BACKEND "opengl")
  if (MSVC)
    set(HOUND_COMPILE_DEFINITIONS "/D HND_USE_OPENGL")
  else ()
    set(HOUND_COMPILE_DEFINITIONS "-Wall -Werror -pedantic -D HND_USE_OPENGL")
  endif ()
  
  message("-- Rendering with OpenGL")
endif ()

if (HOUND_DEBUG)
  if (NOT MSVC)
    add_definitions("-g")
  endif ()

  if (MSVC)
    set(HOUND_COMPILE_DEFINITIONS "${HOUND_COMPILE_DEFINITIONS} /D HND_DEBUG")
  else ()
    set(HOUND_COMPILE_DEFINITIONS "${HOUND_COMPILE_DEFINITIONS} -D HND_DEBUG")
  endif ()
else ()
  if (MSVC)
    set(HOUND_COMPILE_DEFINITIONS "${HOUND_COMPILE_DEFINITIONS} /D HND_RELEASE")
  else ()
    set(HOUND_COMPILE_DEFINITIONS "${HOUND_COMPILE_DEFINITIONS} -D HND_RELEASE")
  endif ()
endif ()

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

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(HOUND_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core/debug.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/core/event/${HOUND_OS}_event.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/video/video.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/video/window/${HOUND_OS}_window.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/video/renderer/${HOUND_BACKEND}/common_${HOUND_BACKEND}_renderer.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/video/renderer/${HOUND_BACKEND}/${HOUND_OS}_${HOUND_BACKEND}_renderer.c)

if (WIN32)
  set(HOUND_LINK_OPTIONS "-lgdi32")
else ()
  set(HOUND_LINK_OPTIONS "-lxcb")
endif ()

if (BUILD_SHARED_LIBS)
  set(HOUND_BUILD SHARED)

  message("-- Building shared libraries")
else ()
  set(HOUND_BUILD STATIC)

  message("-- Building static libraries")
endif ()

add_library(Hound ${HOUND_BUILD} ${HOUND_SRC})
target_link_libraries(Hound ${HOUND_LIBRARIES})

add_definitions(${HOUND_COMPILE_DEFINITIONS})
add_link_options(${HOUND_LINK_OPTIONS})

if (HOUND_BUILD_TEST)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/test/)

  message("-- Building Hound's tests")
endif ()
