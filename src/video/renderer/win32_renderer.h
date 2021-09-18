/**
 * @file src/video/renderer/opengl/win32_opengl_renderer.h
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

#ifndef __HND_WIN32_OPENGL_RENDERER_H__
#define __HND_WIN32_OPENGL_RENDERER_H__

#include "../video.h"

typedef struct hnd_win32_renderer_t
{
  HGLRC gl_context;
  int pixel_format;
  HDC device_context;
} hnd_win32_renderer_t;

#endif /* __HND_WIN32_OPENGL_RENDERER_H__ */
