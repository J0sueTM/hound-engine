# file test/CMakeFiles.txt
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

add_executable(window ${CMAKE_CURRENT_SOURCE_DIR}/window.c)
target_link_libraries(window Hound)
