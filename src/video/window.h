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
#include "video.h"

/**
 * @brief Data oub connection with xcb and current monitor/screen.
 */
typedef struct hnd_connection_t
{
  xcb_connection_t *xcb_connection; ///< Xcb/X11 server connection.
  xcb_screen_t *screen_data;        ///< Current monitor/screen data.
  int default_screen;               ///< Default Monitor/screen id.

#ifdef HND_USE_OPENGL
  Display *display;                  ///< Xlib display.
  GLXFBConfig *fb_configs;           ///< Frame buffer configs.
  GLXFBConfig current_fb_config;     ///< Current frame buffer config.
  int fb_config_count;               ///< Amount of fb_configs.
  int visual_id;                     ///< Current frame buffer config's visual id.
#endif /* HND_USE_OPENGL */
} hnd_connection_t;

/**
 * @brief window data.
 */
typedef struct hnd_window_t
{
  char *title;
  unsigned int width;
  unsigned int height;

  hnd_connection_t connection;
  xcb_window_t id;
  xcb_colormap_t colormap_id;

  uint32_t event_mask;
  uint32_t value_mask;
  uint32_t value_list[3];

#ifdef HND_USE_OPENGL
  GLXContext gl_context;
  GLXWindow gl_window;
#endif /* HND_USE_OPENGL */
} hnd_window_t;

/**
 * @brief Tries to connect to xorg server and creates a window
 *
 * @note If the window style doesn't display the upper bar, the
 *       title won't be displayed as well.
 *
 * @param _title  Specifies the text to be displayed on the new window's upper bar.
 * @param _left   Specifies the distance from the left side of the screen to the upper
                  left corner of the window.
 * @param _top    Specifies the distance from the top of the screen to the upper left
                  corner of the window.
 * @param _width  Specifies the window's width.
 * @param _height Specifies the window's height.
 *
 * @return Created window.
 */
hnd_window_t *
hnd_create_window
(
  const char   *_title,
  unsigned int  _left,
  unsigned int  _top,
  unsigned int  _width,
  unsigned int  _height
);

/**
 * @brief Ends a window.
 *
 * @param _window Specifies the window to be destroyed.
 * @note Since a window should be allocated to the beginning to the end
 *       of hound's lifetime, you don't need to call this function. Let
 *       linux free your memory.
 */
void
hnd_destroy_window
(
  hnd_window_t *_window
);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HND_WINDOW_H__ */
