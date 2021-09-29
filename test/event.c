/**
 * @file test/event/event.c
 * @author Josue Teodoro Moreira <teodoro.josue@protonmail.ch>
 * @date September 29, 2021
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

#include "../src/hound.h"

int
main
(
  void
)
{
  hnd_window_t *window = hnd_create_window("Hound Engine Keyboard Event Test",
                                           0,
                                           0,
                                           800,
                                           600,
                                           HND_WINDOW_DECORATION_ALL);

  hnd_set_renderer_clear_color(0.2f, 0.2f, 0.2f, 1.0f);

  hnd_event_t event;
  while (window->running)
  {
    hnd_clear_render();
    hnd_poll_events(window, &event);

    if (event.type == HND_EVENT_KEY_PRESS)
    {
      printf("Key pressed: %d\n", event.keyboard.pressed_key);

      if (event.keyboard.pressed_key == HND_KEY_ESC)
        window->running = HND_NK;
    }
    else if (event.type == HND_EVENT_KEY_RELEASE)
      printf("Key released: %d\n", event.keyboard.released_key);
    else if (event.type == HND_EVENT_MOUSE_BUTTON_PRESS)
      printf("Button pressed: %d\n", event.mouse.pressed_button);
    else if (event.type == HND_EVENT_MOUSE_BUTTON_RELEASE)
      printf("Button released: %d\n", event.mouse.released_button);

    hnd_swap_renderer_buffers(&window->renderer);
  }

  hnd_destroy_window(window);

  return 0;
}
