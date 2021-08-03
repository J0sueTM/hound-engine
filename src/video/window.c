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

static void
hnd_connect_to_xcb
(
  hnd_connection_t *_connection
)
{
  _connection->xcb_connection = xcb_connect(NULL, &_connection->screen_number);
  if (!hnd_assert(_connection->xcb_connection != NULL, NULL))
    return;

  /* Get screen data */
  const xcb_setup_t *connection_setup = xcb_get_setup(_connection->xcb_connection);

  /* Setup screen iterator */
  xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(connection_setup);
  for (int i = 0; i < _connection->screen_number; ++i)
    xcb_screen_next(&screen_iterator);
  _connection->screen_data = screen_iterator.data;
}

hnd_window_t *
hnd_create_window
(
  const char   *_title,
  unsigned int _left,
  unsigned int _top,
  unsigned int _width,
  unsigned int _height
)
{
  hnd_window_t *new_window = malloc(sizeof(hnd_window_t));
  if (!hnd_assert(new_window != NULL, NULL))
    return NULL;

  new_window->title = (char *)_title;
  new_window->width = _width;
  new_window->height = _height;

  hnd_connect_to_xcb(&new_window->connection);

  new_window->id = xcb_generate_id(new_window->connection.xcb_connection);
  xcb_create_window(new_window->connection.xcb_connection,
                    XCB_COPY_FROM_PARENT,
                    new_window->id,
                    new_window->connection.screen_data->root,
                    _left, _top,
                    _width, _height,
                    10,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    new_window->connection.screen_data->root_visual,
                    0,
                    NULL);

  xcb_map_window(new_window->connection.xcb_connection, new_window->id);
  xcb_flush(new_window->connection.xcb_connection);

  hnd_print_debug(HND_LOG, HND_CREATED("window"), HND_SUCCESS);
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

  xcb_disconnect(_window->connection.xcb_connection);
  hnd_print_debug(HND_LOG, HND_ENDED("window"), HND_SUCCESS);
}
