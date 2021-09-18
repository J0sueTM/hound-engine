/**
 * @file src/video/renderer/opengl_renderer.h
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
 *
 * @note: https://xcb.freedesktop.org/opengl/
 */

#ifndef __HND_LINUX_OPENGL_RENDERER_H__ 
#define __HND_LINUX_OPENGL_RENDERER_H__ 

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "../video.h"

typedef struct hnd_linux_renderer_t
{
  Display *display;
  GLXFBConfig *fb_configs;
  GLXFBConfig current_fb_config;
  int fb_config_count;

  int visual_id;
  int default_screen;

  GLXContext gl_context;
  GLXWindow gl_window;
} hnd_linux_renderer_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_LINUX_OPENGL_RENDERER_H__ */
