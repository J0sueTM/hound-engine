/**
 * @file test/window.c
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

#include "../src/video/window.h"

int
main
(
  void
)
{
  hnd_window_t *window = hnd_create_window("Hound Engine Window Test", 100, 100, 800, 600);

  hnd_event_t event;
  int should_close = 0;
  while (!should_close)
  {
    hnd_clear_window(0.2f, 0.2f, 0.2f, 1.0f);
    hnd_poll_window_events(window->connection.xcb_connection, &event);

    if (event.pressed_keys[0] == HND_KEY_ESC)
      should_close = 1;

    hnd_swap_window_buffers(window);
  }

  hnd_destroy_window(window);
  
  return (0);
}
