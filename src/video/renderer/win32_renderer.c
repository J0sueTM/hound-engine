/**
 * @file src/video/renderer/opengl/win32_opengl_renderer.c
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

static PIXELFORMATDESCRIPTOR pixel_format_descriptor =
{
  sizeof(PIXELFORMATDESCRIPTOR),
  1,
  PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
  PFD_TYPE_RGBA,
  32,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  24,
  8,
  0,
  PFD_MAIN_PLANE,
  0,
  0,
  0,
  0
};

int
hnd_init_renderer
(
  hnd_renderer_t *_renderer
)
{
  if (!hnd_assert(_renderer != NULL, HND_SYNTAX))
    return HND_NK;

  _renderer->pixel_format = ChoosePixelFormat(_renderer->device_context, &pixel_format_descriptor);
  if (!hnd_assert(_renderer->pixel_format != 0, "Could not choose pixel format"))
    return HND_NK;

  if (!SetPixelFormat(_renderer->device_context, _renderer->pixel_format, &pixel_format_descriptor))
    return HND_NK;

  _renderer->gl_context = wglCreateContext(_renderer->device_context);
  if(!hnd_assert(_renderer->gl_context != NULL, "Could not create OpenGL context"))
    return HND_NK;

  if (!hnd_assert(wglMakeCurrent(_renderer->device_context, _renderer->gl_context),
                  "Could not make renderer context current"))
    return HND_NK;
  
  hnd_print_debug(HND_LOG, HND_CREATED("renderer"), HND_SUCCESS);
  return HND_OK;
}

void
hnd_end_renderer
(
  hnd_renderer_t *_renderer
)
{
  wglMakeCurrent(NULL, NULL);
  wglDeleteContext(_renderer->gl_context);
  
  hnd_print_debug(HND_LOG, HND_ENDED("renderer"), HND_SUCCESS);
}

void
hnd_swap_renderer_buffers
(
  hnd_renderer_t *_renderer
)
{
  SwapBuffers(_renderer->device_context);
}
