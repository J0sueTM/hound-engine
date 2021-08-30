/**
 * @file src/video/renderer/opengl_renderer.c
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date August 08, 2021
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

/**
 * @brief Frame buffer configs used on OpenGL context creation
 */
static int fb_configs[] =
{
  GLX_X_RENDERABLE,   True,
  GLX_DRAWABLE_TYPE,  GLX_WINDOW_BIT,
  GLX_RENDER_TYPE,    GLX_RGBA_BIT,
  GLX_X_VISUAL_TYPE,  GLX_TRUE_COLOR,
  GLX_RED_SIZE,       8,
  GLX_GREEN_SIZE,     8,
  GLX_BLUE_SIZE,      8,
  GLX_ALPHA_SIZE,     8,
  GLX_DEPTH_SIZE,     24,
  GLX_STENCIL_SIZE,   8,
  GLX_DOUBLEBUFFER,   True,
  GLX_SAMPLE_BUFFERS, 1,
  GLX_SAMPLES,        4,
  None
};

int
hnd_set_fb_configs
(
  hnd_opengl_renderer_t *_renderer
)
{
  _renderer->fb_configs = glXChooseFBConfig(_renderer->display,
                                            _renderer->default_screen,
                                            fb_configs,
                                            &_renderer->fb_config_count);
  if (!hnd_assert(_renderer->fb_configs != 0, "Could not get frame buffer configs"))
    return HND_NK;

  /* Get the first of frame buffer config list */
  _renderer->current_fb_config = *(_renderer->fb_configs);
  glXGetFBConfigAttrib(_renderer->display, _renderer->current_fb_config, GLX_VISUAL_ID, &_renderer->visual_id);
  
  return HND_OK;
}

int
hnd_init_renderer
(
  hnd_renderer_t *_renderer
)
{
  if (!hnd_assert(_renderer != NULL, HND_SYNTAX))
    return HND_NK;

  _renderer->gl_context = glXCreateNewContext(_renderer->display,
                                              _renderer->current_fb_config,
                                              GLX_RGBA_TYPE,
                                              0,
                                              True);
  if (!hnd_assert(_renderer->gl_context != NULL, "Could not create OpenGL rendering context"))
    return HND_NK;
  
  return HND_OK;
}

void
hnd_end_renderer
(
  hnd_renderer_t *_renderer
)
{
  if (!hnd_assert(_renderer != NULL, HND_SYNTAX))
    return;

  glXDestroyContext(_renderer->display, _renderer->gl_context);
  XCloseDisplay(_renderer->display);
  
  hnd_print_debug(HND_LOG, HND_ENDED("opengl renderer"), HND_SUCCESS);  
}

void
hnd_swap_renderer_buffers
(
  hnd_renderer_t *_renderer
)
{
  glXSwapBuffers(_renderer->display, _renderer->gl_window);
}
