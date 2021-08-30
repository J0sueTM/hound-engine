/**
 * @file src/video/renderer/vulkan/linux_vulkan_renderer.h
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date August 30, 2021
 *
 * Copyright (C) 2021 Josue Teodoro Moreira
 *  
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details.
 */

#include "common_vulkan_renderer.h"

int
hnd_create_surface
(
  hnd_vulkan_renderer_t *_renderer,
  xcb_connection_t      *_connection,
  xcb_window_t           _window
);
