/**
 * @file src/video/window.c
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

#include "window.h"

hnd_window_t *
hnd_create_window
(
  const char *_title,
  int         _width,
  int         _height
)
{
  hnd_window_t *new_window = malloc(sizeof(hnd_window_t));
  if (!hnd_assert(new_window != NULL, NULL))
    return NULL;

  new_window->x_server_connection = xcb_connect(NULL, &new_window->screen_number);

  /* Get current screen's data */
  const xcb_setup_t *connection_setup = xcb_get_setup(new_window->x_server_connection);
  xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(connection_setup);
  for (int i = 0; i < new_window->screen_number; ++i)
    xcb_screen_next(&screen_iterator);
  new_window->screen_data = screen_iterator.data;

  return new_window;
}

void
hnd_destroy_window
(
  hnd_window_t *_window
)
{
  if (!hnd_assert(_window != NULL, HND_SYNTAX))
    return;

  xcb_disconnect(_window->x_server_connection);
}
