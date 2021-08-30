/**
 * @file src/video/renderer/opengl_renderer.c
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

#include "../renderer.h"

void
hnd_clear_render
(
  float _red,
  float _green,
  float _blue,
  float _alpha
)
{
  glClearColor(_red, _green, _blue, _alpha);
  glClear(GL_COLOR_BUFFER_BIT);
}
