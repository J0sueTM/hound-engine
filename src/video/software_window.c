/**
 * @file src/video/software_window.c
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

/**
 * @brief Connects to X server.
 *
 * @param _connection Specifies the hound connection where xcb connection resides.
 *
 * @return Function state. HND_OK or HND_NK.
 */
static int
hnd_connect_to_xcb
(
  hnd_connection_t *_connection
)
{
  _connection->xcb_connection = xcb_connect(NULL, &_connection->default_screen);
  if (!hnd_assert(_connection->xcb_connection != NULL, NULL))
    return HND_NK;

  /* Get screen data */
  xcb_screen_iterator_t screen_iterator = xcb_setup_roots_iterator(xcb_get_setup(_connection->xcb_connection));
  for (int i = _connection->default_screen; i > 0; --i)
    xcb_screen_next(&screen_iterator);
  _connection->screen_data = screen_iterator.data;

  return HND_OK;
}

hnd_window_t *
hnd_create_window
(
  const char   *_title,
  unsigned int  _left,
  unsigned int  _top,
  unsigned int  _width,
  unsigned int  _height
)
{
  hnd_window_t *new_window = malloc(sizeof(hnd_window_t));
  if (!hnd_assert(new_window != NULL, NULL))
    return NULL;

  if (!hnd_connect_to_xcb(&new_window->connection))
  {
    hnd_destroy_window(new_window);

    return NULL;
  }

  new_window->title = (char *)_title;
  new_window->width = _width;
  new_window->height = _height;

  /* Create window */
  new_window->id = xcb_generate_id(new_window->connection.xcb_connection);

  new_window->event_mask = XCB_EVENT_MASK_EXPOSURE  |
                           XCB_EVENT_MASK_KEY_PRESS |
                           XCB_EVENT_MASK_KEY_RELEASE;
  new_window->value_mask = XCB_CW_EVENT_MASK;

  new_window->value_list[0] = new_window->event_mask;
  new_window->value_list[1] = 0;
  new_window->value_list[2] = 0;

  xcb_create_window(new_window->connection.xcb_connection,
                    XCB_COPY_FROM_PARENT,
                    new_window->id,
                    new_window->connection.screen_data->root,
                    _left, _top,
                    _width, _height,
                    10,
                    XCB_WINDOW_CLASS_INPUT_OUTPUT,
                    new_window->connection.screen_data->root_visual,
                    new_window->value_mask,
                    new_window->value_list);

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

  if (_window->connection.xcb_connection)
    xcb_disconnect(_window->connection.xcb_connection);

  free(_window);

  hnd_print_debug(HND_LOG, HND_ENDED("window"), HND_SUCCESS);
}
