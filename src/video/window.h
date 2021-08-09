/**
 * @file src/video/window.h
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date July 27, 2021
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

#ifndef __HND_WINDOW_H__
#define __HND_WINDOW_H__

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

#include "../core/core.h"
#include "../core/event/event.h"
#include "video.h"
#include "renderer/renderer.h"

/**
 * @brief window data.
 */
typedef struct hnd_window_t
{
  char *title;
  unsigned int border;
  unsigned int left;
  unsigned int top;
  unsigned int width;
  unsigned int height;
  int running;

  xcb_connection_t *connection;
  xcb_screen_t *screen_data;
#ifdef HND_USE_OPENGL
  xcb_colormap_t colormap_id;
#endif /* HND_USE_OPENGL */

  xcb_window_t id;
  uint32_t event_mask;
  uint32_t value_mask;
  uint32_t value_list[3];

  xcb_atom_t utf8_string;
  xcb_atom_t wm_name;
  xcb_atom_t wm_protocols;
  xcb_atom_t wm_delete_window;

  hnd_renderer_t renderer;
} hnd_window_t;

/**
 * @brief Tries to connect to xorg server and creates a window
 *
 * @note If the window style doesn't display the upper bar, the
 *       title won't be displayed as well.
 *
 * @param _title  Specifies the text to be displayed on the new window's upper bar.
 * @param _border Specifies the border's width.
 * @param _left   Specifies the distance from the left side of the screen to the upper
                  left corner of the window.
 * @param _top    Specifies the distance from the top of the screen to the upper left
                  corner of the window.
 * @param _width  Specifies the window's width.
 * @param _height Specifies the window's height.
 *
 * @return The created window.
 */
hnd_window_t *
hnd_create_window
(
  const char   *_title,
  unsigned int  _border,
  unsigned int  _left,
  unsigned int  _top,
  unsigned int  _width,
  unsigned int  _height
);

/**
 * @brief Ends a window.
 *
 * @param _window Specifies the window to be destroyed.
 */
void
hnd_destroy_window
(
  hnd_window_t *_window
);

/**
 * @brief Gets the current event when active on window.
 *
 * @param _window Specifies the window where event should happen.
 * @param _event  Specifies the event struct where to allocate the poll.
 */
void
hnd_poll_events
(
  hnd_window_t *_window,
  hnd_event_t  *_event
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_WINDOW_H__ */
