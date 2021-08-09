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

#ifndef __HND_OPENGL_RENDERER_H__ 
#define __HND_OPENGL_RENDERER_H__ 

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "../video.h"

typedef struct hnd_opengl_renderer_t
{
  Display *display;
  GLXFBConfig *fb_configs;
  GLXFBConfig current_fb_config;
  int fb_config_count;
  
  int visual_id;
  int default_screen;
  
  GLXContext gl_context;
  GLXWindow gl_window;
} hnd_opengl_renderer_t;

/**
 * @brief Gets and sets frame buffer config matching current screen's attributes.
 *
 * @param _connection Specifies the connection where fb data resides and
 *                    should be updated.
 *
 * @return Function state. HND_OK or HND_NK.
 */
int
hnd_set_fb_configs
(
  hnd_opengl_renderer_t *_renderer
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_OPENGL_RENDERER_H__ */
