/**
 * @file src/video/window/linux_window.h
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date August 25, 2021
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

#ifndef __HND_LINUX_WINDOW_H__
#define __HND_LINUX_WINDOW_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "../video.h"
#include "../renderer/renderer.h"

/**
 * @brief Linux window data.
 */
typedef struct hnd_linux_window_t
{
  char *title;
  unsigned int border;
  unsigned int left;
  unsigned int top;
  unsigned int width;
  unsigned int height;
  unsigned int decoration;
  unsigned int fullscreen;
  int running;

  xcb_connection_t *connection;
  xcb_screen_t *screen_data;
  xcb_colormap_t colormap_id;

  xcb_window_t id;
  uint32_t event_mask;
  uint32_t value_mask;
  uint32_t value_list[2];

  xcb_intern_atom_reply_t *wm_protocols;
  xcb_intern_atom_reply_t *wm_delete_window;

  hnd_renderer_t renderer;
} hnd_linux_window_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_LINUX_WINDOW_H__ */
